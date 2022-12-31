// MeshIO Copyright © 2019 Andy Maloney <asmaloney@gmail.com>
// SPDX-License-Identifier: BSD-3-Clause

#include "glTF.h"

glTFFilter::glTFFilter() :
   // clang-format off
   mioAbstractLoader( {
      "MeshIO glTF Filter",
      FileIOFilter::DEFAULT_PRIORITY,
      QStringList{ "gltf", "glb" },
      "gltf",
      QStringList{ "MeshIO - glTF file (*.gltf *.glb)" },
      QStringList(),
      Import
   } )   
{   
}
