#pragma once
// MeshIO Copyright © 2019 Andy Maloney <asmaloney@gmail.com>
// SPDX-License-Identifier: BSD-3-Clause

#include <QRegularExpression>

#include "mioAbstractLoader.h"


class IFCFilter final : public mioAbstractLoader
{
 public:
   IFCFilter();
   
 private:
   void _postProcess( ccHObject &ioContainer ) override;
   
   void _recursiveRename( ccHObject *ioContainer );
   
   QRegularExpression   mNameMatcher;
};
