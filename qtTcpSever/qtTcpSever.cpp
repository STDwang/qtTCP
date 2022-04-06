#include "qtTcpSever.h"

qtTcpSever::qtTcpSever(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
	server = new QTcpServer();
	//�����źŲ�
	connect(server, &QTcpServer::newConnection, this, &qtTcpSever::server_New_Connect);

	ui.lineEdit_Port->setText("8765");
	ui.pushButton_Send->setEnabled(false);
}

void qtTcpSever::on_pushButton_Listen_clicked()
{
	if (ui.pushButton_Listen->text() == "����")
	{
		//��������ȡ�˿ں�
		int port = ui.lineEdit_Port->text().toInt();

		//����ָ���Ķ˿�
		if (!server->listen(QHostAddress::Any, port))
		{
			//�����������������Ϣ
			qDebug() << server->errorString();
			return;
		}
		//�޸İ�������
		ui.pushButton_Listen->setText("ȡ������");
		qDebug() << "Listen succeessfully!";
	}
	else
	{
		socket->abort();
		//ȡ������
		server->close();
		//�޸İ�������
		ui.pushButton_Listen->setText("����");
		//���Ͱ���ʧ��
		ui.pushButton_Send->setEnabled(false);
	}

}

void qtTcpSever::on_pushButton_Send_clicked()
{
	qDebug() << "Send: " << ui.textEdit_Send->toPlainText();
	//��ȡ�ı������ݲ���ASCII����ʽ����
	socket->write(ui.textEdit_Send->toPlainText().toLocal8Bit());
	socket->flush();

	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_date = current_date_time.toString("yyyy.MM.dd hh:mm:ss");
	QString str = ui.textEdit_Recv->toPlainText();
	str += "\n" + current_date + "   ��������" + ui.textEdit_Send->toPlainText();
	ui.textEdit_Recv->setText(str);

	ui.textEdit_Send->setText("");
}

void qtTcpSever::server_New_Connect()
{
	//��ȡ�ͻ�������
	socket = server->nextPendingConnection();
	//����QTcpSocket���źŲۣ��Զ�ȡ������
	QObject::connect(socket, &QTcpSocket::readyRead, this, &qtTcpSever::socket_Read_Data);
	QObject::connect(socket, &QTcpSocket::disconnected, this, &qtTcpSever::socket_Disconnected);
	//���Ͱ���ʹ��
	ui.pushButton_Send->setEnabled(true);

	qDebug() << "A Client connect!";
}

void qtTcpSever::socket_Read_Data()
{
	//���ݽ���ʱ��
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_date = current_date_time.toString("yyyy.MM.dd hh:mm:ss");
	//��ȡ����������
	QByteArray buffer = socket->readAll();
	QString tmpQStr = QString::fromLocal8Bit(buffer);
	if (!buffer.isEmpty())
	{
		QString str = ui.textEdit_Recv->toPlainText();
		str += "\n" + current_date + "   �ͻ���: " + tmpQStr;
		//ˢ����ʾ
		ui.textEdit_Recv->setText(str);
	}
}

void qtTcpSever::socket_Disconnected()
{
	//���Ͱ���ʧ��
	ui.pushButton_Send->setEnabled(false);
	qDebug() << "Disconnected!";
}