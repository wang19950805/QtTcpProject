#include "widget.h"
#include "ui_widget.h"

#include <iostream>
#include <QMessageBox>
#include <QNetworkProxy>
#include <QTcpSocket>
#include <qnetworkinterface.h>



Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setLayout(ui->verticalLayout);
    server = new QTcpServer(this);
    // if(server->listen("192.168.1.4",8888)){
    //     qDebug()<<"server disconnect!";
    //     return;
    // }
    connect(server,SIGNAL(newConnection()),this,SLOT(on_newClient_connect()));
    ui->btnStopListen->setEnabled(false);
    ui->btnLineOut->setEnabled(false);
    ui->btnSendMsg->setEnabled(false);

    QList<QHostAddress> address = QNetworkInterface::allAddresses();
    for (QHostAddress addr  : address) {
        if(addr.protocol() == QAbstractSocket::IPv4Protocol){
            ui->comboBox_SelectIPAddress->addItem(addr.toString());
        }
    }

    connect(ui->comboBoxChildren,&MyComboBox::on_ComboBox_cilcked,this,&Widget::myCombox_Refresh);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_newClient_connect()
{
    if(server->hasPendingConnections()){
        //nextPendingConnection 获取新的客户端链接
        QTcpSocket *socket = server->nextPendingConnection();

        qDebug()<<"new client connect!";
        qDebug()<<"client address:"<<socket->peerAddress().toString()<<"client port:"<<socket->peerPort();

        ui->textEditRev->insertPlainText("客户端地址:"+socket->peerAddress().toString()+"\n端口号:"+QString::number(socket->peerPort()));

        connect(socket,SIGNAL(readyRead()),this,SLOT(on_readyRead_handler()));
        connect(socket,SIGNAL(disconnected()),this,SLOT(on_disconnected_handler()));
        connect(socket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(on_stateChanged_handler(QAbstractSocket::SocketState)));

        ui->comboBoxChildren->addItem(QString::number(socket->peerPort()));
        ui->comboBoxChildren->setCurrentText(QString::number(socket->peerPort()));
        if(!ui->btnSendMsg->isEnabled()){
            ui->btnSendMsg->setEnabled(true);
        }
    }
}

void Widget::on_btnListen_clicked()
{
    int port = ui->lineEditPort->text().toInt();
    if(!server->listen(QHostAddress(ui->comboBox_SelectIPAddress->currentText()),port)){
        qDebug()<<"no connect!";
        QMessageBox msgBox;
        msgBox.setWindowTitle("监听失败!");
        msgBox.setText("端口占用!");
        msgBox.exec();
        return;
    }
    ui->btnListen->setEnabled(false);
    ui->btnStopListen->setEnabled(true);
    ui->btnLineOut->setEnabled(true);
}


void Widget::on_btnStopListen_clicked()
{
    QList<QTcpSocket *> tcpSocketClients = server->findChildren<QTcpSocket *>();
    for(QTcpSocket *tmpSocket : tcpSocketClients){
        tmpSocket->close();
    }
    server->close();
    ui->btnListen->setEnabled(true);
    ui->btnLineOut->setEnabled(false);
    ui->btnStopListen->setEnabled(false);
}


void Widget::on_btnLineOut_clicked()
{
    on_btnStopListen_clicked();
    delete server;
    this->close();
}

void Widget::on_readyRead_handler()
{
    QTcpSocket *scoket = qobject_cast<QTcpSocket *>(sender());
    QByteArray revData = scoket->readAll();
    // ui->textEditRev->insertPlainText("客户端: "+revData + "\n");
    insertRevTextByColor(revData,Qt::black);

}

void Widget::on_disconnected_handler()
{
   QTcpSocket *tmpScoket = qobject_cast<QTcpSocket *>(sender());
   int port = tmpScoket->peerPort();
   qDebug()<<"client disconnect!"+ QString::number(port);
   int index = ui->comboBoxChildren->findText(QString::number(port));
   ui->comboBoxChildren->removeItem(index);
}

void Widget::on_stateChanged_handler(QAbstractSocket::SocketState scoketState)
{
    QTcpSocket *tmpScoket = qobject_cast<QTcpSocket *>(sender());
    qDebug()<<"client connect status changed!";
    switch(scoketState){
    //断开链接
    case QAbstractSocket::UnconnectedState:
        qDebug()<<"client Unconnect!";
        ui->textEditRev->insertPlainText("client Unconnect!");
        tmpScoket->deleteLater();
        break;
    //链接
    case QAbstractSocket::ConnectedState:
        qDebug()<<"client Connected!";
        break;
    //链接中
    case QAbstractSocket::ConnectingState:
        qDebug()<<"client Connecting!";
        break;
    //关闭链接
    case QAbstractSocket::ClosingState:
        qDebug()<<"client Closing!";
        break;

    default:
        break;

    }
}


void Widget::on_btnSendMsg_clicked()
{
    QList<QTcpSocket *> tcpSocketClients = server->findChildren<QTcpSocket *>();
    if(tcpSocketClients.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("发送失败!");
        msgBox.setText("当前暂无客户端链接!");
        msgBox.exec();
        return;
    }
    if(ui->comboBoxChildren->currentText() == "all"){
        for(QTcpSocket *tmpSocket : tcpSocketClients){
            tmpSocket->write(ui->textEditSend->toPlainText().toStdString().c_str());
        }

    }else{
        QString currentSocket = ui->comboBoxChildren->currentText();
        for(QTcpSocket *tmpSocket : tcpSocketClients){
            if(QString::number(tmpSocket->peerPort()) == currentSocket){
                 tmpSocket->write(ui->textEditSend->toPlainText().toStdString().c_str());
            }
        }

    }
    QByteArray revData = ui->textEditSend->toPlainText().toUtf8();
    insertRevTextByColor(revData,Qt::red);
}

void Widget::myCombox_Refresh()
{
    ui->comboBoxChildren->clear();
    qDebug()<<"refresh!";
    QList<QTcpSocket *> tcpSocketClients = server->findChildren<QTcpSocket *>();
    for (QTcpSocket *tmpSocket: tcpSocketClients) {
        if(tmpSocket != nullptr){
            ui->comboBoxChildren->addItem(QString::number(tmpSocket->peerPort()));
        }
    }
    ui->comboBoxChildren->addItem("all");
}


// void Widget::on_comboBoxChildren_activated(int index)
// {
//     childIndex = index;
// }

void Widget::insertRevTextByColor(QString str, Qt::GlobalColor color)
{

    QTextCursor cursor = ui->textEditRev->textCursor();
    QTextCharFormat format;
    format.setForeground(QBrush(QColor(color)));
    cursor.setCharFormat(format);
    cursor.insertText(str);
    ui->textEditRev->moveCursor(QTextCursor::End);
    ui->textEditRev->ensureCursorVisible();

}
