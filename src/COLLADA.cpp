// MeshIO Copyright © 2019 Andy Maloney <asmaloney@gmail.com>
// SPDX-License-Identifier: BSD-3-Clause

#include "COLLADA.h"

COLLADAFilter::COLLADAFilter() :
   // clang-format off
   mioAbstractLoader( {
      "MeshIO COLLADA Filter",
      FileIOFilter::DEFAULT_PRIORITY,
      QStringList{ "dae" },
      "dae",
      QStringList{ "MeshIO - COLLADA file (*.dae)" },
      QStringList(),
      Import
   } ) 
{    
}
