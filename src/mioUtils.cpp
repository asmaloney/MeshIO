// MeshIO Copyright © 2019 Andy Maloney <asmaloney@gmail.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <QImageReader>
#include <QRegularExpression>
#include <QVector3D>

#include "ccHObjectCaster.h"
#include "ccMaterialSet.h"
#include "ccMesh.h"
#include "ccPointCloud.h"

#include "assimp/material.h"
#include "assimp/mesh.h"
#include "assimp/metadata.h"

#include "mioUtils.h"


namespace mioUtils
{
   ccMaterial::Shared   _createMaterial( const QString &inPath, const QString &inName, const QString &inTexturePath )
   {
      static QRegularExpression sRegExp( "^\\*(?<index>[0-9]+)$" );
            
      ccMaterial::Shared mat;
      
      auto  match = sRegExp.match( inTexturePath );
            
      if ( match.hasMatch() )
      {
         const QString cIndex = match.captured( "index" );
                  
         ccLog::Warning( QStringLiteral( "[MeshIO] Embedded materials not yet handled" ) );
      }
      else
      {
         const QString    cPath = QStringLiteral( "%1/%2" ).arg( inPath, inTexturePath );
         
         if ( !QFile::exists( cPath ) )
         {
            ccLog::Warning( QStringLiteral( "[MeshIO] Material not found: '%1'" ).arg( cPath ) );
            return {};
         }
         
         QImageReader reader( cPath );
         
         QImage   textureImage = reader.read();
         
         mat = ccMaterial::Shared( new ccMaterial( inName ) );
         mat->setTexture( textureImage, cPath, false );
      }
      
      return mat;
   }
   
   ccMaterialSet *createMaterialSetForMesh( const aiMesh *inMesh, const QString &inPath, unsigned int inNumMaterial, aiMaterial **inMaterials)
   {
      if ( inNumMaterial == 0 )
      {
         return nullptr;
      }
      
      unsigned int  index = inMesh->mMaterialIndex;
      const auto    material = inMaterials[index];
      
      if ( material->GetTextureCount( aiTextureType_DIFFUSE ) == 0 )
      {
         return nullptr;
      }
      
      aiString name = material->GetName();
      aiString texturePath;
      
      // we only handle the diffuse material for now
      if ( material->GetTexture( aiTextureType_DIFFUSE, 0, &texturePath ) != AI_SUCCESS )
      {
         return nullptr;
      }
      
      auto mat = _createMaterial( inPath, name.C_Str(), texturePath.C_Str() );
      
      if ( mat.isNull() )
      {
         return nullptr;
      }
      
      ccMaterialSet *materialSet = new ccMaterialSet( "Materials" );
      
      materialSet->addMaterial( mat );
      
      return materialSet;
   }
   
