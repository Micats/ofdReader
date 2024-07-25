#pragma once

#include <QObject>

#include "YOfdXmlParser.h"
#include "YOfdUncompress.h"
#include "OAnnot.h"

class YOfdParser : public QObject
{
	Q_OBJECT

public:
	YOfdParser(QObject *parent = nullptr);
	~YOfdParser();

	bool loadFile(QString fileName);
	//QStringList getFilePaths() { return m_ofdUncompress->getFilePaths(); }


	QSizeF getPhysicalPageSizeF(int pageNum);
	int getTotalPageNum();

	//绘制页到控件上去 page从0开始
	void paintPageToWidget(QWidget* widget, int page);
	QImage paintPageToImage(int page);


	//信息
	QString getFontInfo(QString fontFile);
	
	OPublicRes* getPublicRes() { return pPublicRes; }

private:
	QString m_fileName;
	//YOfdUncompress* m_ofdUncompress = nullptr;
	
	YOfdXmlParser* m_xmlParser = nullptr;
	OOfd* pOofd = nullptr;
	ODocument* pDocument = nullptr;
	OPublicRes* pPublicRes = nullptr;
	ODocumentRes* pDocumentRes = nullptr;
	OAnnotations* pAnnots = nullptr;
	OSignatures* pSignatures = nullptr;

};
