#include <QDebug>
#include <QString>

#include "OBJ.h"

OBJFilter::OBJFilter() :
   // clang-format off
   mioAbstractLoader( {
      "MeshIO OBJ Filter",
      FileIOFilter::DEFAULT_PRIORITY,
      QStringList{ "obj" },
      "obj",
      QStringList{ "MeshIO - OBJ file (*.obj)" },
      QStringList(),
      Import
   } )
{
}
