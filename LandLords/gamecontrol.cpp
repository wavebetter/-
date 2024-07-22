#include "gamecontrol.h"
#include<QRandomGenerator>
#include<QDebug>
#include<QTimer>
#include"playhand.h"
GameControl::GameControl(QObject *parent)
    : QObject{parent}
{}

void GameControl::playerInit()
{
  //对象实例化
    m_robotLeft=new Robot("机器人A",this);
    m_robotRight=new Robot("机器人B",this);
    m_user=new UserPlayer("玩家1",this);

    //玩家头像显示方位
    m_robotLeft->setDirection(Player::Left);
    m_robotRight->setDirection(Player::Right);
    m_user->setDirection(Player::Right);

    //性别
    Player::Sex sex;
    sex=(Player::Sex)QRandomGenerator::global()->bounded(2);
    m_robotLeft->setSex(sex);
    sex=(Player::Sex)QRandomGenerator::global()->bounded(2);
    m_robotRight->setSex(sex);
    sex=(Player::Sex)QRandomGenerator::global()->bounded(2);
    m_user->setSex(sex);

    //出牌顺序
    m_user->setPrevPlayer(m_robotLeft);
    m_user->setNextPlayer(m_robotRight);

    //左侧机器人
    m_robotLeft->setPrevPlayer(m_robotRight);
    m_robotLeft->setNextPlayer(m_user);

    //右侧机器人
    m_robotRight->setPrevPlayer(m_user);
    m_robotRight->setNextPlayer(m_robotLeft);
    //指定当前玩家
    m_curPlayer=m_user;
    //处理玩家发射的信号
    connect(m_user,&UserPlayer::notifyGrabLordBet,this,&GameControl::onGrabBet);
    connect(m_robotLeft,&UserPlayer::notifyGrabLordBet,this,&GameControl::onGrabBet);
    connect(m_robotRight,&UserPlayer::notifyGrabLordBet,this,&GameControl::onGrabBet);
    //传递出牌玩家对象和玩家打出的牌
    connect(this,&GameControl::pendingInfo,m_robotLeft,&Robot::storePendingInfo);
    connect(this,&GameControl::pendingInfo,m_robotRight,&Robot::storePendingInfo);
    connect(this,&GameControl::pendingInfo,m_user,&Robot::storePendingInfo);
    //处理玩家出牌
    connect(m_robotLeft,&Robot::notifyPlayHand,this,&GameControl::onPlayHand);
    connect(m_robotRight,&Robot::notifyPlayHand,this,&GameControl::onPlayHand);
    connect(m_user,&UserPlayer::notifyPlayHand,this,&GameControl::onPlayHand);
}

Robot *GameControl::getLeftRobot()
{
    return m_robotLeft;
}

Robot *GameControl::getRightRobot()
{
    return m_robotRight;
}

UserPlayer *GameControl::getUserPlayer()
{
    return m_user;
}

void GameControl::setCurrentPlayer(Player *player)
{
    m_curPlayer=player;
}

Player *GameControl::getCurrentPlayer() const
{
    return m_curPlayer;
}

Player *GameControl::getPendPlayer() const
{
    return m_pendPlayer;
}

Cards GameControl::getPendCards() const
{
    return m_pendCards;
}

void GameControl::initAllCards()
{
    m_allCards.clear();//先确保操作的是一个空对象
    for(Card::CardPoint p = static_cast<Card::CardPoint>(Card::Card_Begin + 1);
         p < Card::Card_SJ; p = static_cast<Card::CardPoint>(static_cast<int>(p) + 1)){//枚举类的遍历
        for(Card::CardSuit s = static_cast<Card::CardSuit>(Card::Suit_Begin + 1);
             s < Card::Suit_End; s = static_cast<Card::CardSuit>(static_cast<int>(s) + 1)){
            Card c(p,s);
            m_allCards.add(c);

        }
    }
    m_allCards.add(Card(Card::Card_SJ,Card::Suit_Begin));//小王
    m_allCards.add(Card(Card::Card_BJ,Card::Suit_Begin));//大王

}

Card GameControl::takeOneCard()
{
    return m_allCards.takeRandCard();
}

Cards GameControl::getSurplusCards() const
{
    return m_allCards;
}

void GameControl::resetCardDara()
{
    initAllCards();
    m_robotLeft->clearCards();
    m_robotRight->clearCards();
    m_user->clearCards();
    //初始化出牌玩家和牌
    m_pendPlayer=nullptr;
    m_pendCards.clear();
}

void GameControl::startLordCard()
{
    m_curPlayer->prepareCallLord();
    emit playerStatusChanged(m_curPlayer,ThinkingForCallLord);
}

