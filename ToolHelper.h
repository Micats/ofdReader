#pragma once
#include <QObject>
#include <QRectf>
#include <QDebug>
#include <QTransform>
#include <QTreeWidget>
#include <QTreeWidgetItem>


/*
	常用公式计算
	1英寸 = 25.4mm

	毫米转像素
	pixel = mm * dpi / 25.4;
	
	像素转毫米
	mm = pixel *25.4 / dpi
	
*/
//300dpi
#define MM2PIXEL72P(m) (double)(2.8346456692913387*(double)(m))
#define MM2PIXEL(m) (double)(11.811023622047246*(double)(m))
//300/25.4 == 11.81

#define DeletePtr(ptr) if(ptr){delete ptr;ptr = nullptr;}
#define DeleteContainer(container) qDeleteAll(container);container.clear();


#define DEFER_1(x, y) x##y
#define DEFER_2(x, y) DEFER_1(x, y)
#define DEFER_0(x)    DEFER_2(x, __COUNTER__)
#define defer(expr)   auto DEFER_0(_defered_option) = deferer([&](){expr})


template<typename F>
struct DoDefer
{
	F f;
	DoDefer(F f) : f(f) {}
	~DoDefer() { f(); }
};
template <typename F>
DoDefer<F> deferer(F f) {
	return DoDefer<F>(f);
};


class ToolHelper
{
	static QSet<QString> FontWordSet;
public:

	//字符串转QRect结构
	static QRectF strToRectf(QString str,QString sep = " ");

	static QPointF strToPointf(QString str, QString sep = " ");

	static QColor strToColor(QString str, QString sep = " ");

	//字符串转QList<int>结构
	static QList<int> strToIntList(QString str, QString sep = " ");

	//字符串转QList<int>结构
	static QList<double> strToDoubleList(QString str, QString sep = " ");

	//获取文件路径，
	static QString getFilePath(QString fileName);

	//CTM to QTransform
	static QTransform ctmToTransform(QString ctm);

	//QStirng to bool
	static bool stringToBool(QString str, bool init = false);

	//毫米转像素
	static double mmToPixel(double val);
	static QSizeF mmToPixel(QSizeF val);
	static QRectF mmToPixel(QRectF val);
	static QPointF mmToPixel(QPointF pos);
	static QLineF mmToPixel(QLineF& pos);

	//字号转像素
	static double fontToPixel(double point);

	static QTreeWidgetItem* findItem(QTreeWidget* tree,int layer, QString text);

	//字体粗细转Qt字体粗细
	static QFont::Weight toQWeight(int weight);
	static QString toSysFont(QString fontName);  //ofd中的字体名称转换成系统字体名称，不然加载不上
	static QStringList matchSysFonts(QString fontName);  //找出相近的字体

	static void SetImageAlpha(QImage& img,int val);
};


class GSettings
{
public:

	static GSettings& instance();
	int doc_render_dpi;  //初始化会自动获取



private:
	GSettings();
};


