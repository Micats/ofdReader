#pragma once


#include "freetype/freetype.h"
#include "freetype/ftoutln.h"
#include "freetype/ftglyph.h"

#include <QObject>
#include <QColor>
#include <QPainter>
#include <QList>


class YFont 
{
public:
	YFont(FT_Library* lib);
	~YFont();

	bool load(QString fontPath);
	bool loadFromData(QByteArray bytes);
	void setFontSize(float size); //pixel size
	void setColor(QColor color);

	void drawText(QPainter& painter, float x, float y, const QString& text );
	void drawText(QPainter& painter, float x, float y, float fontSize,QColor fontColor, const QString& text);
	FT_Face* ft_face() { return &m_face; }
	char* face_name() { return m_face->family_name; }
	void test(QString text);
	void test2(QTransform& mtx,QString text);

	void setTransform(QTransform& mtx);
private:

	FT_Library* m_library;
	QString m_fontPath;
	FT_Face m_face;
	bool m_bLoad;
	QColor m_fontColor;
	QTransform m_mtx;
	float m_fontSize;

	


};


class YFontManager
{
public:
	~YFontManager();

	static YFontManager& Instance();

	YFont* addFont(QByteArray fontBytes);
	YFont* addFontPath(QString path);

private:
	YFontManager();

	FT_Library m_library;
	QList<YFont*> m_fontList;

	bool m_bInit;
};

