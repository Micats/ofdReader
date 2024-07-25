#pragma once

#include "OObject.h"
#include <QPainter>



class OSignedInfo : public OObject
{
public:
	OSignedInfo(OObject* parent = nullptr) :OObject(parent) {}
	~OSignedInfo() {}
	OSignedInfo* parseFromXmlStream(QXmlStreamReader& xmlReader);
	QString tagName() { return "SignedInfo"; }

	struct Provider
	{
		QString providerName;
		QString company;
		QString version;
	};

	struct StampAnnot
	{
		QString id;
		QString boundary;
		QString pageRef;
	};

	struct Reference
	{
		QString fileRef;
		QString checkValue;
	};

	struct References
	{
		QString checkMethod;
		QList<Reference> referenceList;
	};

	QString signatureMethod;
	QString SignatureDateTime;
	Provider provider;
	StampAnnot stampAnnot;
	References references;

};


class OSignature : public OObject
{
public:
	OSignature(OObject* parent = nullptr):OObject(parent){}
	~OSignature(){}
	OSignature* parseFromXmlStream(QXmlStreamReader& xmlReader);
	void parseFromXmlStreamFile(QXmlStreamReader& xmlReader);
	QString tagName() { return "Signature"; }
	bool parseSignedValue();
	QImage getSealImage();
	void paint(QPainter& painter);

	struct Atrributes {
		QString id;		//
		QString type;	//
		QString baseLoc;	//
	};

	Atrributes atrributes;
	QString signedValue;
	OSignedInfo signedInfo;

};

class OSignatures  : public OObject
{
public:
	static OSignatures& instance();
	OSignatures(OObject *parent=nullptr);
	~OSignatures();
	OSignatures* parseFromXmlStream(QXmlStreamReader& xmlReader);
	QString tagName() { return "Signatures"; }
	QList<OSignature*> pageIdSignatures(QString pageId);


	QString maxSignId = "";
	QList<OSignature*> signatures;



};
