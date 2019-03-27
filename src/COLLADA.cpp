// MeshIO Copyright © 2019 Andy Maloney <asmaloney@gmail.com>
// SPDX-License-Identifier: BSD-3-Clause

#include "COLLADA.h"


COLLADAFilter::COLLADAFilter() :
   mioAbstractLoader( {
      "_COLLADA Filter",
      FileIOFilter::DEFAULT_PRIORITY,	// priority
      QStringList{ "dae" },
      "dae",
      QStringList{ "MeshIO - COLLADA file (*.dae)" },
      QStringList(),
      Import
   } ) 
{    
}
