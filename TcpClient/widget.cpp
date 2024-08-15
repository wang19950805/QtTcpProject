#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setLayout(ui->verticalLayout_2);
    ui->btnSendMsg->setEnabled(false);

    sockectClient = new QTcpSocket(this);
    connect(sockectClient,SIGNAL(readyRead()),this,SLOT(readData_From_Server()));

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_btnLink_clicked()
{
    this->setEnabled(false);
    sockectClient->connectToHost(ui->lineEditIPAddr->text(),ui->lineEditPort->text().toInt());
    connect(sockectClient,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(on_Socket_StateChanged(QAbstractSocket::SocketState)));
    connect(sockectClient,SIGNAL(errorOccurred(QAbstractSocket::SocketError)),this,SLOT(on_Socket_Error(QAbstractSocket::SocketError)));
    //设置超时时间
    timeout = new QTimer();
    timeout->setInterval(5000);
    timeout->setSingleShot(true);
    timeout->start();
    connect(timeout,SIGNAL(timeout()),this,SLOT(on_Socket_ConnectTimeout()));
}

void Widget::readData_From_Server()
{
    QByteArray revData = sockectClient->readAll();
    insertRevTextByColor(revData,Qt::black);
}


void Widget::on_btnSendMsg_clicked()
{
    sockectClient->write(ui->textEditSendMsg->toPlainText().toUtf8());
    QByteArray sendData = ui->textEditSendMsg->toPlainText().toUtf8();
    insertRevTextByColor(sendData,Qt::red);
}


void Widget::on_btnDiscon_clicked()
{
    sockectClient->close();
    ui->textEditRev->append("server disconnected!");
    ui->btnLink->setEnabled(true);
    ui->lineEditIPAddr->setEnabled(true);
    ui->lineEditPort->setEnabled(true);
    ui->btnDiscon->setEnabled(false);
    ui->btnSendMsg->setEnabled(false);
}


void Widget::on_Socket_StateChanged(QAbstractSocket::SocketState socketState)
{
    // QTcpSocket tmpSocket = qobject_cast<QTcpSocket *>(sender());
    switch(socketState){
        //连接中
    case QAbstractSocket::ConnectingState:

        qDebug()<<"connecting...";
        break;
    //已连接
    case QAbstractSocket::ConnectedState:
        timeout->stop();
        timeout->destroyed();
        this->setEnabled(true);
        ui->textEditRev->append("connect success!");
        ui->btnLink->setEnabled(false);
        ui->lineEditIPAddr->setEnabled(false);
        ui->lineEditPort->setEnabled(false);
        ui->btnSendMsg->setEnabled(true);
        ui->btnDiscon->setEnabled(true);

        break;
        //断开连接
    case QAbstractSocket::UnconnectedState:

        qDebug()<<"unConnecting!";
        break;

    }
}

void Widget::on_Socket_Error(QAbstractSocket::SocketError socketError)
{
    this->setEnabled(true);
    qDebug()<<"连接错误! "+socketError;
    switch(socketError){
    case QAbstractSocket::SocketAccessError:

        break;
        //socket拒绝
    case QAbstractSocket::SocketResourceError:

        break;
        //连接拒绝
    case QAbstractSocket::ConnectionRefusedError:

        break;
        //ip错误
    case QAbstractSocket::HostNotFoundError:

        break;
    }
}

void Widget::on_Socket_ConnectTimeout()
{
    ui->textEditRev->insertPlainText("连接超时!");
    sockectClient->abort();
    this->setEnabled(true);
}

void Widget::insertRevTextByColor(QString str, Qt::GlobalColor color)
{

    QTextCursor cursor = ui->textEditRev->textCursor();
    QTextCharFormat format;
    format.setForeground(QBrush(QColor(color)));
    cursor.setCharFormat(format);
    cursor.insertText(str);

}

