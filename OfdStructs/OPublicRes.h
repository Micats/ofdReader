#pragma once
#include "OOBject.h"
#include <QFont>
#include "YFont.h"

struct FontInfo
{
	QString fontName;
	QString fontFile;
	bool blod;
	bool italic;
	bool load;
	QStringList sysFontNames;
	QString family;
};



class OFont;
class OPublicRes :public OObject
{
public:
	static OPublicRes& instance();
	OPublicRes* parseFromXmlStream(QXmlStreamReader& xmlReader);
	QString tagName() { return "Res"; };
	bool loadFonts(); //加载资源字体库
	QFont getFont(QString id);
	OFont* getOFont(QString id);

	QString getFontInfo(QString fontFile);
	QList<FontInfo> getAllFontInfo();
	void switchFont(QString fontName, QString name);

	QString baseLoc;
	QList<OFont*> fonts;
	
};


class OFont :public OObject
{
public:
	OFont* parseFromXmlStream(QXmlStreamReader& xmlReader);
	QString tagName() { return "Font"; };
	QFont loadSystemFont();
	QFont getFont();  //返回字体，有可能没有加载，所以加一层过滤

	//原结构
	QString fontName;
	QString fontFile;

	//替代字体
	QString familyName;
	bool bold = false;
	bool fixedWidth =false;
	bool italic = false;
	bool serif = false;

	
	//引申结构
	QFont font;
	QFont fontSys;  //系统
	YFont* yfont;
	bool loadSuc = false;
};

