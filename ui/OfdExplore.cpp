#pragma execution_character_set("utf-8")
#include "OfdExplore.h"
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QMimeData>
#include <QXmlFormatter>
#include <QComboBox>
#include "Items.h"
#include "XmlHignlighter.h"

OfdExplore::OfdExplore(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setAcceptDrops(true);

	initCtrls();

	ui.frame->hide();
}

OfdExplore::~OfdExplore()
{

}

void OfdExplore::initCtrls()
{
	ui.labelDisplay->setScaledContents(false);
	XmlHignlighter* xmlHighlighter = new XmlHignlighter(ui.plainTextEditRaw->document());

	QPushButton* btnOpen = new QPushButton(ui.mainToolBar);
	btnOpen->setText("");
	btnOpen->setToolTip("打开文件");
	btnOpen->setIcon(QIcon(":/OfdReader/Src/Icon/工具栏-打开文件.png"));
	btnOpen->setFlat(true);
	btnOpen->setFixedSize(40,40);
	connect(btnOpen, &QPushButton::clicked, this, &OfdExplore::on_actionOpen_triggered);
	ui.mainToolBar->addWidget(btnOpen);

	QPushButton* btnClose = new QPushButton(ui.mainToolBar);
	btnClose->setText("");
	btnClose->setToolTip("关闭文件");
	btnClose->setIcon(QIcon(":/OfdReader/Src/Icon/工具栏-关闭文件.png"));
	btnClose->setFlat(true);
	btnClose->setFixedSize(40, 40);
	connect(btnClose, &QPushButton::clicked, this, &OfdExplore::on_actionClose_triggered);
	ui.mainToolBar->addWidget(btnClose);
}



void OfdExplore::createTreeItem(QStringList paths)
{
	QString name;
	QTreeWidgetItem* parent = nullptr;// ui.treeWidgetDoc;
	for (int i=0;i<paths.size();i++)
	{
		name += paths[i];
		QString text = paths[i];
		bool isLeaf = (i == paths.size() - 1);
		auto item = ToolHelper::findItem(ui.treeWidgetDoc, i, name);
		if (!item)
		{
			QTreeWidgetItem* newItem;
			if (i == 0)
			{
				newItem = new QTreeWidgetItem(ui.treeWidgetDoc);
				newItem->setText(0,text);
				newItem->setData(0, Qt::UserRole, name);
				ui.treeWidgetDoc->addTopLevelItem(newItem);
				parent = newItem;
			}
			else
			{
				newItem = new QTreeWidgetItem(parent);
				newItem->setText(0, text);
				newItem->setData(0, Qt::UserRole, name);
				parent->addChild(newItem);
				parent = newItem;
			}

			if (!isLeaf) 
			{
				newItem->setIcon(0, QIcon(":/OfdReader/Src/Icon/目录-文件夹.png"));
				newItem->setText(1, "文件夹");
			}	
			else 
			{
				QString suffix = name.split(".").last();
				if(name.endsWith(".xml",Qt::CaseInsensitive))
					newItem->setIcon(0, QIcon(":/OfdReader/Src/Icon/目录-xml.png"));
				else if (name.endsWith(".png", Qt::CaseInsensitive) || name.endsWith(".jpg", Qt::CaseInsensitive) || name.endsWith(".bmp", Qt::CaseInsensitive))
					newItem->setIcon(0, QIcon(":/OfdReader/Src/Icon/目录-图片.png"));
				else if (name.endsWith(".ttf", Qt::CaseInsensitive) || name.endsWith(".ott", Qt::CaseInsensitive))
					newItem->setIcon(0, QIcon(":/OfdReader/Src/Icon/目录-字体.png"));
				else if (name.endsWith(".dat", Qt::CaseInsensitive))
					newItem->setIcon(0, QIcon(":/OfdReader/Src/Icon/目录-二进制.png"));
				newItem->setText(1, suffix);
			}
			
		}
		else
		{
			parent = item;
		}

		name += "/";
	}
}

