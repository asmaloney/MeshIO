#ifndef PLY_HEADER
#define PLY_HEADER

#include "mioAbstractLoader.h"


class PLYFilter : public mioAbstractLoader
{
public:		
	QStringList getFileFilters( bool onImport ) const override;
	QString getDefaultExtension() const override;
	
	bool canLoadExtension( const QString &inUpperCaseExt ) const override;
};

#endif