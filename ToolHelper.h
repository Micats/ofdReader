#pragma once
#include <QObject>
#include <QRectf>
#include <QDebug>
#include <QTransform>
#include <QTreeWidget>
#include <QTreeWidgetItem>


/*
	���ù�ʽ����
	1Ӣ�� = 25.4mm

	����ת����
	pixel = mm * dpi / 25.4;
	
	����ת����
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

	//�ַ���תQRect�ṹ
	static QRectF strToRectf(QString str,QString sep = " ");

	static QPointF strToPointf(QString str, QString sep = " ");

	static QColor strToColor(QString str, QString sep = " ");

	//�ַ���תQList<int>�ṹ
	static QList<int> strToIntList(QString str, QString sep = " ");

	//�ַ���תQList<int>�ṹ
	static QList<double> strToDoubleList(QString str, QString sep = " ");

	//��ȡ�ļ�·����
	static QString getFilePath(QString fileName);

	//CTM to QTransform
	static QTransform ctmToTransform(QString ctm);

	//QStirng to bool
	static bool stringToBool(QString str, bool init = false);

	//����ת����
	static double mmToPixel(double val);
	static QSizeF mmToPixel(QSizeF val);
	static QRectF mmToPixel(QRectF val);
	static QPointF mmToPixel(QPointF pos);
	static QLineF mmToPixel(QLineF& pos);

	//�ֺ�ת����
	static double fontToPixel(double point);

	static QTreeWidgetItem* findItem(QTreeWidget* tree,int layer, QString text);

	//�����ϸתQt�����ϸ
	static QFont::Weight toQWeight(int weight);
	static QString toSysFont(QString fontName);  //ofd�е���������ת����ϵͳ�������ƣ���Ȼ���ز���
	static QStringList matchSysFonts(QString fontName);  //�ҳ����������

	static void SetImageAlpha(QImage& img,int val);
};


class GSettings
{
public:

	static GSettings& instance();
	int doc_render_dpi;  //��ʼ�����Զ���ȡ



private:
	GSettings();
};


