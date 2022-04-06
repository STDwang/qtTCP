#pragma once
#pragma execution_character_set("utf-8")
#include <QtWidgets/QWidget>
#include "ui_qtTcpSever.h"
#include "qdebug.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextCodec>
#include <QDateTime>

class qtTcpSever : public QWidget
{
    Q_OBJECT

public:
    qtTcpSever(QWidget *parent = Q_NULLPTR);

private slots:
	void on_pushButton_Listen_clicked();
	void on_pushButton_Send_clicked();
	void server_New_Connect();
	void socket_Read_Data();
	void socket_Disconnected();

private:
    Ui::qtTcpSeverClass ui;
	QTcpServer* server;
	QTcpSocket* socket;
};
