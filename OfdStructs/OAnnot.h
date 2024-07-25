#pragma once

#include <QObject>
#include "OObject.h"
#include "OContent.h"

class OAnnot :public OObject
{

public:
	OAnnot(OObject *parent=nullptr):OObject(parent){}
	~OAnnot(){}

	OAnnot* parseFromXmlStream(QXmlStreamReader& xmlReader);
	QString tagName() { return "Annot"; };
	void paint(QPainter& painter);


	struct Atrributes {
		QString id;		//required
		QString type;	//required enum:Link Path Highlight Stamp Watermark
		QString creator;	//required
		QString lastModDate; //xs:date required
		bool visible = true ;
		QString subtype;
		bool print = true;
		bool noZoom = false;
		bool noRotate =false ;
		bool readOnly =true;
	};

	struct Paramter {
		QString name;
		QString value;
	};

	struct Appearance:public OObject {

		QRectF boundary;

		//pageblock
		QList<OTextObject*> textObjects;
		QList<OPathObject*> pathObjects;
		QList<OImageObject*> imageObjects;

		Appearance(OObject* parent=nullptr):OObject(parent){}
		Appearance* parseFromXmlStream(QXmlStreamReader& xmlReader);
		QString tagName() { return "Appearance"; }
	};

	Atrributes  attributes;
	QStringList remarks;  //¿ÉÑ¡×¢ÊÍ
	QList<Paramter*> parameters; 
	QList<Appearance*> appearances;
	
};


class OPageAnnot :public OObject
{

public:
	OPageAnnot(OObject* parent = nullptr) :OObject(parent) {}
	~OPageAnnot() {}

	OPageAnnot* parseFromXmlStream(QXmlStreamReader& xmlReader);
	QString tagName() { return "PageAnnot"; };
	void paint(QPainter& painter);

	QList<OAnnot*> annots;
};

class OAnnotations:public OObject
{

public:
	OAnnotations(OObject*parent = nullptr) :OObject(parent){}
	~OAnnotations(){}
	OAnnotations* parseFromXmlStream(QXmlStreamReader& xmlReader);
	QString tagName() { return "Annotations"; }

	struct Page {
		QString pageId;
		QString fileLoc;
		OPageAnnot pageAnnot;
	};

	QList<Page*> annots;

};

