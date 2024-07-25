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

	//����
	QList<OLayer*> layers; //�ֲ�
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

	void paintTo(QPainter& painter);  //�����ͻ���  --ʵ���ǰ���˳����Ƶ�

	QList<OTextObject*> textObjects;
	QList<OPathObject*> pathObjects;
	QList<OImageObject*> imageObjects;

	QList<OObject*> list;

};

//���ֶ��� 
//--�ڲ�����: OCGTransform OFillColor OStrokeColor
class OTextObject :public OObject
{

public:
	//ͨ��
	QTransform ctm; //����

	QRectF boundary; //�߽�
	QString font;	//������Դ�ļ��ж�������͵ı�ʶ
	double size; //��С mm
	double miterLimit = 2; //���ֵ�ǰ��б�����Ʊ��� ����ֱ��  Ĭ��ֵ��2
	bool stroke = false; //�Ƿ����
	bool fill = false; //�Ƿ����
	double hscale = 1.0; //ˮƽ���������
	int readDirection = 0; //�������з��� 0 90 180 270
	int charDirection = 0;//���ַ��÷���
	int weight = 400;//ö�� 100,200,300...900
	bool italic = false; //�Ƿ�б��
	qreal lineWidth = 1; //�ֿ�
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


//�������������������ַ����뵽���������ı任��ϵ
class OCGTransform :public OObject
{
public:
	OCGTransform(OObject* parent) :OObject(parent) {}
	OCGTransform* parseFromXmlStream(QXmlStreamReader& xmlReader);
	QString tagName() { return "CGTransform"; };

	int codePosition = -1;  //��λ��
	int codeCount = -1;		//�ָ���
	int glyphCount = -1;
	QList<int> glyphs;
};


//���ֿ�
//��������㷽ʽΪboundary������ + TextCodeƫ������*����任
class OTextCode :public OObject
{

public:
	//һ�����ֵ���Ϣ
	struct XYInfoCharacter
	{
		float xOff;
		float yOff;
		QString character; //ֻʢ��һ����
		int glyph = -1; //��������
	};
	OTextCode(OObject* parent );
	OTextCode* parseFromXmlStream(QXmlStreamReader& xmlReader);
	QString tagName() { return "TextCode"; };
	QList<XYInfoCharacter> parseXY();

	float x = -1;
	float y = -1;
	QList<double> deltaX;  //����
	QList<double> deltaY;	//����
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


	QTransform ctm; //����
	QRectF boundary; //�߽�

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
		S start ��ʼ����
		M move �ƶ����㣬��������
		L line ���ߣ�������ǰ���Ƶ�ָ����
		Q ���α��������߲�Move�˵� ����㣬ǰһ�����Ϊ���Ƶ�
		B ���α��������ߣ�pos1 pos2��Ϊ���Ƶ�
		A Բ��
		C �Զ��պ�
	*/


public:
	OAbbreviatedData(OObject* parent) :OObject(parent) {}
	OAbbreviatedData* parseFromXmlStream(QXmlStreamReader& xmlReader);
	QString tagName() { return "AbbreviatedData"; };

	QString content;

	QPainterPath drawPath();

};

//���Լ���
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
	QTransform ctm; //����
	QRectF boundary; //�߽�
	bool visible = true;
	double lineWidth = 0.353;
	double miterLimit = 4.234;
	QString cap; //Ĭ�� BUTT
	QString join;//Ĭ�� MITER;
	double DashOffset = 0.0;
	int alpha = 255;
	QString dashPattern;

	//std::vector<CT_Action*> actions;
	//std::vector<CT_Clip*> clips;
	//ST_RefID drawParam;
};


//���򽥱�
class OAxialShd :public OObject
{
public:
	OAxialShd(OObject* parent=nullptr):OObject(parent){}
	~OAxialShd(){}

	OAxialShd* parseFromXmlStream(QXmlStreamReader& xmlReader);
	QString tagName() { return "AxialShd"; };

	struct Atrributes
	{
		QString mapType; //���ƽ��䷽ʽ Direct Repeat Reflect
		double mapUnit;		//�������䳤�� Ĭ�����߳���
		int extent;			//�����ӳ��߷����Ƿ�������ƽ��� 0�����β� 1������->��ʼ�㷽����� 2����ʼ->�����㷽����� 3�����඼����
		QPointF startPoint;
		QPointF endPoint;
	};

	//��ɫ�Σ����ٳ�����
	struct segment  
	{
		double position=-1; //��ɫλ�� [0.0,1.0]
		QColor color;	//��ɫֵ
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

