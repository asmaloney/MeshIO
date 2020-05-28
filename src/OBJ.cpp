#include <QDebug>
#include <QString>

#include "OBJ.h"


QStringList OBJFilter::getFileFilters( bool onImport ) const
{
	if ( onImport )
	{
		// import
		return {
			QStringLiteral( "MeshIO - OBJ file (*.obj)" ),
		};
	}
	else
	{
		// export
		return {};
	}
}

QString OBJFilter::getDefaultExtension() const
{
	return QStringLiteral( "obj" );
}

bool OBJFilter::canLoadExtension( const QString &inUpperCaseExt ) const
{
	const QStringList extensions{
		"OBJ",
	};
	
	return extensions.contains( inUpperCaseExt );
}
