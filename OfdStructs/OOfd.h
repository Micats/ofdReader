#pragma once
#include "OObject.h"

class ODocument;
class OSignatures;
class OOfd :public OObject
{

public:
	OOfd() {}
	~OOfd();

	//文件内容
	QString docId;
	QString author;
	QString creationDate;
	QString modDate;
	QString creator;
	QString creatorVersion;
	QString docRoot;
	QString signatures;


	//解析后内容
	ODocument* document = nullptr;
	OSignatures* signatureObjs = nullptr;

	//文件本身信息
	QString fileName;

public:
	OOfd* parseFromXml(QXmlStreamReader& xmlReader);
	QString tagName() { return "OFD"; };
};

