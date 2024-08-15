#include "mycombobox.h"

#include <QMouseEvent>

MyComboBox::MyComboBox(QWidget *parent) : QComboBox(parent)
{

}

void MyComboBox::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        emit on_ComboBox_cilcked();
    }
    QComboBox::mousePressEvent(event);
}