   ccMesh   *newCCMeshFromAIMesh( const aiMesh *inMesh )
   {
      auto  newPC = new ccPointCloud( "Vertices" );
      auto  newMesh = new ccMesh( newPC );
      
      QString   name( inMesh->mName.C_Str() );
      
      if ( name.isEmpty() )
      {
         name = QStringLiteral( "Mesh" );
      }
      
      ccLog::Print( QStringLiteral( "[MeshIO] Mesh '%1' has %2 verts & %3 faces" )
                       .arg( name,
                             QLocale::system().toString( inMesh->mNumVertices ),
                             QLocale::system().toString( inMesh->mNumFaces ) ) );
      
      if ( !inMesh->HasPositions() || !inMesh->HasFaces() )
      {
         ccLog::Warning( QStringLiteral( "[MeshIO] Mesh '%1' does not have vertices or faces" ).arg( name ) );
         
         delete newPC;
         delete newMesh;
         
         return nullptr;
      }
      
      // reserve memory for points and mesh (because we need to do this before other memory allocations)
      newPC->reserveThePointsTable( inMesh->mNumVertices );
      
      if ( inMesh->HasFaces() )
      {
         newMesh->reserve( inMesh->mNumFaces );
      }
      
      // normals
      if ( inMesh->HasNormals() )
      {
         bool   allocated = newPC->reserveTheNormsTable();
         
         if ( !allocated )
         {
            ccLog::Warning( QStringLiteral( "[MeshIO] Cannot allocate normals for mesh '%1'" ).arg( name ) );
         }
      }
      
      // texture coordinates
      bool  hasTextureCoordinates = inMesh->HasTextureCoords( 0 );
      
      TextureCoordsContainer    *texCoords = nullptr;
      
      if ( hasTextureCoordinates )
      {
         texCoords = new TextureCoordsContainer;
         
         texCoords->reserve( inMesh->mNumVertices );
         
         bool	allocated = texCoords->isAllocated();
         
         allocated &= newMesh->reservePerTriangleTexCoordIndexes();
         allocated &= newMesh->reservePerTriangleMtlIndexes();
         
         if ( !allocated )
         {
            delete texCoords;
            hasTextureCoordinates = false;
            ccLog::Warning( QStringLiteral( "[MeshIO] Cannot allocate texture coordinates for mesh '%1'" ).arg( name ) );
         }
         else
         {
            newMesh->setTexCoordinatesTable( texCoords );
         }
      }
      
      // vertices
      for ( unsigned int i = 0; i < inMesh->mNumVertices; ++i )
      {
         const aiVector3D &point = inMesh->mVertices[i];
         
         CCVector3 point2( static_cast<PointCoordinateType>( point.x ),
                           static_cast<PointCoordinateType>( point.y ),
                           static_cast<PointCoordinateType>( point.z ) );
         
         newPC->addPoint( point2 );
         
         // normals
         if ( newPC->hasNormals() )
         {
            const aiVector3D &normal = inMesh->mNormals[i];
            
            CCVector3 normal2( static_cast<PointCoordinateType>( normal.x ),
                               static_cast<PointCoordinateType>( normal.y ),
                               static_cast<PointCoordinateType>( normal.z ) );
            
            newPC->addNorm( normal2 );
         }
         
         // texture coordinates
         if ( hasTextureCoordinates )
         {
            const aiVector3D &texCoord = inMesh->mTextureCoords[0][i];
            
            const TexCoords2D coord{ texCoord.x, texCoord.y };
            
            texCoords->addElement( coord );
         }
      }
      
      newPC->setEnabled( false );
      
      // faces
      if ( inMesh->HasFaces() )
      {
         newMesh->reserve( inMesh->mNumFaces );
         
         for ( unsigned int i = 0; i < inMesh->mNumFaces; ++i )
         {
            const aiFace    &face = inMesh->mFaces[i];
            
            if ( face.mNumIndices != 3 )
            {
               continue;
            }
            
            newMesh->addTriangle( face.mIndices[0],
                                  face.mIndices[1],
                                  face.mIndices[2] );
            
            // texture coordinates
            if ( hasTextureCoordinates )
            {
               newMesh->addTriangleMtlIndex( 0 );
               
               newMesh->addTriangleTexCoordIndexes( static_cast<int>(face.mIndices[0]),
                                                    static_cast<int>(face.mIndices[1]),
                                                    static_cast<int>(face.mIndices[2]) );
            }
         }
      }
      
      if ( newMesh->size() == 0 )
      {
         ccLog::Warning( QStringLiteral( "[MeshIO] Mesh '%1' does not have any faces" ).arg( name ) );
         
         delete newPC;
         delete newMesh;
         
         return nullptr;
      }
      
      newMesh->setName( name );
      newMesh->setVisible( true );
      
      if ( !newPC->hasNormals() )
      {
         ccLog::Warning( QStringLiteral( "[MeshIO] Mesh '%1' does not have normals - will compute them per vertex" ).arg( name ) );
         
         newMesh->computeNormals( true );
      }
      
      newMesh->showNormals( true );
      newMesh->addChild( newPC );
      
      return newMesh;
   }
   
   ccGLMatrix convertMatrix( const aiMatrix4x4 &inAssimpMatrix )
   {
      const int	cWidth = 4;
      const int	cHeight = 4;
      
      PointCoordinateType   data[OPENGL_MATRIX_SIZE];
      
      for ( unsigned int i = 0; i < cWidth; ++i)
      {
         for ( unsigned int j = 0; j < cHeight; ++j)
         {
            data[j * cHeight + i] = static_cast<PointCoordinateType>(inAssimpMatrix[i][j]);
         }
      }
      
      return ccGLMatrix( data );
   }
   
   QVariant convertMetaValueToVariant( aiMetadata *inData, unsigned int inValueIndex )
   {
      QVariant	metaValue;
      
      switch ( inData->mValues[inValueIndex].mType )
      {
         case AI_BOOL:
         {
            bool    value = false;
            
            inData->Get<bool>( inValueIndex, value );
            
            metaValue = value;
            break;
         }
            
         case AI_INT32:
         {
            int32_t value = 0;
            
            inData->Get<int32_t>( inValueIndex, value );
            
            metaValue = value;
            break;
         }
            
         case AI_UINT64:
         {
            uint64_t    value = 0;
            
            inData->Get<uint64_t>( inValueIndex, value );
            
            metaValue = value;
            break;
         }
            
         case AI_FLOAT:
         {
            float   value = 0;
            
            inData->Get<float>( inValueIndex, value );
            
            metaValue = value;
            break;
         }
            
         case AI_DOUBLE:
         {
            double  value = 0;
            
            inData->Get<double>( inValueIndex, value );
            
            metaValue = value;
            break;
         }
            
         case AI_AISTRING:
         {
            aiString    value;
            
            inData->Get<aiString>( inValueIndex, value );
            
            metaValue = value.C_Str();
            break;
         }
            
         case AI_AIVECTOR3D:
         {
            aiVector3D  value;
            
            inData->Get<aiVector3D>( inValueIndex, value );
            
            metaValue = QVector3D( value.x, value.y, value.z );
            break;
         }
            
         case AI_META_MAX:
         case FORCE_32BIT:
         {
            // This is necessary to avoid a warning.
            // Assimp doesn't use enum type specifiers.
            // It uses this odd trick w/FORCE_32BIT to force the type of the enum.
            break;
         }
      }
      
      return metaValue;
   }
}
   