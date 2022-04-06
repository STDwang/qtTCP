#include "qtTcpSever.h"

qtTcpSever::qtTcpSever(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
	server = new QTcpServer();
	//连接信号槽
	connect(server, &QTcpServer::newConnection, this, &qtTcpSever::server_New_Connect);

	ui.lineEdit_Port->setText("8765");
	ui.pushButton_Send->setEnabled(false);
}

void qtTcpSever::on_pushButton_Listen_clicked()
{
	if (ui.pushButton_Listen->text() == "侦听")
	{
		//从输入框获取端口号
		int port = ui.lineEdit_Port->text().toInt();

		//监听指定的端口
		if (!server->listen(QHostAddress::Any, port))
		{
			//若出错，则输出错误信息
			qDebug() << server->errorString();
			return;
		}
		//修改按键文字
		ui.pushButton_Listen->setText("取消侦听");
		qDebug() << "Listen succeessfully!";
	}
	else
	{
		socket->abort();
		//取消侦听
		server->close();
		//修改按键文字
		ui.pushButton_Listen->setText("侦听");
		//发送按键失能
		ui.pushButton_Send->setEnabled(false);
	}

}

void qtTcpSever::on_pushButton_Send_clicked()
{
	qDebug() << "Send: " << ui.textEdit_Send->toPlainText();
	//获取文本框内容并以ASCII码形式发送
	socket->write(ui.textEdit_Send->toPlainText().toLocal8Bit());
	socket->flush();

	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_date = current_date_time.toString("yyyy.MM.dd hh:mm:ss");
	QString str = ui.textEdit_Recv->toPlainText();
	str += "\n" + current_date + "   服务器：" + ui.textEdit_Send->toPlainText();
	ui.textEdit_Recv->setText(str);

	ui.textEdit_Send->setText("");
}

void qtTcpSever::server_New_Connect()
{
	//获取客户端连接
	socket = server->nextPendingConnection();
	//连接QTcpSocket的信号槽，以读取新数据
	QObject::connect(socket, &QTcpSocket::readyRead, this, &qtTcpSever::socket_Read_Data);
	QObject::connect(socket, &QTcpSocket::disconnected, this, &qtTcpSever::socket_Disconnected);
	//发送按键使能
	ui.pushButton_Send->setEnabled(true);

	qDebug() << "A Client connect!";
}

void qtTcpSever::socket_Read_Data()
{
	//数据接收时间
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_date = current_date_time.toString("yyyy.MM.dd hh:mm:ss");
	//读取缓冲区数据
	QByteArray buffer = socket->readAll();
	QString tmpQStr = QString::fromLocal8Bit(buffer);
	if (!buffer.isEmpty())
	{
		QString str = ui.textEdit_Recv->toPlainText();
		str += "\n" + current_date + "   客户机: " + tmpQStr;
		//刷新显示
		ui.textEdit_Recv->setText(str);
	}
}

void qtTcpSever::socket_Disconnected()
{
	//发送按键失能
	ui.pushButton_Send->setEnabled(false);
	qDebug() << "Disconnected!";
}