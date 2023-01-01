// MeshIO Copyright © 2019 Andy Maloney <asmaloney@gmail.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <QImageReader>
#include <QRegularExpression>
#include <QVector3D>

#include "ccMaterialSet.h"
#include "ccMesh.h"
#include "ccPointCloud.h"

#include "assimp/material.h"
#include "assimp/mesh.h"
#include "assimp/metadata.h"
#include "assimp/scene.h"

#include "mioUtils.h"

namespace
{
   QImage _getEmbeddedTexture( unsigned int inTextureIndex, const aiScene *inScene )
   {
      QImage image;

      if ( inScene->mNumTextures == 0 )
      {
         ccLog::Warning( QStringLiteral( "[MeshIO] Scene requests embedded texture, but there are none" ) );
         return image;
      }

      auto texture = inScene->mTextures[inTextureIndex];

      // From assimp: "If mHeight is zero the texture is compressed"
      bool isCompressed = ( texture->mHeight == 0 );

      if ( !isCompressed )
      {
         ccLog::Warning( QStringLiteral( "[MeshIO] Uncompressed embedded textures not yet implemented" ) );
         return image;
      }

      // From assimp: "mWidth specifies the size of the memory area pcData is pointing to, in bytes"
      auto dataSize = static_cast<const int32_t>( texture->mWidth );

      const QByteArray imageDataByteArray( reinterpret_cast<const char *>( texture->pcData ), dataSize );

      return QImage::fromData( imageDataByteArray );
   }

   QImage _getTextureFromFile( const QString &inPath, const QString &inTexturePath )
   {
      const QString cPath = QStringLiteral( "%1/%2" ).arg( inPath, inTexturePath );

      if ( !QFile::exists( cPath ) )
      {
         ccLog::Warning( QStringLiteral( "[MeshIO] Material not found: '%1'" ).arg( cPath ) );
         return {};
      }

      QImageReader reader( cPath );

      QImage texture = reader.read();

      if ( texture.isNull() )
      {
         ccLog::Error(
            QStringLiteral( "[MeshIO] Error reading material: '%1' - %2" ).arg( cPath, reader.errorString() ) );
      }

      return texture;
   }

   inline ccColor::Rgbaf _convertColour( const aiColor3D &inColour )
   {
      return ccColor::Rgbaf{ inColour.r, inColour.g, inColour.b, 0.0f };
   }

   inline ccColor::Rgbaf _convertColour( const aiColor4D &inColour )
   {
      return ccColor::Rgbaf{ inColour.r, inColour.g, inColour.b, inColour.a };
   }

   // Map all the material properties we know about from assimp
   void _assignMaterialProperties( aiMaterial *inAIMaterial, ccMaterial::Shared &inCCMaterial )
   {
      aiColor4D colour;

      if ( inAIMaterial->Get( AI_MATKEY_COLOR_DIFFUSE, colour ) == AI_SUCCESS )
      {
         inCCMaterial->setDiffuse( _convertColour( colour ) );
      }

      if ( inAIMaterial->Get( AI_MATKEY_COLOR_AMBIENT, colour ) == AI_SUCCESS )
      {
         inCCMaterial->setAmbient( _convertColour( colour ) );
      }

      if ( inAIMaterial->Get( AI_MATKEY_COLOR_SPECULAR, colour ) == AI_SUCCESS )
      {
         inCCMaterial->setSpecular( _convertColour( colour ) );
      }

      if ( inAIMaterial->Get( AI_MATKEY_COLOR_EMISSIVE, colour ) == AI_SUCCESS )
      {
         inCCMaterial->setEmission( _convertColour( colour ) );
      }

      ai_real property;

      if ( inAIMaterial->Get( AI_MATKEY_SHININESS, property ) == AI_SUCCESS )
      {
         inCCMaterial->setShininess( property );
      }

      if ( inAIMaterial->Get( AI_MATKEY_OPACITY, property ) == AI_SUCCESS )
      {
         inCCMaterial->setTransparency( property );
      }
   }
}

namespace mioUtils
{
   // For assimp details, see:
   //	https://assimp-docs.readthedocs.io/en/latest/usage/use_the_lib.html#material-system
   ccMaterialSet *createMaterialSetForMesh( const aiMesh *inMesh, const QString &inPath, const aiScene *inScene )
   {
      if ( inScene->mNumMaterials == 0 )
      {
         return nullptr;
      }

      unsigned int index = inMesh->mMaterialIndex;
      const auto aiMaterial = inScene->mMaterials[index];

      const aiString cName = aiMaterial->GetName();

      auto material = ccMaterial::Shared( new ccMaterial( cName.C_Str() ) );

      ccLog::PrintDebug( QStringLiteral( "[MeshIO] Creating material '%1'" ).arg( material->getName() ) );

      aiColor3D color;

      // Set colours
      auto found = aiMaterial->Get( AI_MATKEY_COLOR_DIFFUSE, color );
      if ( found == aiReturn_SUCCESS )
      {
         material->setDiffuse( _convertColour( color ) );
      }

      found = aiMaterial->Get( AI_MATKEY_COLOR_SPECULAR, color );
      if ( found == aiReturn_SUCCESS )
      {
         material->setSpecular( _convertColour( color ) );
      }

      found = aiMaterial->Get( AI_MATKEY_COLOR_AMBIENT, color );
      if ( found == aiReturn_SUCCESS )
      {
         material->setAmbient( _convertColour( color ) );
      }

      found = aiMaterial->Get( AI_MATKEY_COLOR_EMISSIVE, color );
      if ( found == aiReturn_SUCCESS )
      {
         material->setEmission( _convertColour( color ) );
      }

      // Set Textures
      // We only handle the diffuse texture for now
      if ( aiMaterial->GetTextureCount( aiTextureType_DIFFUSE ) > 0 )
      {
         aiString texturePath;

         if ( aiMaterial->GetTexture( aiTextureType_DIFFUSE, 0, &texturePath ) != AI_SUCCESS )
         {
            ccLog::Warning( QStringLiteral( "[MeshIO] Could not get texture path" ) );
         }
         else
         {
            static QRegularExpression sRegExp( "^\\*(?<index>[0-9]+)$" );

            auto match = sRegExp.match( texturePath.C_Str() );

            QImage image;
            QString path;

            if ( match.hasMatch() )
            {
               const QString cIndex = match.captured( "index" );

               image = _getEmbeddedTexture( cIndex.toUInt(), inScene );
            }
            else
            {
               image = _getTextureFromFile( inPath, texturePath.C_Str() );
               path = QStringLiteral( "%1/%2" ).arg( inPath, texturePath.C_Str() );
            }

            if ( !image.isNull() )
            {
               material->setTexture( image, path );
            }
         }
      }

      _assignMaterialProperties( aiMaterial, material );

      ccMaterialSet *materialSet = new ccMaterialSet( "Materials" );

      materialSet->addMaterial( material );

      return materialSet;
   }

