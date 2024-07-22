#include "cardpanel.h"
#include<QPainter>
#include<QMouseEvent>
CardPanel::CardPanel(QWidget *parent)
    : QWidget{parent}
{
    m_isfront=true;
}

void CardPanel::setImage(QPixmap &front,QPixmap &back){
    m_front=front;
    m_back=back;
    setFixedSize(m_front.size());
}
QPixmap CardPanel::getImage(){
    return m_front;
}
void CardPanel::paintEvent(QPaintEvent *event){
    QPainter p(this);
    if(m_isfront){
        p.drawPixmap(rect(),m_front);
    }else{
        p.drawPixmap(rect(),m_back);
    }
}

void CardPanel::mousePressEvent(QMouseEvent *event)
{
    emit cardsSelectde(event->button());
}
void CardPanel::setFrontSize(bool flag){
    m_isfront=flag;
}
bool CardPanel::isFrontSide(){
    return m_isfront;
}
void CardPanel::setSelected(bool flag){
    m_isSelect=flag;
}
bool CardPanel::isSelected(){
    return m_isSelect;
}
void CardPanel::setCard(Card& card){
    m_card=card;
}
Card CardPanel::getCard(){
    return m_card;
}
void CardPanel::setOwner(Player*player){
    m_owner=player;
}
Player*CardPanel::getOwner(){
    return m_owner;
}

void CardPanel::clicked()
{
    emit cardsSelectde(Qt::LeftButton);
}
