#pragma execution_character_set("utf-8")
#include "Items.h"


FontItem::FontItem(QWidget *parent)
	: QWidget(parent)
{

	/*lab = new QLabel("字体:" + infos[i].fontName);
	lab2 = new QLabel("  位置:" + infos[i].fontFile);
	lab3 = new QLabel("  使用:" + infos[i].family);*/
	lab = new QLabel("字体:");
	lab2 = new QLabel("  位置:");
	lab3 = new QLabel("  使用:");
	lab4 = new QLabel("  加载:");
	fontBox = new QComboBox;
	fontBox->setFixedWidth(150);
	fontBox->addItem("");
	btn = new QPushButton("切换");
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
	lab ->setText("字体:" + info.fontName);
	lab2->setText("  位置:" + info.fontFile);
	lab3->setText("  使用:" + info.family);
	lab4->setText("  加载:" + QString::number(info.load));

	fontBox->clear();
	fontBox->addItems(info.sysFontNames);
}

void FontItem::on_btn_clicked()
{
	QString name = fontBox->currentText();
	QString fontName = lab->text().replace("字体:","");
	emit sigSwitchFont(fontName, name);
	lab3->setText("  使用:" + name);
}
