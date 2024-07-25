#pragma once

#include <QObject>
#include <QPaintDevice>
#include <QWidget>
#include <QImage>
#include "OContent.h"   //绘制page结构
#include "OAnnot.h"
#include "OSignatures.h"

//绘制页到无关设备上，到时候从无关设备绘制到具体设备上
class RenderPage : public QObject
{
	Q_OBJECT

public:
	RenderPage(QObject* parent = nullptr);
	~RenderPage();

	void setPage(OContent* pContent) { m_pContent = pContent; }
	void setDpi(int dpi) { m_dpi = dpi; }
	void setAnnots(OPageAnnot* pageAnnot) { m_pageAnnot = pageAnnot; }
	void setSignatures(QList<OSignature*> pageSignatures) { m_signatures = pageSignatures; }

	QImage paintToImage();
	void paintToWidget(QWidget* widget);

private:
	void paintToDevice(QPaintDevice* device);

private:
	OContent* m_pContent = nullptr;
	OPageAnnot* m_pageAnnot = nullptr;
	QList<OSignature*> m_signatures;
	int m_dpi = 96;
};
