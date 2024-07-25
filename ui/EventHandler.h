#pragma once

#include <QWidget>

class EventHandler : public QWidget
{
	Q_OBJECT

public:
	EventHandler(QWidget *parent);
	~EventHandler();

	void setSubWindowFlags();	//设置自定义子窗口标志

	void setMoveEvent(bool can);	//窗口拖动
	void setAcceptMoveEvent(bool isAccept);  //接收移动事件，其余控件就接收不到了
	void setMoveWidget(QWidget* widget);	//设置鼠标按压移动的窗体控件，不设置，默认是整个窗体

	void setPaintShadow(bool can);	//窗口绘制阴影
	void setPaintMargins(int margins); //窗口边框大小
	void setAcceptPaintShadowEvent(bool isAccept);

	int getMargins() { return m_paintMargins; };
protected:
	bool eventFilter(QObject* watched, QEvent* evt) override;

private:
	QWidget* m_parent;		//父窗口 窗体句柄
	QWidget* m_moveWidget;	//移动控件句柄

	bool m_canMove = false;
	bool m_bAcceptMove = true;
	bool m_canPaintShadow = false;
	bool m_bAcceptPaintShadow = true;
	int m_paintMargins = 10;	//阴影边框

	QPoint mousePoint;
	bool mousePressed = false;
};
