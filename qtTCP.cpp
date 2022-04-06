#include "qtTCP.h"

qtTCP::qtTCP(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
	socket = new QTcpSocket();

	//�����źŲ�
	QObject::connect(socket, &QTcpSocket::readyRead, this, &qtTCP::socket_Read_Data);
	QObject::connect(socket, &QTcpSocket::disconnected, this, &qtTCP::socket_Disconnected);

	ui.pushButton_2->setEnabled(false);
	ui.lineEdit->setText("127.0.0.1");
	ui.lineEdit_2->setText("8765");
}

void qtTCP::on_pushButton_clicked()
{
	if (ui.pushButton->text() == tr("����"))
	{
		//��ȡIP��ַ
		QString IP = ui.lineEdit->text();
		//��ȡ�˿ں�
		int port = ui.lineEdit_2->text().toInt();

		//ȡ�����е�����
		socket->abort();
		//���ӷ�����
		socket->connectToHost(IP, port);

		//�ȴ����ӳɹ�
		if (!socket->waitForConnected(30000))
		{
			qDebug() << "Connection failed!";
			return;
		}
		qDebug() << "Connect successfully!";

		//���Ͱ���ʹ��
		ui.pushButton_2->setEnabled(true);
		//�޸İ�������
		ui.pushButton->setText("�Ͽ�����");
	}
	else
	{
		//�Ͽ�����
		socket->disconnectFromHost();
		//�޸İ�������
		ui.pushButton->setText("����");
		ui.pushButton_2->setEnabled(false);
	}
}

void qtTCP::on_pushButton_2_clicked()
{
	qDebug() << "Send: " << ui.textEdit_2->toPlainText();
	//��ȡ�ı������ݲ���ASCII����ʽ����
	socket->write(ui.textEdit_2->toPlainText().toLocal8Bit());
	socket->flush();

	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_date = current_date_time.toString("yyyy.MM.dd hh:mm:ss");
	QString str = ui.textEdit->toPlainText();
	str += "\n" + current_date + "   �ͻ�����" + ui.textEdit_2->toPlainText();
	ui.textEdit->setText(str);

	ui.textEdit_2->setText("");
}

void qtTCP::socket_Read_Data()
{
	//���ݽ���ʱ��
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_date = current_date_time.toString("yyyy.MM.dd hh:mm:ss");
	//��ȡ����������
	QByteArray buffer = socket->readAll();
	//�����������
	QString tmpQStr = QString::fromLocal8Bit(buffer);
	if (!buffer.isEmpty())
	{
		QString str = ui.textEdit->toPlainText();
		str += "\n" + current_date + "   ������: " + tmpQStr;
		//ˢ����ʾ
		ui.textEdit->setText(str);
	}
}

void qtTCP::socket_Disconnected()
{
	//���Ͱ���ʧ��
	ui.pushButton_2->setEnabled(false);
	//�޸İ�������
	ui.pushButton->setText("����");
	qDebug() << "Disconnected!";
}