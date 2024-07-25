#pragma once


#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QBoxLayout>
#include <QString>
#include "OPublicRes.h"

class FontItem  : public QWidget
{
	Q_OBJECT

public:
	FontItem(QWidget *parent = nullptr);
	~FontItem();

	void setFontInfo(FontInfo info);

public slots:
	void on_btn_clicked();

signals:
	void sigSwitchFont(QString, QString);

private:
	QLabel* lab;
	QLabel* lab2;
	QLabel* lab3;
	QLabel* lab4;
	QComboBox* fontBox;
	QPushButton* btn;
	QHBoxLayout* hlayout;
	QVBoxLayout* vlayout;
};
