// MeshIO Copyright © 2019 Andy Maloney <asmaloney@gmail.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <QDebug>
#include <QString>

#include "COLLADA.h"


QStringList COLLADAFilter::getFileFilters( bool onImport ) const
{
   if ( onImport )
   {
      // import
      return {
         QStringLiteral( "MeshIO - COLLADA file (*.dae)" ),
      };
   }
   else
   {
      // export
      return {};
   }
}

QString COLLADAFilter::getDefaultExtension() const
{
   return QStringLiteral( "dae" );
}

bool COLLADAFilter::canLoadExtension( const QString &inUpperCaseExt ) const
{
   const QStringList extensions{
      "DAE",
   };
   
   return extensions.contains( inUpperCaseExt );
}
