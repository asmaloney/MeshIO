#pragma once
// MeshIO Copyright © 2019 Andy Maloney <asmaloney@gmail.com>
// SPDX-License-Identifier: BSD-3-Clause

#include "FileIOFilter.h"

class mioAbstractLoader : public FileIOFilter
{
public:
   bool canSave( CC_CLASS_ENUM inType, bool &outMultiple, bool &outExclusive ) const override;

   CC_FILE_ERROR loadFile( const QString &inFileName, ccHObject &ioContainer, LoadParameters &inParameters ) override;

protected:
   explicit mioAbstractLoader( const FileIOFilter::FilterInfo &info );

   // Override this to do additional postprocessing on the data.
   // e.g. object renaming or cleaning up the object hierarchy.
   virtual void _postProcess( ccHObject &ioContainer );
};
