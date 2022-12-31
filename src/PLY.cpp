#include <QDebug>
#include <QString>

#include "PLY.h"

PLYFilter::PLYFilter() :
   // clang-format off
   mioAbstractLoader( {
	  "MeshIO PLY Filter",
	  FileIOFilter::DEFAULT_PRIORITY,
	  QStringList{ "ply" },
	  "ply",
	  QStringList{ "MeshIO - PLY file (*.ply)" },
	  QStringList(),
	  Import
   } )
{
}
