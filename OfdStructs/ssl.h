#pragma once

#include <openssl/asn1.h>

typedef struct cert_digest_obj_st
{
	ASN1_PRINTABLESTRING* type;
	ASN1_OCTET_STRING* value;
}CertDigestObj;
DECLARE_ASN1_FUNCTIONS(CertDigestObj)

typedef struct ses_certlist_st {
	//# define GEN_CERTINFOLIST			0
	//# define GEN_CERTDIGESTLIST			1
	//int type;
	//	union {
	//char *ptr;
	STACK_OF(ASN1_OCTET_STRING)* certs;
	//		STACK_OF(CertDigestObj) *certDigestList;
	//	}d;
} SES_CertList;
DEFINE_STACK_OF(ASN1_OCTET_STRING)
DECLARE_ASN1_FUNCTIONS(SES_CertList)

typedef struct ses_espropertyinfo_st {
	ASN1_INTEGER* type;
	ASN1_UTF8STRING* name;
	STACK_OF(ASN1_OCTET_STRING)* certList;
	ASN1_UTCTIME* createDate;
	ASN1_UTCTIME* validStart;
	ASN1_UTCTIME* validEnd;
} SES_ESPropertyInfo;
DECLARE_ASN1_FUNCTIONS(SES_ESPropertyInfo)

typedef struct v4_ses_espropertyinfo_st {
	ASN1_INTEGER* type;
	ASN1_UTF8STRING* name;
	ASN1_INTEGER* certListType;
	SES_CertList* certList;
	ASN1_GENERALIZEDTIME* createDate;
	ASN1_GENERALIZEDTIME* validStart;
	ASN1_GENERALIZEDTIME* validEnd;
} V4_SES_ESPropertyInfo;
DECLARE_ASN1_FUNCTIONS(V4_SES_ESPropertyInfo)

typedef struct ses_espictrueinfo_st {
	ASN1_IA5STRING* type;
	ASN1_OCTET_STRING* data;
	ASN1_INTEGER* width;
	ASN1_INTEGER* height;
} SES_ESPictrueInfo;
DECLARE_ASN1_FUNCTIONS(SES_ESPictrueInfo)

typedef struct v4_ses_espictrueinfo_st {
	ASN1_IA5STRING* type;
	ASN1_OCTET_STRING* data;
	ASN1_INTEGER* width;
	ASN1_INTEGER* height;
} V4_SES_ESPictrueInfo;
DECLARE_ASN1_FUNCTIONS(V4_SES_ESPictrueInfo)

typedef struct ses_header_st {
	ASN1_IA5STRING* ID;
	ASN1_INTEGER* version;
	ASN1_IA5STRING* Vid;
} SES_Header;
DECLARE_ASN1_FUNCTIONS(SES_Header)

typedef struct v4_ses_header_st {
	ASN1_IA5STRING* ID;
	ASN1_INTEGER* version;
	ASN1_IA5STRING* Vid;
} V4_SES_Header;
DECLARE_ASN1_FUNCTIONS(V4_SES_Header)

typedef struct extdata_st {
	ASN1_OBJECT* extnID;
	ASN1_BOOLEAN* critical;
	ASN1_OCTET_STRING* extnValue;
} ExtData;
DECLARE_ASN1_FUNCTIONS(ExtData)

typedef struct v4_extdata_st {
	ASN1_OBJECT* extnID;
	ASN1_BOOLEAN* critical;
	ASN1_OCTET_STRING* extnValue;
} V4ExtData;
DECLARE_ASN1_FUNCTIONS(V4ExtData)

typedef struct ses_sealinfo_st {
	SES_Header* header;
	ASN1_IA5STRING* esID;
	SES_ESPropertyInfo* property;
	SES_ESPictrueInfo* picture;
	STACK_OF(ExtData)* extDatas;
} SES_SealInfo;
DECLARE_ASN1_FUNCTIONS(SES_SealInfo)

typedef struct v4_ses_sealinfo_st {
	V4_SES_Header* header;
	ASN1_IA5STRING* esID;
	V4_SES_ESPropertyInfo* property;
	V4_SES_ESPictrueInfo* picture;
	STACK_OF(V4ExtData)* extDatas;
} V4_SES_SealInfo;
DECLARE_ASN1_FUNCTIONS(V4_SES_SealInfo)

typedef struct ses_signinfo_st
{
	ASN1_OCTET_STRING* cert;
	ASN1_OBJECT* signatureAlorithm;
	ASN1_BIT_STRING* signData;
} SES_SignInfo;
DECLARE_ASN1_FUNCTIONS(SES_SignInfo)

typedef struct seseal_st
{
	SES_SealInfo* esealInfo;
	SES_SignInfo* signInfo;
} SESeal;
DECLARE_ASN1_FUNCTIONS(SESeal)

typedef struct v4_seseal_st
{
	V4_SES_SealInfo* esealInfo;
	ASN1_OCTET_STRING* cert;
	ASN1_OBJECT* signAlgID;
	ASN1_BIT_STRING* signedValue;
} V4SESeal;
DECLARE_ASN1_FUNCTIONS(V4SESeal)

typedef struct tbssign_st
{
	ASN1_INTEGER* version;
	SESeal* eseal;
	ASN1_BIT_STRING* timeinfo;
	ASN1_BIT_STRING* dataHash;
	ASN1_IA5STRING* propertyInfo;
	ASN1_OCTET_STRING* cert;
	ASN1_OBJECT* signatureAlgorithm;
}TBS_Sign;
DECLARE_ASN1_FUNCTIONS(TBS_Sign)

typedef struct v4_tbssign_st
{
	ASN1_INTEGER* version;
	V4SESeal* eseal;
	ASN1_GENERALIZEDTIME* timeinfo;
	ASN1_BIT_STRING* dataHash;
	ASN1_IA5STRING* propertyInfo;
	STACK_OF(V4ExtData)* extDatas;
} V4_TBS_Sign;
DECLARE_ASN1_FUNCTIONS(V4_TBS_Sign)

typedef struct sessignature_st
{
	TBS_Sign* toSign;
	ASN1_BIT_STRING* signature;
} SES_Signature;
DECLARE_ASN1_FUNCTIONS(SES_Signature)

typedef struct v4_sessignature_st
{
	V4_TBS_Sign* toSign;
	ASN1_OCTET_STRING* cert;
	ASN1_OBJECT* signatureAlgID;
	ASN1_BIT_STRING* signature;
	ASN1_BIT_STRING* timeStamp;
} V4_SES_Signature;
DECLARE_ASN1_FUNCTIONS(V4_SES_Signature)

