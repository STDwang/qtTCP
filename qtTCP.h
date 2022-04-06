#pragma once
#pragma execution_character_set("utf-8")
#include <QtWidgets/QWidget>
#include <qtcpsocket.h>
#include "ui_qtTCP.h"
#include <QTextCodec>
#include <QDateTime>
#include "qdebug.h"

class qtTCP : public QWidget
{
    Q_OBJECT

public:
    qtTCP(QWidget *parent = Q_NULLPTR);

private slots:
	void on_pushButton_clicked();
	void on_pushButton_2_clicked();
	void socket_Read_Data();
	void socket_Disconnected();

private:
    Ui::qtTCPClass ui;
	QTcpSocket *socket;
};
