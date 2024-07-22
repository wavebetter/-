#include "card.h"

Card::Card() {}

void Card::setPoint(CardPoint point){
    m_point=point;
}
void Card::setSuit(CardSuit suit){
    m_suit=suit;
}
Card::CardPoint Card::point() const{
    return m_point;
}
Card::CardSuit Card::suit() const{
    return m_suit;
}
bool lessSort(const Card&c1,const Card&c2){
    if(c1.point()==c2.point()){
        return c1.suit()<c2.suit();
    }else{
        return c1.point()<c2.point();
    }
}
bool greatorSort(const Card&c1,const Card&c2){
    if(c1.point()==c2.point()){
        return c1.suit()>c2.suit();
    }else{
        return c1.point()>c2.point();
    }
}
bool operator== (const Card&left,const Card&right){
    return (left.point()==right.point())&&(left.suit()==right.suit());
}
bool operator< (const Card&left,const Card&right){
    if(left.point()==right.point()){
        return left.suit()<right.suit();
    }else{
        return left.point()<right.point();
    }
}
uint qHash(const Card& card){
    return card.point()*100+card.suit();//确保哈希值都不同
}
