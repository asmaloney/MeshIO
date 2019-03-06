// MeshIO Copyright © 2019 Andy Maloney <asmaloney@gmail.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <QDebug>
#include <QString>

#include "IFC.h"


QStringList IFCFilter::getFileFilters( bool onImport ) const
{
   if ( onImport )
   {
      // import
      return {
         QStringLiteral( "MeshIO - IFC file (*.ifc *.stp *.step)" ),
      };
   }
   else
   {
      // export
      return {};
   }
}

QString IFCFilter::getDefaultExtension() const
{
   return QStringLiteral( "ifc" );
}

bool IFCFilter::canLoadExtension( const QString &inUpperCaseExt ) const
{
   const QStringList extensions{
      "IFC",
      "STP",
      "STEP",
   };
   
   return extensions.contains( inUpperCaseExt );
}
