#include "mybutton.h"
#include<QMouseEvent>
#include<QPainter>
#include<QDebug>
MyButton::MyButton(QWidget *parent)
    : QPushButton{parent}
{}

void MyButton::setImage(QString normal, QString hover, QString pressed)
{
    m_normal=normal;
    m_hover=hover;
    m_pressed=pressed;
    m_pixmap.load(m_normal);
    update();
}

void MyButton::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::LeftButton){
        m_pixmap.load(m_pressed);//左键点击按钮之后应该显示的图片
        update();
    }
    QPushButton::mousePressEvent(ev);//再执行父类相关的操作
}

void MyButton::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::LeftButton){
        m_pixmap.load(m_normal);//正常状态下按钮上显示的图片
        update();
    }

    QPushButton::mouseReleaseEvent(ev);//这句话一定要写，要不单击不给你发clicked
}

void MyButton::enterEvent(QEnterEvent *ev)
{
    m_pixmap.load(m_hover);//鼠标悬在按钮上显示的图片
    update();
    QPushButton::enterEvent(ev); // 调用父类的 enterEvent
}

void MyButton::leaveEvent(QEvent *ev)
{
    m_pixmap.load(m_normal);//鼠标悬在按钮上显示的图片
    update();
}

void MyButton::paintEvent(QPaintEvent *ev)
{
    QPainter p(this);
    p.drawPixmap(rect(),m_pixmap);

}
