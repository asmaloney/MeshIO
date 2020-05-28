#ifndef OBJ_HEADER
#define OBJ_HEADER

#include "mioAbstractLoader.h"


class OBJFilter : public mioAbstractLoader
{
public:		
	QStringList getFileFilters( bool onImport ) const override;
	QString getDefaultExtension() const override;
	
	bool canLoadExtension( const QString &inUpperCaseExt ) const override;
};

#endif