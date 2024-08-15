#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <mycombobox.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    QTcpServer *server;

public slots:
    void on_newClient_connect();

private slots:
    void on_btnListen_clicked();

    void on_btnStopListen_clicked();

    void on_btnLineOut_clicked();

    void on_readyRead_handler();

    void on_disconnected_handler();

    void on_stateChanged_handler(QAbstractSocket::SocketState scoketState);

    void on_btnSendMsg_clicked();

    void myCombox_Refresh();

    // void on_comboBoxChildren_activated(int index);

private:
    Ui::Widget *ui;
    int childIndex;

    void insertRevTextByColor(QString str,Qt::GlobalColor color);
};
#endif // WIDGET_H
