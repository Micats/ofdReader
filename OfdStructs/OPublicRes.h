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
	bool loadFonts(); //������Դ�����
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
	QFont getFont();  //�������壬�п���û�м��أ����Լ�һ�����

	//ԭ�ṹ
	QString fontName;
	QString fontFile;

	//�������
	QString familyName;
	bool bold = false;
	bool fixedWidth =false;
	bool italic = false;
	bool serif = false;

	
	//����ṹ
	QFont font;
	QFont fontSys;  //ϵͳ
	YFont* yfont;
	bool loadSuc = false;
};

