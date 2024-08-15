#ifndef MYCOMBOBOX_H
#define MYCOMBOBOX_H

#include <QComboBox>
#include <QWidget>

class MyComboBox : public QComboBox
{
    Q_OBJECT
public:
    MyComboBox(QWidget *parent);

signals:
    void on_ComboBox_cilcked();

protected:
    void mousePressEvent(QMouseEvent *event) override;


};

#endif // MYCOMBOBOX_H
