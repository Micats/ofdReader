#pragma execution_character_set("utf-8")
#include "OfdReader.h"
#include "YOfdParser.h"
#include <QFontDatabase>
#include <QFileDialog>
OfdReader::OfdReader(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	//this->setWindowFlag(Qt::FramelessWindowHint);
	//this->setAttribute(Qt::WA_TranslucentBackground);
	//this->setAttribute(Qt::WA_StyledBackground);

	m_winHander = new EventHandler(this);
	m_winHander->setMoveEvent(true);

	

	m_currentPage = 0;
	
	

}

bool OfdReader::openFile(QString fileName)
{
	

	m_parser = new YOfdParser;
	m_parser->loadFile(fileName);
	//m_parser->parseAll();

	//获取第一页大小 96dpi
	QSizeF size = ToolHelper::mmToPixel(m_parser->getPhysicalPageSizeF(0));
	this->resize(size.toSize());
	return true;
}

void OfdReader::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);
	if (!m_parser)return;
	m_parser->paintPageToWidget(this, m_currentPage);

}

void OfdReader::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Down)
	{
		if (m_currentPage + 1 < m_parser->getTotalPageNum())
		{
			m_currentPage++;
			this->update();
		}
		
	}
	else if(event->key() == Qt::Key_Up)
	{

		if (m_currentPage - 1 >=0)
		{
			m_currentPage--;
			this->update();
		}

	}
	else if(event->key() == Qt::Key_O)
	{
		QString fileName =  QFileDialog::getOpenFileName(this,"打开OFD文件","","OFD(*.ofd)");
		if (fileName.isEmpty())
			return;

		if (m_parser)
		{
			delete m_parser;
			m_parser = nullptr;
			m_parser = new YOfdParser;
			m_parser->loadFile(fileName);
			//m_parser->parseAll();
			QSizeF size = ToolHelper::mmToPixel(m_parser->getPhysicalPageSizeF(0));
			this->resize(size.toSize());
			update();
		}

	}

}
