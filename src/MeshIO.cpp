// Copyright © 2019 Andy Maloney <asmaloney@gmail.com>
// SPDX-License-Identifier: BSD-3-Clause

#include "assimp/version.h"

#include "MeshIO.h"

#include "COLLADA.h"
#include "IFC.h"
#include "glTF.h"

MeshIO::MeshIO( QObject *parent ) : QObject( parent ), ccIOPluginInterface( ":/asmaloney/MeshIO/info.json" )
{
   const QString cAssimpVer = QStringLiteral( "[MeshIO] Using Assimp %1.%2 (%3-%4)" )
                                 .arg( QString::number( aiGetVersionMajor() ), QString::number( aiGetVersionMinor() ) )
                                 .arg( aiGetVersionRevision(), 0, 16 )
                                 .arg( aiGetBranchName() );

   ccLog::Print( cAssimpVer );
}

void MeshIO::registerCommands( ccCommandLineInterface *inCmdLine )
{
   Q_UNUSED( inCmdLine );
}

ccIOPluginInterface::FilterList MeshIO::getFilters()
{
   // clang-format off
   return {
      FileIOFilter::Shared( new COLLADAFilter ),
      FileIOFilter::Shared( new glTFFilter ),
      FileIOFilter::Shared( new IFCFilter ), 
   };
}
