#pragma once
#include "OOBject.h"
#include <QPainterPath>
#include <QRawFont>

class OLayer;
class OTextObject;
class OPathObject;
class OImageObject;
class OPath;
class OCGTransform;
class OTextCode;
class OFillColor;
class OStrokeColor;
class OGraphicUnitObject;
class OAbbreviatedData;
class OAxialShd;
class OClip;
class OClips;


class OContent :public OObject
{
public:
	~OContent();
	QRectF physicalBox;
	QRectF cropBox;

	//内容
	QList<OLayer*> layers; //分层
public:
	OContent* parseFromXmlStream(QXmlStreamReader& xmlReader);
	QString tagName() { return "Page"; };
};


class OLayer :public OObject
{
public:
	OLayer(OObject* parent ) :OObject(parent) {}
	~OLayer() {}
	OLayer* parseFromXmlStream(QXmlStreamReader& xmlReader);
	QString tagName() { return "Layer"; };

	void paintTo(QPainter& painter);  //按类型绘制  --实际是按照顺序绘制的

	QList<OTextObject*> textObjects;
	QList<OPathObject*> pathObjects;
	QList<OImageObject*> imageObjects;

	QList<OObject*> list;

};

//文字对象 
//--内部对象: OCGTransform OFillColor OStrokeColor
class OTextObject :public OObject
{

public:
	//通用
	QTransform ctm; //矩阵

	QRectF boundary; //边界
	QString font;	//引用资源文件中定义的字型的标识
	double size; //大小 mm
	double miterLimit = 2; //保持当前的斜接限制比率 两条直线  默认值是2
	bool stroke = false; //是否填充
	bool fill = false; //是否填充
	double hscale = 1.0; //水平方向放缩比
	int readDirection = 0; //文字排列方向 0 90 180 270
	int charDirection = 0;//文字放置方向
	int weight = 400;//枚举 100,200,300...900
	bool italic = false; //是否斜体
	qreal lineWidth = 1; //字宽
	OFillColor* fillColor = nullptr;
	OStrokeColor* strokeColor = nullptr;

	OCGTransform* cgtransform =nullptr;
	OTextCode* textcode = nullptr;

public:
	OTextObject(OObject* parent) :OObject(parent) {  }
	void parseFromXmlStream(QXmlStreamReader& xmlReader);
	QString tagName() { return "TextObject"; };
	void paintTo(QPainter& painter);

};


//字形索引表，用来描述字符编码到字形索引的变换关系
class OCGTransform :public OObject
{
public:
	OCGTransform(OObject* parent) :OObject(parent) {}
	OCGTransform* parseFromXmlStream(QXmlStreamReader& xmlReader);
	QString tagName() { return "CGTransform"; };

	int codePosition = -1;  //字位置
	int codeCount = -1;		//字个数
	int glyphCount = -1;
	QList<int> glyphs;
};


//文字块
//其坐标计算方式为boundary的坐标 + TextCode偏移坐标*仿射变换
class OTextCode :public OObject
{

public:
	//一整个字的信息
	struct XYInfoCharacter
	{
		float xOff;
		float yOff;
		QString character; //只盛放一个字
		int glyph = -1; //字形索引
	};
	OTextCode(OObject* parent );
	OTextCode* parseFromXmlStream(QXmlStreamReader& xmlReader);
	QString tagName() { return "TextCode"; };
	QList<XYInfoCharacter> parseXY();

	float x = -1;
	float y = -1;
	QList<double> deltaX;  //数组
	QList<double> deltaY;	//数组
	QString content;

private:
	QRectF boundary;
};



class OPathObject :public OObject
{
	Q_DISABLE_COPY(OPathObject)
public:
	enum RULE
	{
		NONZERO, EVEN_ODD
	};
	

	OPathObject(OObject* parent) :OObject(parent) {  }
	~OPathObject() {
		delete graphicUnit;
		delete fillColor;
		delete strokeColor;
		qDeleteAll(abbreviatedDatas);
	};

	OPathObject* parseFromXmlStream(QXmlStreamReader& xmlReader) ;
	QString tagName() { return "PathObject"; };
	void paintTo(QPainter& painter);
	QPolygonF polygonf();
	QPainterPath path();

	OGraphicUnitObject* graphicUnit;

	
	bool stroke = false;
	bool fill = false;
	bool visible = true;
	QString rule = "";
	//RULE rule = NONZERO;
	OClips* clips = nullptr;
	OFillColor* fillColor = nullptr;
	OStrokeColor* strokeColor = nullptr;
	QList<OAbbreviatedData*> abbreviatedDatas;
	
};

class OPath :public OPathObject
{
public:
	OPath(OObject* parent) :OPathObject(parent) {}

	OPath* parseFromXmlStream(QXmlStreamReader& xmlReader);
	QString tagName() { return "Path"; };
};

class OImageObject :public OObject
{
public:
	OImageObject(OObject* parent) :OObject(parent) {}
	OImageObject* parseFromXmlStream(QXmlStreamReader& xmlReader);
	QString tagName() { return "ImageObject"; };

