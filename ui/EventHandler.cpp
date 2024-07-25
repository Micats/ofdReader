#pragma execution_character_set("utf-8")
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QtMath>
#include <QLayout>

#include "EventHandler.h"


EventHandler::EventHandler(QWidget *parent)
	: QWidget(parent)
{
	Q_ASSERT(parent);
	m_parent = parent;
	m_moveWidget = parent;

	m_parent->installEventFilter(this);
	this->hide();


}

EventHandler::~EventHandler()
{
}

void EventHandler::setSubWindowFlags()
{
	if (m_parent)
	{
		//WindowStaysOnTopHint 
		m_parent->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
		m_parent->setAttribute(Qt::WA_StyledBackground);  //自定义背景
		
	}
}

void EventHandler::setMoveEvent(bool can)
{
	m_canMove = can;
}

void EventHandler::setAcceptMoveEvent(bool isAccept)
{
	m_bAcceptMove = isAccept;
}

void EventHandler::setMoveWidget(QWidget* widget)
{
	m_moveWidget = widget;
}

void EventHandler::setPaintShadow(bool can)
{
	Q_ASSERT(m_parent);
	m_parent->setAttribute(Qt::WA_TranslucentBackground,can);
	m_canPaintShadow = can;
}

void EventHandler::setPaintMargins(int margins)
{
	m_paintMargins = margins;
}

void EventHandler::setAcceptPaintShadowEvent(bool isAccept)
{
	m_bAcceptPaintShadow = isAccept;
}

bool EventHandler::eventFilter(QObject* watched, QEvent* evt)
{

	//窗口移动事件
	if (m_canMove && evt->type() == QEvent::MouseButtonPress && watched == m_moveWidget) {
		QMouseEvent* event = static_cast<QMouseEvent*>(evt);
		if (event->button() == Qt::LeftButton) {
			mousePressed = true;
			mousePoint = event->globalPos() - m_parent->pos();
			return m_bAcceptMove;
		}
	}
	else if (m_canMove && evt->type() == QEvent::MouseButtonRelease && watched == m_moveWidget) {
		mousePressed = false;
		return m_bAcceptMove;
	}
	else if (m_canMove && evt->type() == QEvent::MouseMove && watched == m_moveWidget) {
		QMouseEvent* event = static_cast<QMouseEvent*>(evt);
		if (mousePressed && (event->buttons() && Qt::LeftButton)) {
			m_parent->move(event->globalPos() - mousePoint);
			return m_bAcceptMove;
		}
	}


	//绘制边框阴影事件
	else if (m_canPaintShadow && evt->type() == QEvent::Paint && watched == m_parent)
	{
		QPainterPath path;
		path.setFillRule(Qt::WindingFill);
		QRectF rect(m_paintMargins, m_paintMargins, m_parent->width() - m_paintMargins*2, m_parent->height() - m_paintMargins*2); //偏移 边框为10
		//左顶，宽高，边为10px，则左右，上下各少20px

		path.addRoundedRect(rect, 0, 0); //圆角

		QPainter painter(m_parent);
		painter.setRenderHint(QPainter::Antialiasing, true); //抗锯齿
		painter.fillPath(path, QBrush(Qt::white)); //无边框绘制

		QColor color(0, 0, 0, 50);
		for (int i = 0; i < m_paintMargins; i++)
		{
			QPainterPath path;
			path.setFillRule(Qt::WindingFill);
			path.addRect(m_paintMargins - i, m_paintMargins - i, m_parent->width() - (m_paintMargins - i) * 2,
				m_parent->height() - (m_paintMargins - i) * 2);
			color.setAlpha(150 - qSqrt(i) * 50);
			painter.setPen(color);
			painter.drawPath(path); //drawpath会绘制边框
		}
	}

	return QWidget::eventFilter(watched, evt);
}