void GameControl::becomeLord(Player *player,int bet)
{
    m_curBet=bet;
    player->setRole(Player::Lord);
    player->getPrevPlayer()->setRole(Player::Farmer);
    player->getNextPlayer()->setRole(Player::Farmer);
    //设定地主为当前玩家
    m_curPlayer=player;
    player->storeDispatchCard(m_allCards);//把三张底牌给地主
    QTimer::singleShot(1000,this,[=](){
        emit gameStatusChanged(PlayingHand);//整个游戏变为出牌状态
        emit playerStatusChanged(player,ThinkingForPlayHand);//玩家变成出牌状态
        m_curPlayer->preparePlayHand();
        });
}

void GameControl::clearPlayerScore()
{
    m_robotLeft->setScore(0);
    m_robotRight->setScore(0);
    m_user->setScore(0);
}

int GameControl::getPlayerMaxBet()
{
    return m_betRecord.bet;
}

void GameControl::onGrabBet(Player *player, int bet)
{
    //1.通知主界面玩家叫地主了
    if(bet==0||m_betRecord.bet>=bet){
        emit notifyGrabLordBet(player,0,false);//这种情况等于放弃
    }else if(bet>0&&m_betRecord.bet==0){
        //第一个抢地主的玩家
        emit notifyGrabLordBet(player,bet,true);
    }else{
        //后面抢地主的玩家
        emit notifyGrabLordBet(player,bet,false);
    }
    qDebug() << "curent player name: " << player->getName() << ", 下注分数: "
             << bet<< ", m_betRecord.times: " << m_betRecord.times;
    //2.看抢地主的玩家叫的是几分，3分就可以直接结束了
    if(bet==3){
        becomeLord(player,bet);
        //直接成为地主，关于叫分的数据可以清空了
        m_betRecord.reset();
        return;
    }
    //3.不够三分的话对玩家分数进行比较，分数高的是地主
    if(m_betRecord.bet<bet){
        m_betRecord.bet=bet;
        m_betRecord.player=player;
    }
    m_betRecord.times++;
    if(m_betRecord.times==3){
        if(m_betRecord.bet==0){//要重新发牌了
            emit gameStatusChanged(DispatchCard);
        }else{
            becomeLord(m_betRecord.player,m_betRecord.bet);
        }
        m_betRecord.reset();
        return;
    }
    //4.切换玩家，让下一个玩家抢地主
    m_curPlayer=player->getNextPlayer();
    //发送信号给主界面，告知当前状态为抢地主
    emit playerStatusChanged(m_curPlayer,ThinkingForCallLord);
    m_curPlayer->prepareCallLord();
}

void GameControl::onPlayHand(Player *player, Cards &cards)
{
   //1.将玩家出牌的信号转发给主界面
    emit notifyPlayHand(player,cards);
    //2.如果不是空牌，给其他玩家发送信号
    if(!cards.isEmpty()){
        m_pendCards=cards;
        m_pendPlayer=player;
        emit pendingInfo(player,cards);
    }
    //如果有炸弹，底分翻倍
    PlayHand::HandType type=PlayHand(cards).getHandType();
    if(type==PlayHand::Hand_Bomb||type==PlayHand::Hand_Bomb_Jokers){
        m_curBet*=2;
    }
    //3.如果牌出完了，计算本局游戏总分
    if(player->getCards().isEmpty()){
        Player*prev=player->getPrevPlayer();
        Player*next=player->getNextPlayer();
        if(player->getRole()==Player::Lord){
            player->setScore(player->getScore()+2*m_curBet);
            prev->setScore(prev->getScore()-m_curBet);
            next->setScore(next->getScore()-m_curBet);
            player->setWin(true);
            prev->setWin(false);
            next->setWin(false);
        }else{
            player->setWin(true);
            player->setScore(player->getScore()+m_curBet);
            if(prev->getRole()==Player::Lord){
                prev->setScore(prev->getScore()-2*m_curBet);
                next->setScore(next->getScore()+m_curBet);
                prev->setWin(false);
                next->setWin(true);
            }else{
                prev->setScore(prev->getScore()+m_curBet);
                next->setScore(next->getScore()-2*m_curBet);
                prev->setWin(true);
                next->setWin(false);
            }
        }
        emit playerStatusChanged(player,GameControl::Winning);
        return;
    }
    //4.如果牌没有出完，下一个玩家继续出牌
    m_curPlayer=player->getNextPlayer();
    m_curPlayer->preparePlayHand();
    emit playerStatusChanged(m_curPlayer,GameControl::ThinkingForPlayHand);

}