   ccMesh *newCCMeshFromAIMesh( const aiMesh *inMesh )
   {
      auto newPC = new ccPointCloud( "Vertices" );
      auto newMesh = new ccMesh( newPC );

      QString name( inMesh->mName.C_Str() );

      if ( name.isEmpty() )
      {
         name = QStringLiteral( "Mesh" );
      }

      ccLog::Print( QStringLiteral( "[MeshIO] Mesh '%1' has %2 verts & %3 faces" )
                       .arg( name, QLocale::system().toString( inMesh->mNumVertices ),
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
         bool allocated = newPC->reserveTheNormsTable();

         if ( !allocated )
         {
            ccLog::Warning( QStringLiteral( "[MeshIO] Cannot allocate normals for mesh '%1'" ).arg( name ) );
         }
      }

      // texture coordinates
      bool hasTextureCoordinates = inMesh->HasTextureCoords( 0 );

      TextureCoordsContainer *texCoords = nullptr;

      if ( hasTextureCoordinates )
      {
         texCoords = new TextureCoordsContainer;

         texCoords->reserve( inMesh->mNumVertices );

         bool allocated = texCoords->isAllocated();

         allocated &= newMesh->reservePerTriangleTexCoordIndexes();
         allocated &= newMesh->reservePerTriangleMtlIndexes();

         if ( !allocated )
         {
            delete texCoords;
            hasTextureCoordinates = false;
            ccLog::Warning(
               QStringLiteral( "[MeshIO] Cannot allocate texture coordinates for mesh '%1'" ).arg( name ) );
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

         CCVector3 point2( static_cast<PointCoordinateType>( point.x ), static_cast<PointCoordinateType>( point.y ),
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
            const aiFace &face = inMesh->mFaces[i];

            if ( face.mNumIndices != 3 )
            {
               continue;
            }

            newMesh->addTriangle( face.mIndices[0], face.mIndices[1], face.mIndices[2] );

            // texture coordinates
            if ( hasTextureCoordinates )
            {
               newMesh->addTriangleMtlIndex( 0 );

               newMesh->addTriangleTexCoordIndexes( static_cast<int>( face.mIndices[0] ),
                                                    static_cast<int>( face.mIndices[1] ),
                                                    static_cast<int>( face.mIndices[2] ) );
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
         ccLog::Warning(
            QStringLiteral( "[MeshIO] Mesh '%1' does not have normals - will compute them per vertex" ).arg( name ) );

         newMesh->computeNormals( true );
      }

      newMesh->showNormals( true );
      newMesh->addChild( newPC );

      return newMesh;
   }

   ccGLMatrix convertMatrix( const aiMatrix4x4 &inAssimpMatrix )
   {
      const int cWidth = 4;
      const int cHeight = 4;

      PointCoordinateType data[OPENGL_MATRIX_SIZE];

      for ( unsigned int i = 0; i < cWidth; ++i )
      {
         for ( unsigned int j = 0; j < cHeight; ++j )
         {
            data[j * cHeight + i] = static_cast<PointCoordinateType>( inAssimpMatrix[i][j] );
         }
      }

      return ccGLMatrix( data );
   }

   QVariant convertMetaValueToVariant( aiMetadata *inData, unsigned int inValueIndex )
   {
      QVariant metaValue;

      switch ( inData->mValues[inValueIndex].mType )
      {
         case AI_BOOL:
         {
            bool value = false;

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
            uint64_t value = 0;

            inData->Get<uint64_t>( inValueIndex, value );

            metaValue = static_cast<qulonglong>( value );
            break;
         }

         case AI_FLOAT:
         {
            float value = 0;

            inData->Get<float>( inValueIndex, value );

            metaValue = value;
            break;
         }

         case AI_DOUBLE:
         {
            double value = 0;

            inData->Get<double>( inValueIndex, value );

            metaValue = value;
            break;
         }

         case AI_AISTRING:
         {
            aiString value;

            inData->Get<aiString>( inValueIndex, value );

            metaValue = value.C_Str();
            break;
         }

         case AI_AIVECTOR3D:
         {
            aiVector3D value;

            inData->Get<aiVector3D>( inValueIndex, value );

            metaValue = QVector3D( value.x, value.y, value.z );
            break;
         }

         case AI_AIMETADATA:
         {
            // This is recursive data used for glTF.
            // TODO: Figure out how to handle this properly
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
