#pragma execution_character_set("utf-8")
#include "Items.h"


FontItem::FontItem(QWidget *parent)
	: QWidget(parent)
{

	/*lab = new QLabel("����:" + infos[i].fontName);
	lab2 = new QLabel("  λ��:" + infos[i].fontFile);
	lab3 = new QLabel("  ʹ��:" + infos[i].family);*/
	lab = new QLabel("����:");
	lab2 = new QLabel("  λ��:");
	lab3 = new QLabel("  ʹ��:");
	lab4 = new QLabel("  ����:");
	fontBox = new QComboBox;
	fontBox->setFixedWidth(150);
	fontBox->addItem("");
	btn = new QPushButton("�л�");
	connect(btn, &QPushButton::clicked, this, &FontItem::on_btn_clicked);
	btn->setFixedWidth(60);
	hlayout = new QHBoxLayout;
	hlayout->addWidget(fontBox);
	hlayout->addWidget(btn);
	hlayout->setMargin(10);
	vlayout = new QVBoxLayout;
	vlayout->addWidget(lab);
	vlayout->addWidget(lab2);
	vlayout->addWidget(lab3);
	vlayout->addWidget(lab4);
	vlayout->addItem(hlayout);
	this->setLayout(vlayout);
	this->setFixedWidth(280);
}

FontItem::~FontItem()
{}

void FontItem::setFontInfo(FontInfo info)
{
	lab ->setText("����:" + info.fontName);
	lab2->setText("  λ��:" + info.fontFile);
	lab3->setText("  ʹ��:" + info.family);
	lab4->setText("  ����:" + QString::number(info.load));

	fontBox->clear();
	fontBox->addItems(info.sysFontNames);
}

void FontItem::on_btn_clicked()
{
	QString name = fontBox->currentText();
	QString fontName = lab->text().replace("����:","");
	emit sigSwitchFont(fontName, name);
	lab3->setText("  ʹ��:" + name);
}
