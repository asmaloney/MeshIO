#ifndef MF_UTILS_HEADER
#define MF_UTILS_HEADER

// MeshIO Copyright © 2019 Andy Maloney <asmaloney@gmail.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <QMap>

#include "assimp/matrix4x4.h"

class QString;
class QVariant;

class ccGLMatrix;
class ccMaterialSet;
class ccMesh;

class aiMaterial;
class aiMesh;
class aiMetadata;


namespace mioUtils
{
   // This maps an aiMaterial index to a ccMaterialSet index
   using MaterialIndexMap = QMap<unsigned int, int>;
   
   ccMaterialSet    *createSharedMaterialSet( const QString &inPath, unsigned int inNumMaterial, aiMaterial **inMaterials, MaterialIndexMap &outMap );
   ccMaterialSet    *createMaterialSetForMesh( const aiMesh *inMesh, const QString &inPath, unsigned int inNumMaterial, aiMaterial **inMaterials );
   
   ccMesh   *newCCMeshFromAIMesh( const aiMesh *inMesh );
   
   ccGLMatrix   convertMatrix( const aiMatrix4x4 &inAssimpMatrix );
   
   QVariant convertMetaValueToVariant( aiMetadata *inData, unsigned int inValueIndex );
}

#endif