void OfdExplore::closeFile()
{
	ui.statusBar->showMessage("正在关闭文件",3);

	ui.treeWidgetDoc->clear();
	ui.plainTextEditRaw->clear();
	ui.labelDisplay->setPixmap(QPixmap());
	QWidget* w = ui.scrollAreaFont->widget();
	if(w)delete w;
	ui.scrollAreaFont->setWidget(nullptr);


	if (m_parser)
	{
		delete m_parser;
		m_parser = nullptr;
	}
}

void OfdExplore::openFile(QString fileName)
{

	ui.statusBar->showMessage("正在打开文件:" + fileName+"...");
	QApplication::processEvents(QEventLoop::AllEvents, 300);  //刷新界面

	defer(ui.statusBar->showMessage("当前文件:" + fileName););
	if (m_parser != nullptr)
	{
		delete m_parser;
		m_parser = nullptr;
	}

	m_parser = new YOfdParser();
	bool ret = m_parser->loadFile(fileName);
	if (!ret)
	{
		QMessageBox::critical(this, "错误", "文件打开失败");
		return;
	}

	//加载结构
	QList<QTreeWidgetItem*> list;
	QStringList fileNames = YOfdUncompress::instance().getFilePaths();// m_parser->getFilePaths();
	for (auto fileName : fileNames)
	{
		QStringList paths = fileName.split("/");
		createTreeItem(paths);
	}


	ui.treeWidgetDoc->sortItems(1, Qt::DescendingOrder);
	ui.treeWidgetDoc->hideColumn(1);

	//加载字体信息
	QWidget* w = new QWidget;
	layoutFont = new QVBoxLayout;
	QList<FontInfo> infos = m_parser->getPublicRes()->getAllFontInfo();
	for( int i = 0; i<infos.size();i++ )
	{
		FontItem* item = new FontItem;
		//connect(item, &FontItem::sigSwitchFont, this, &OfdExplore::on_switchFont);
		item->setFontInfo(infos[i]);
		layoutFont->addWidget(item);
	}
	w->setLayout(layoutFont);
	ui.scrollAreaFont->setWidget(w);

}

QString OfdExplore::formatXml(QString xml)
{
	if (!xml.isEmpty()) {
		QString xmlOut;
		QXmlStreamReader reader(xml);
		QXmlStreamWriter writer(&xmlOut);
		writer.setAutoFormatting(true);

		while (!reader.atEnd()) {
			reader.readNext();
			if (!reader.isWhitespace()) {
				writer.writeCurrentToken(reader);
			}
		}

		return xmlOut;
	}
	else {
		return xml;
	}

}

void OfdExplore::on_actionOpen_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this, "打开文件", "", "Files(*.ofd)");
	if (fileName.isEmpty())return;
	
	openFile(fileName);
}

void OfdExplore::on_actionClose_triggered()
{
	closeFile();
}

void OfdExplore::on_actionFind_triggered()
{
	ui.frame->show();
}

void OfdExplore::on_treeWidgetDoc_itemClicked(QTreeWidgetItem* item, int col)
{
	if (!item)
		return;

	if (item->childCount() > 0)
		return;

	QString path = item->data(0, Qt::UserRole).value<QString>();
	auto bytes = YOfdUncompress::instance().getFile(path);
	
	if (path.endsWith(".xml", Qt::CaseInsensitive)) {
		ui.plainTextEditRaw->setPlainText(formatXml(bytes));
	}

	if (path.endsWith("Content.xml", Qt::CaseInsensitive)) {
		//渲染页
		int index = path.indexOf("Page_");
		if (index == -1)
			return;

		int indexLast = path.lastIndexOf("/");
		if (indexLast == -1)
			return;

		QString pageStr = path.mid(index + 5, indexLast-index-5);
		int page = pageStr.toInt();

		QImage img= m_parser->paintPageToImage(page);
		
		ui.labelDisplay->setPixmap(QPixmap::fromImage(img));
	}
	else if (path.endsWith("jpg", Qt::CaseInsensitive) || path.endsWith("png", Qt::CaseInsensitive)|| path.endsWith("bmp", Qt::CaseInsensitive))
	{
		QImage img;
		img.loadFromData(bytes);
		ui.labelDisplay->setPixmap(QPixmap::fromImage(img));
	}
	else if (path.endsWith("ttf", Qt::CaseInsensitive) || path.endsWith("tif", Qt::CaseInsensitive) || path.endsWith("cff", Qt::CaseInsensitive))
	{
		QString info = m_parser->getFontInfo(path.split("/").last());
		ui.plainTextEditInfo->setPlainText(info);
	}
}

