#include "player.h"
#include<QDebug>
Player::Player(QObject *parent)
{
    m_score=0;
    m_isWin=false;
}

Player::Player(QString name, QObject *parent):Player(parent)//委托构造函数
{
    m_name=name;
}

void Player::setName(QString name){
    m_name=name;
}
QString Player::getName() const
{
    return m_name;
}
void Player::setRole(Role role){
    m_role=role;
}
Player::Role Player::getRole() const
{
    return m_role;
}

void Player::setSex(Sex sex)
{
    m_sex=sex;
}

Player::Sex Player::getSex() const
{
    return m_sex;
}

void Player::setDirection(Direction direction)
{
    m_direction=direction;
}

Player::Direction Player::getDirection() const
{
    return m_direction;
}

void Player::setType(Type type)
{
    m_type=type;
}

Player::Type Player::getType() const
{
    return m_type;
}

void Player::setScore(int score)
{
    m_score=score;
}

int Player::getScore() const
{
    return m_score;
}

void Player::setWin(bool flag)
{
    m_isWin=flag;
}

bool Player::isWin() const
{
    return m_isWin;
}

void Player::setPrevPlayer(Player *player)
{
    m_prev=player;
}

void Player::setNextPlayer(Player *player)
{
    m_next=player;
}

Player *Player::getPrevPlayer() const
{
    return m_prev;
}

Player *Player::getNextPlayer() const
{
    return m_next;
}

void Player::grabLordBet(int point)
{
    emit notifyGrabLordBet(this,point);
}

void Player::storeDispatchCard(Card &card)
{
    m_cards.add(card);
    Cards cs;
    cs.add(card);
    emit notifyPickCards(this,cs);
}

void Player::storeDispatchCard(Cards &cards)
{
    m_cards.add(cards);
    emit notifyPickCards(this,cards);
}

Cards Player::getCards() const
{
    return m_cards;
}

void Player::clearCards()
{
    m_cards.clear();
}

void Player::playHand(Cards &cards)
{
    m_cards.remove(cards);
    emit notifyPlayHand(this,cards);
}

// void Player::setPendingInfo(Player *player, Cards &cards)
// {
//     m_pendPlayer=player;
//     m_pendCards=cards;
// }

Player *Player::getPendPlayer() const
{
        return m_pendPlayer;
}

Cards Player::getPendCards() const
{
    return m_pendCards;

}

void Player::storePendingInfo(Player *player, Cards &cards)
{
    m_pendPlayer=player;
    m_pendCards=cards;
}

void Player::prepareCallLord()
{

}

void Player::preparePlayHand()
{

}

void Player::thingkingCallLord()
{

}

void Player::thinkPlayHand()
{

}

