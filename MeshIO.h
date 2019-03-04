#ifndef MESH_IO_HEADER
#define MESH_IO_HEADER

// Copyright © 2019 Andy Maloney <asmaloney@gmail.com>
// SPDX-License-Identifier: BSD-3-Clause

#include "ccIOFilterPluginInterface.h"


class MeshIO : public QObject, public ccIOFilterPluginInterface
{
   Q_OBJECT
   Q_INTERFACES( ccIOFilterPluginInterface )
   
   Q_PLUGIN_METADATA(IID "com.asmaloney.plugin.MeshIO" FILE "info.json")
   
 public:
   explicit MeshIO( QObject *parent = nullptr );
   
 protected:
   void registerCommands( ccCommandLineInterface *inCmdLine ) override;
   
   QVector<FileIOFilter::Shared> getFilters() override;
};

#endif
