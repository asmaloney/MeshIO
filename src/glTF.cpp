// MeshIO Copyright © 2019 Andy Maloney <asmaloney@gmail.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <QDebug>
#include <QString>

#include "glTF.h"


QStringList glTFFilter::getFileFilters( bool onImport ) const
{
   if ( onImport )
   {
      // import
      return {
         QStringLiteral( "MeshIO - glTF file (*.gltf)" ),
      };
   }
   else
   {
      // export
      return {};
   }
}

QString glTFFilter::getDefaultExtension() const
{
   return QStringLiteral( "gltf" );
}

bool glTFFilter::canLoadExtension( const QString &inUpperCaseExt ) const
{
   const QStringList extensions{
      "GLTF",
   };
   
   return extensions.contains( inUpperCaseExt );
}
