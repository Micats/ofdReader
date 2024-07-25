#pragma once
#include "OOBject.h"
#include <QImage>

class OMultiMedia;
class ODocumentRes :public OObject
{
public:
	ODocumentRes* parseFromXmlStream(QXmlStreamReader& xmlReader);
	QString tagName() { return "Res"; };
	QImage getImage(QString id);
	static ODocumentRes& instance();

	QString baseLoc;
	QList<OMultiMedia*> multiMedias;

private:
	void loadRes();
};


class OMultiMedia :public OObject
{
public:
	OMultiMedia* parseFromXmlStream(QXmlStreamReader& xmlReader);
	QString tagName() { return "MultiMedia"; };

	QString type;
	QString mediaFile;

	QImage img;
};