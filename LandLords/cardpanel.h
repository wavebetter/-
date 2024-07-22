#ifndef CARDPANEL_H
#define CARDPANEL_H

#include <QWidget>
#include "card.h"
#include "player.h"
class CardPanel : public QWidget
{
    Q_OBJECT
public:
    explicit CardPanel(QWidget *parent = nullptr);
    //设置获取图片函数
    void setImage(QPixmap &front,QPixmap &back);
    QPixmap getImage();

    //扑克牌显示的一面
    void setFrontSize(bool flag);
    bool isFrontSide();

    //记录窗口是否被选中
    void setSelected(bool flag);
    bool isSelected();

    //扑克牌的花色和点数
    void setCard(Card& card);
    Card getCard();

    //扑克牌的所有者
    void setOwner(Player*player);
    Player*getOwner();
    //模拟扑克牌的点击事件
    void clicked();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent*event);
private:
    QPixmap m_front;
    QPixmap m_back;
    bool m_isfront;
    bool m_isSelect;
    Card m_card;
    Player* m_owner;
signals:
    void cardsSelectde(Qt::MouseButton button);
};

#endif // CARDPANEL_H
