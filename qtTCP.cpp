#include "qtTCP.h"

qtTCP::qtTCP(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
	socket = new QTcpSocket();

	//连接信号槽
	QObject::connect(socket, &QTcpSocket::readyRead, this, &qtTCP::socket_Read_Data);
	QObject::connect(socket, &QTcpSocket::disconnected, this, &qtTCP::socket_Disconnected);

	ui.pushButton_2->setEnabled(false);
	ui.lineEdit->setText("127.0.0.1");
	ui.lineEdit_2->setText("8765");
}

void qtTCP::on_pushButton_clicked()
{
	if (ui.pushButton->text() == tr("连接"))
	{
		//获取IP地址
		QString IP = ui.lineEdit->text();
		//获取端口号
		int port = ui.lineEdit_2->text().toInt();

		//取消已有的连接
		socket->abort();
		//连接服务器
		socket->connectToHost(IP, port);

		//等待连接成功
		if (!socket->waitForConnected(30000))
		{
			qDebug() << "Connection failed!";
			return;
		}
		qDebug() << "Connect successfully!";

		//发送按键使能
		ui.pushButton_2->setEnabled(true);
		//修改按键文字
		ui.pushButton->setText("断开连接");
	}
	else
	{
		//断开连接
		socket->disconnectFromHost();
		//修改按键文字
		ui.pushButton->setText("连接");
		ui.pushButton_2->setEnabled(false);
	}
}

void qtTCP::on_pushButton_2_clicked()
{
	qDebug() << "Send: " << ui.textEdit_2->toPlainText();
	//获取文本框内容并以ASCII码形式发送
	socket->write(ui.textEdit_2->toPlainText().toLocal8Bit());
	socket->flush();

	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_date = current_date_time.toString("yyyy.MM.dd hh:mm:ss");
	QString str = ui.textEdit->toPlainText();
	str += "\n" + current_date + "   客户机：" + ui.textEdit_2->toPlainText();
	ui.textEdit->setText(str);

	ui.textEdit_2->setText("");
}

void qtTCP::socket_Read_Data()
{
	//数据接收时间
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_date = current_date_time.toString("yyyy.MM.dd hh:mm:ss");
	//读取缓冲区数据
	QByteArray buffer = socket->readAll();
	//解决中文乱码
	QString tmpQStr = QString::fromLocal8Bit(buffer);
	if (!buffer.isEmpty())
	{
		QString str = ui.textEdit->toPlainText();
		str += "\n" + current_date + "   服务器: " + tmpQStr;
		//刷新显示
		ui.textEdit->setText(str);
	}
}

void qtTCP::socket_Disconnected()
{
	//发送按键失能
	ui.pushButton_2->setEnabled(false);
	//修改按键文字
	ui.pushButton->setText("连接");
	qDebug() << "Disconnected!";
}