	void paintTo(QPainter& painter);


	QTransform ctm; //矩阵
	QRectF boundary; //边界

	int resourceID = -1;
};


class OFillColor :public OObject
{
public:
	OFillColor(OObject* parent) :OObject(parent) {}
	OFillColor() {}
	OFillColor* parseFromXmlStream(QXmlStreamReader& xmlReader);
	QString tagName() { return "FillColor"; };
	bool isNull() { return value == "-1 -1 -1" && axialShd == nullptr; }
	bool isShd() { return axialShd != nullptr; };
	QColor color();
	QLinearGradient getShdColor();
	void setPainterColor(QPainter& painter);

	QString value = "-1 -1 -1";

	OAxialShd* axialShd = nullptr;

};

class OStrokeColor :public OObject
{
public:
	OStrokeColor(OObject* parent) :OObject(parent) {}
	OStrokeColor* parseFromXmlStream(QXmlStreamReader& xmlReader);
	QString tagName() { return "StrokeColor"; };
	bool isNull() { return value == "-1 -1 -1"; }
	QColor color();

	QString value = "-1 -1 -1";
};


class OAbbreviatedData :public OObject
{
	/*
		S start 起始坐标
		M move 移动到点，并不连线
		L line 划线，并将当前点移到指定点
		Q 二次贝赛尔曲线并Move此点 两组点，前一组点作为控制点
		B 三次贝塞尔曲线，pos1 pos2作为控制点
		A 圆弧
		C 自动闭合
	*/


public:
	OAbbreviatedData(OObject* parent) :OObject(parent) {}
	OAbbreviatedData* parseFromXmlStream(QXmlStreamReader& xmlReader);
	QString tagName() { return "AbbreviatedData"; };

	QString content;

	QPainterPath drawPath();

};

//属性集合
class OGraphicUnitObject :public OObject
{
	Q_DISABLE_COPY(OGraphicUnitObject)
public:

	enum CAP
	{
		BUTT, ROUND, SQUARE
	};
	enum JOIN
	{
		MITER, _ROUND, BEVEL
	};

	OGraphicUnitObject(OObject* parent) :OObject(parent) {}
	~OGraphicUnitObject() {}

	OGraphicUnitObject* parseFromXmlStream(QXmlStreamReader& xmlReader);
	QString tagName() { return "Attibute-inner"; };
	//void paintTo(QPainter& painter);

	QString name;
	QTransform ctm; //矩阵
	QRectF boundary; //边界
	bool visible = true;
	double lineWidth = 0.353;
	double miterLimit = 4.234;
	QString cap; //默认 BUTT
	QString join;//默认 MITER;
	double DashOffset = 0.0;
	int alpha = 255;
	QString dashPattern;

	//std::vector<CT_Action*> actions;
	//std::vector<CT_Clip*> clips;
	//ST_RefID drawParam;
};


//轴向渐变
class OAxialShd :public OObject
{
public:
	OAxialShd(OObject* parent=nullptr):OObject(parent){}
	~OAxialShd(){}

	OAxialShd* parseFromXmlStream(QXmlStreamReader& xmlReader);
	QString tagName() { return "AxialShd"; };

	struct Atrributes
	{
		QString mapType; //绘制渐变方式 Direct Repeat Reflect
		double mapUnit;		//渐变区间长度 默认轴线长度
		int extent;			//轴线延长线方向是否继续绘制渐变 0：两段不 1：结束->起始点方向绘制 2：起始->结束点方向绘制 3：两侧都绘制
		QPointF startPoint;
		QPointF endPoint;
	};

	//颜色段，至少出现俩
	struct segment  
	{
		double position=-1; //颜色位置 [0.0,1.0]
		QColor color;	//颜色值
	};

	Atrributes atrribute;
	QList<segment> segments;

};


class OClip :public OObject
{
public:
	OClip(OObject* parent = nullptr) :OObject(parent) {}
	~OClip() {}

	OClip* parseFromXmlStream(QXmlStreamReader& xmlReader);
	QString tagName() { return "Clip"; };
	QPolygonF polygonf();
	QPainterPath path();

	struct Area :public OObject
	{
		Area(OObject* parent = nullptr) :OObject(parent) {}
		Area* parseFromXmlStream(QXmlStreamReader& xmlReader);
		QString tagName() { return "Area"; };

		QString drawParam;
		QTransform ctm;

		OPath* path = nullptr;
		OTextObject* text = nullptr;
	};

	QList<Area*> areas;
};


class OClips:public OObject
{
public:
	OClips(OObject* parent = nullptr) :OObject(parent) {}
	~OClips() {}

	OClips* parseFromXmlStream(QXmlStreamReader& xmlReader);
	QString tagName() { return "Clips"; };
	QPolygonF polygonf();
	QPainterPath path();

	bool transFlag = false;
	QList<OClip*> clips;
};

