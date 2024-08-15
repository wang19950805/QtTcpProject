#ifndef WIDGET_H
#define WIDGET_H

#include <QTcpSocket>
#include <QTimer>
#include <QWidget>

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

private slots:
    void on_btnLink_clicked();
    void readData_From_Server();

    void on_btnSendMsg_clicked();

    void on_btnDiscon_clicked();

    void on_Socket_StateChanged(QAbstractSocket::SocketState socketState);

    void on_Socket_Error(QAbstractSocket::SocketError socketError);

    void on_Socket_ConnectTimeout();
private:
    Ui::Widget *ui;
    QTcpSocket *sockectClient;
    QTimer *timeout;
    void insertRevTextByColor(QString str,Qt::GlobalColor color);

};
#endif // WIDGET_H
