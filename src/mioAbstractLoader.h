#ifndef MIO_ABSTRACT_LOADER_HEADER
#define MIO_ABSTRACT_LOADER_HEADER

// MeshIO Copyright © 2019 Andy Maloney <asmaloney@gmail.com>
// SPDX-License-Identifier: BSD-3-Clause

#include "FileIOFilter.h"


class mioAbstractLoader : public FileIOFilter
{
 public:
   bool importSupported() const override;
   bool exportSupported() const override;
   
   CC_FILE_ERROR loadFile( const QString &inFileName, ccHObject &ioContainer, LoadParameters &inParameters ) override;
   
   QStringList getFileFilters( bool onImport ) const override = 0;
   QString getDefaultExtension() const override = 0;
   
   bool canLoadExtension( const QString &inUpperCaseExt ) const override = 0;
   bool canSave( CC_CLASS_ENUM inType, bool &outMultiple, bool &outExclusive ) const override;
};

#endif
