// MeshIO Copyright © 2019 Andy Maloney <asmaloney@gmail.com>
// SPDX-License-Identifier: BSD-3-Clause

#include "glTF.h"


glTFFilter::glTFFilter() :
   mioAbstractLoader( {
      "_glTF Filter",
      FileIOFilter::DEFAULT_PRIORITY,	// priority
      QStringList{ "gltf", "glb" },
      "gltf",
      QStringList{ "MeshIO - glTF file (*.gltf *.glb)" },
      QStringList(),
      Import
   } )   
{   
}
