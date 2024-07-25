#pragma once

#include <QObject>
#include <QDomDocument>
#include <QXmlStreamReader>

#include "YOfdUncompress.h"
#include "OObject.h"
#include "OOfd.h"
#include "ODocument.h"
#include "OContent.h"
#include "OPublicRes.h"
#include "ODocumentRes.h"
#include "OAnnot.h"
#include "OSignatures.h"

class YOfdXmlParser : public QObject
{
	Q_OBJECT

public:
	YOfdXmlParser(QObject *parent = nullptr);
	~YOfdXmlParser();

	//解析ofd.xml
	OOfd* parseOFDXml(QString fileZipPath = "OFD.xml");

	//document
	ODocument* parseDocumentXml(ODocumentRes* pDocumentRes,OPublicRes* pPublicRes, QString fileZipPath = "Doc_0/Document.xml");

	//content
	OContent* parseContentXml(QString fileZipPath,OPublicRes* pPublicRes, ODocumentRes* pDocumentRes);

	//公共资源，字体
	OPublicRes* parsePublicResXml(QString fileZipPath = "Doc_0/PublicRes.xml");

	//页面资源 图片
	ODocumentRes* parseDocumentResXml(QString fileZipPath = "Doc_0/DocumentRes.xml");

	//注释
	OAnnotations* parseAnnotatiosXml(QString fileZipPath = "Doc_0/Annots/Annotations.xml");

	//签名
	OSignatures* parseSignaturesXml(QString fileZipPath = "Doc_0/Signs/Signatures.xml");
private:

	//流解析
	QXmlStreamReader m_xmlReader;
};
