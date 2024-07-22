#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QWidget>
#include<QPushButton>
#include<QEnterEvent>
class MyButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MyButton(QWidget *parent = nullptr);
    void setImage(QString normal,QString hover,QString pressed);
protected:
    //鼠标按下
    void mousePressEvent(QMouseEvent*ev) override;
    //鼠标释放
    void mouseReleaseEvent(QMouseEvent*ev) override;
    //鼠标进入
    void enterEvent(QEnterEvent*ev) override;//这里很坑Qt6把类型换成QEnterEvent了
    //鼠标离开
    void leaveEvent(QEvent*ev) override;
    //绘图
    void paintEvent(QPaintEvent*ev) override;
private:
    QString m_normal;
    QString m_hover;
    QString m_pressed;
    QPixmap m_pixmap;
signals:
};

#endif // MYBUTTON_H
