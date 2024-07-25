

/*
	页的大小信息
	资源文件
	页个数及路径信息
	页的打开模式
*/


#pragma once
#include "OObject.h"
#include "OPublicRes.h"
#include "ODocumentRes.h"
#include "OAnnot.h"


class OContent;
class OAction;
class OActions;
class OGoto;
class OGotoA;
class OURI;
class OSound;
class OMovie;
class ODest;
class OBookmark;

class ODocument :public OObject
{
public:
	ODocument(OObject* parent = nullptr):OObject(parent){}
	~ODocument();
	//CommonData
	qint64 maxUnitID;
	QRectF physicalBox;
	QString publicRes;
	QString documentRes;
	//page
	QList<OContent*> pages;

	//VPreferences
	QString TabDisplay;
	bool hideToolbar;
	bool hideMenubar;
	bool hideWindowUI;
	double zoomMode;
	double zoom;

	//延伸
	OPublicRes* pPublicRes = nullptr;
	ODocumentRes* pDocumentRes = nullptr;
	OAnnotations* pAnnots = nullptr;

public:
	ODocument* parseFromXml(QXmlStreamReader& xmlReader);
	QString tagName() { return "Document"; };
};


class ODest :public OObject
{
public:
	DECLARE_OOBECT(Dest);

	QString type;
	QString pageID;
	double left = 0.0;
	double top = 0.0;
	double right = 0.0;
	double bottom = 0.0;
	double zoom = 1.0;
};

class OBookmark :public OObject
{
public:
	DECLARE_OOBECT(Bookmark);
	QString name;
	ODest* dest;
};

class OGoto :public OObject
{
public:
	DECLARE_OOBECT(Goto);

	ODest* dest = nullptr;
	OBookmark* bookmark = nullptr;
};

class OGotoA :public OObject
{
public:
	DECLARE_OOBECT(GotoA);

	QString attachID;
	bool newWindow = false;
};

class OURI :public OObject
{
public:
	DECLARE_OOBECT(URI);

	QString uri;
	QString base;

};

class OSound :public OObject
{
public:
	DECLARE_OOBECT(Sound);

	QString resourceID;
	int volume;
	bool repeat;
	bool synchronous;
};

class OMovie :public OObject
{
public:
	DECLARE_OOBECT(Movie);

	QString resourceID;
	QString operator_;
};


class OAction :public OObject
{
public:
	DECLARE_OOBECT(Action);

	QString event;
	//ORegion region;
	OGoto* pGoto = nullptr;
	OURI* pUri = nullptr;
	OGotoA* pGotoA = nullptr;
	OSound* pSound = nullptr;
	OMovie* pMovie = nullptr;
};



class OActions :public OObject
{
public:
	DECLARE_OOBECT(Actions);

	QList<OAction*> actions;

};

class OOutlineElem :public OObject
{
public:
	DECLARE_OOBECT(OutlineElem);

	struct Attributes
	{
		QString title;
		int count;
		bool expanded = true;
	};

	Attributes attribute;
	OActions* actions = nullptr;
	QList<OOutlineElem*> children;

};

