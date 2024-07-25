#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_OfdReader.h"
#include "YOfdParser.h"
#include "EventHandler.h"

#include <QKeyEvent>


class OfdReader : public QMainWindow
{
    Q_OBJECT

public:
    OfdReader(QWidget *parent = Q_NULLPTR);
    bool openFile(QString fileName);

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;


private:
    Ui::OfdReaderClass ui;
    YOfdParser* m_parser = nullptr;
    EventHandler* m_winHander;

    int m_currentPage = 0;
};
