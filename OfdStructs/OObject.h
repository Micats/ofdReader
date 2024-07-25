#pragma once
#include <QObject>
#include <QXmlStreamReader>
#include <QRectF>
#include <QDebug>

#include "ToolHelper.h"

#define DECLARE_OOBECT(type) \
	O##type(OObject* parent = nullptr) :OObject(parent) {} \
	~O##type() {}\
	QString tagName() { return #type; }; \
	O##type* parseFromXml(QXmlStreamReader& xmlReader)


class OPublicRes;
class ODocumentRes;
class OObject
{
	Q_DISABLE_COPY(OObject)

public:
	//OObject(){}
	OObject(OObject* parent = nullptr) { m_parent = parent; }
	virtual ~OObject(){}

	virtual QString tagName() = 0;


	template<class T>
	T* getParent() { return (T*)m_parent; }

	template<class T>
	T* getParents(QString typeName) { 
		auto pParent = m_parent;
		while (pParent)
		{
			if (pParent->tagName() == typeName)
				return (T*)pParent;
			pParent = pParent->m_parent;
		}
		return nullptr; }

	QString id = "-1";
	QString loc = "";	//文件中本来的路径
	QString zipLoc = ""; //解压包中的路径

	OObject* m_parent = nullptr;
	//需要几个指针 指向顶层或者父指针

	//公共资源
	OPublicRes* pPublicRes = nullptr;
	ODocumentRes* pDocumentRes = nullptr;

};

