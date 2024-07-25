#pragma once

#include <QMainWindow>
#include "ui_OfdExplore.h"
#include "YOfdParser.h"
#include <QTreeWidgetItem>
#include <QDragEnterEvent>
#include <QDropEvent>

class OfdExplore : public QMainWindow
{
	Q_OBJECT

public:
	OfdExplore(QWidget *parent = nullptr);
	~OfdExplore();
	void initCtrls();
	void createTreeItem(QStringList paths);
	void closeFile();
	void openFile(QString fileName);
	QString formatXml(QString xml);
public slots:
	void on_actionOpen_triggered();  //打开文件
	void on_actionClose_triggered();  //关闭文件
	void on_actionFind_triggered();  //关闭文件
	void on_treeWidgetDoc_itemClicked(QTreeWidgetItem* item, int col); //目录树项切换

	void on_pushButtonPre_clicked();
	void on_pushButtonNext_clicked();
	void on_pushButtonFindClose_clicked();

	void on_switchFont(QString fontName, QString newFontName);

protected:
	//文件拖入
	void dragEnterEvent(QDragEnterEvent* event) override;
	void dropEvent(QDropEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;
private:
	Ui::OfdExploreClass ui;
	YOfdParser* m_parser = nullptr;
	QVBoxLayout* layoutFont = nullptr;
};