void OfdExplore::on_pushButtonPre_clicked()
{
	ui.labelSearchResult->setText("");

	QString searchText = ui.lineEditSearch->text();
	if (searchText.isEmpty())
		return;
	
	QString text = ui.plainTextEditRaw->toPlainText();
	QTextCursor cursor = ui.plainTextEditRaw->textCursor();
	int index = -1;



	index = text.lastIndexOf(searchText, cursor.position() - text.length() - 1, ui.checkBoxMatch->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);
	//如果index大于等于0，说明找到了
	if (index >= 0)
	{
		//选中高亮（先找到第二个位置，再找到第一个位置）
		cursor.setPosition(index + searchText.length());
		cursor.setPosition(index, QTextCursor::KeepAnchor);
		//把光标信息设置到对应的文本框下面
		ui.plainTextEditRaw->setFocus();
		ui.plainTextEditRaw->setTextCursor(cursor);
		return;
	}

	ui.labelSearchResult->setText("搜索到开始，无匹配项");
}

void OfdExplore::on_pushButtonNext_clicked()
{
	ui.labelSearchResult->setText("");

	QString searchText = ui.lineEditSearch->text();
	if (searchText.isEmpty())
		return;
	


	QString text = ui.plainTextEditRaw->toPlainText();
	QTextCursor cursor = ui.plainTextEditRaw->textCursor();

	qDebug() << cursor.position();

	int index = -1;
	index = text.indexOf(searchText, cursor.position(), ui.checkBoxMatch->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);
	if (index >= 0)
	{
		cursor.setPosition(index);
		cursor.setPosition(index + searchText.length(), QTextCursor::KeepAnchor);
		ui.plainTextEditRaw->setFocus();
		ui.plainTextEditRaw->setTextCursor(cursor);
		return;
	}
	ui.labelSearchResult->setText("搜索到结尾，无匹配项");
}


void OfdExplore::on_pushButtonFindClose_clicked()
{
	ui.frame->hide();
}

void OfdExplore::on_switchFont(QString fontName, QString newFontName)
{
	m_parser->getPublicRes()->switchFont(fontName, newFontName);
}

void OfdExplore::dragEnterEvent(QDragEnterEvent* event)
{
	closeFile();
	if (event->mimeData()->hasUrls())                    // 数据中是否包含URL
	{
		event->setDropAction(Qt::MoveAction);
		event->acceptProposedAction();                  // 如果是则接收动作
	}
	else event->ignore();
}

void OfdExplore::dropEvent(QDropEvent* event)
{
	const QMimeData* mimeData = event->mimeData();      // 获取MIME数据
	if (mimeData->hasUrls()) {                            // 如果数据中包含URL
		QList<QUrl> urlList = mimeData->urls();         // 获取URL列表
		// 将其中第一个URL表示为本地文件路径
		QString fileName = urlList.at(0).toLocalFile();
		if (!fileName.isEmpty()) {                        // 如果文件路径不为空
			QFile file(fileName);     // 建立QFile对象并且以只读方式打开该文件
			if (file.exists())
			{
				openFile(fileName);
			}
		}
	}
}

void OfdExplore::keyPressEvent(QKeyEvent* event)
{
	if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_F)
	{
		//测试
		ui.frame->show();
	}
}
