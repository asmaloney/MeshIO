#pragma once
// Copyright © 2019 Andy Maloney <asmaloney@gmail.com>
// SPDX-License-Identifier: BSD-3-Clause

#include "mioAbstractLoader.h"

class OBJFilter : public mioAbstractLoader
{
public:
   QStringList getFileFilters( bool onImport ) const override;
   QString getDefaultExtension() const override;

   bool canLoadExtension( const QString &inUpperCaseExt ) const override;
};
