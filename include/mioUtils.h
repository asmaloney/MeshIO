#pragma once
// MeshIO Copyright © 2019 Andy Maloney <asmaloney@gmail.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <QMap>

#include "assimp/matrix4x4.h"

class QString;
class QVariant;

class ccGLMatrix;
class ccMaterialSet;
class ccMesh;

class aiMesh;
class aiMetadata;
class aiScene;


namespace mioUtils
{
   ccMaterialSet    *createMaterialSetForMesh( const aiMesh *inMesh, const QString &inPath, const aiScene *inScene );
   
   ccMesh   *newCCMeshFromAIMesh( const aiMesh *inMesh );
   
   ccGLMatrix   convertMatrix( const aiMatrix4x4 &inAssimpMatrix );
   
   QVariant convertMetaValueToVariant( aiMetadata *inData, unsigned int inValueIndex );
}
