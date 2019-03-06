// MeshIO Copyright © 2019 Andy Maloney <asmaloney@gmail.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <QCoreApplication>
#include <QString>

#include "IFC.h"


IFCFilter::IFCFilter() :
   mNameMatcher( "^(?<type>Ifc[^_]+)_(?<name>.*)_(?<guid>.{22})$" )
{
   mNameMatcher.optimize();
}

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

void IFCFilter::_postProcess( ccHObject &ioContainer )
{
   _recursiveRename( &ioContainer );
}

void IFCFilter::_recursiveRename( ccHObject *ioContainer )
{
   const auto    cChildCount = ioContainer->getChildrenNumber();
   
   for ( unsigned int i = 0; i < cChildCount; ++i )
   {
      _recursiveRename( ioContainer->getChild( i ) );
   }
   
   if ( ioContainer->getName() == QLatin1String( "$RelAggregates" ) )
   {
       
      ioContainer->setName( QCoreApplication::translate( "MeshIO", "Unnamed Group" ) );
   }
   else
   {
      auto match = mNameMatcher.match( ioContainer->getName() );
      
      if ( match.hasMatch() )
      {
         // Split names of the form:
         //    IfcBeam_Holzbalken-4_1X4dAmiW97Nurao$8fngpg
         // into type, name, and GUID:
         //    IfcBeam, Holzbalken-4, 1X4dAmiW97Nurao$8fngpg
         // Then rename our node with "name" put "type" and "GUID" into our meta data
         
         const QString cType = match.captured( "type" );
         const QString cName = match.captured( "name" );
         const QString cGUID = match.captured( "guid" );
         
         ioContainer->setMetaData( "IFC Type", cType );
         ioContainer->setMetaData( "IFC GUID", cGUID );
         
         ioContainer->setName( cName );
      }
   }
}
