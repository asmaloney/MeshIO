#include <QDebug>
#include <QString>

#include "PLY.h"

QStringList PLYFilter::getFileFilters( bool onImport ) const
{
   if ( onImport )
   {
      // import
      return {
         QStringLiteral( "MeshIO - PLY file (*.ply)" ),
      };
   }
   else
   {
      // export
      return {};
   }
}

QString PLYFilter::getDefaultExtension() const
{
   return QStringLiteral( "ply" );
}

bool PLYFilter::canLoadExtension( const QString &inUpperCaseExt ) const
{
   const QStringList extensions{
      "PLY",
   };

   return extensions.contains( inUpperCaseExt );
}
