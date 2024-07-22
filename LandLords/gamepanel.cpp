#include "gamepanel.h"
#include "ui_gamepanel.h"
#include<QPainter>
#include<QRandomGenerator>
#include<QDebug>
#include<QRect>
#include<QMouseEvent>
#include"PlayHand.h"
#include"endingpanel.h"
#include <QPropertyAnimation>

GamePanel::GamePanel(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GamePanel)
{
    ui->setupUi(this);

    //1.背景图
    int num=QRandomGenerator::global()->bounded(10);//随机挑一张背景图
    QString path=QString(":/images/background-%1.png").arg(num+1);
    m_bkImage.load(path);
    //2.窗口标题的大小
    this->setWindowTitle("PP斗地主");
    this->setFixedSize(1000,650);
    //3.实例化游戏控制类对象
    GameControlInit();
    //4.更新玩家得分
    updatePlayerScore();
    //5.切割游戏图片
    initCardMap();
    //6.初始化游戏按钮
    initButtonsGroup();
    //7.初始化窗口中的各种位置
    initPlayerContext();
    //8.扑克牌场景初始化
    initGameScene();
    //定时器实例化
    m_timer=new QTimer(this);
    connect(m_timer,&QTimer::timeout,this,&GamePanel::onDispatchCard);
    m_animation=new AnimationWindow(this);
}

void GamePanel::GameControlInit()
{
    m_gameCtl=new GameControl(this);
    m_gameCtl->playerInit();
    //得到三个玩家的实例
    Robot* leftRobot=m_gameCtl->getLeftRobot();
    Robot* rightRobot=m_gameCtl->getRightRobot();
    UserPlayer* user=m_gameCtl->getUserPlayer();
    //存储的顺序：左侧机器人 右侧机器人 当前玩家
    m_playerList<<leftRobot<<rightRobot<<user;

    connect(m_gameCtl,&GameControl::playerStatusChanged,this,&GamePanel::onPlayerStatusChanged);
    connect(m_gameCtl,&GameControl::notifyGrabLordBet,this,&GamePanel::onGrabBet);
    connect(m_gameCtl,&GameControl::gameStatusChanged,this,&GamePanel::gameStatusPrecess);
    connect(m_gameCtl,&GameControl::notifyPlayHand,this,&GamePanel::onDisposePlayHand);

    connect(leftRobot,&Player::notifyPickCards,this,&GamePanel::disposCard);
    connect(rightRobot,&Player::notifyPickCards,this,&GamePanel::disposCard);
    connect(user,&Player::notifyPickCards,this,&GamePanel::disposCard);
}

void GamePanel::updatePlayerScore()
{
    ui->scorePanel->setScores(m_playerList[0]->getScore()
                              ,m_playerList[1]->getScore(),m_playerList[2]->getScore());
}

void GamePanel::initCardMap()
{
    //加载大图
    QPixmap pixmap(":/images/card.png");

    //计算每张图片大小
    m_cardSize.setWidth(pixmap.width()/13);
    m_cardSize.setHeight(pixmap.height()/5);
    //有三类图片 背景图 正常花色 大小王
    m_cardBackImg=pixmap.copy(2*m_cardSize.width(),4*m_cardSize.height()
                                ,m_cardSize.width(),m_cardSize.height());//背景图
    for(int i=0,suit =Card::Suit_Begin+1;suit<Card::Suit_End;++suit,++i){
        for(int j=0,point=Card::Card_Begin+1;point<Card::Card_SJ;++point,++j){
            Card card((Card::CardPoint)point,(Card::CardSuit)suit);
            //裁剪图片
            cropImage(pixmap,j*m_cardSize.width(),i*m_cardSize.height(),card);//有花色的牌
        }
    }
    Card c;
    c.setPoint(Card::Card_SJ);
    c.setSuit(Card::Suit_Begin);
    cropImage(pixmap,0,4*m_cardSize.height(),c);

    c.setPoint(Card::Card_BJ);
    c.setSuit(Card::Suit_Begin);
    cropImage(pixmap,m_cardSize.width(),4*m_cardSize.height(),c);//大王

}

void GamePanel::cropImage(QPixmap &pix, int x, int y,Card&c)
{
    QPixmap sub=pix.copy(x,y,m_cardSize.width(),m_cardSize.height());
    CardPanel*panel =new CardPanel(this);
    panel->setImage(sub,m_cardBackImg);
    panel->setCard(c);
    panel->hide();
    m_cardMap.insert(c,panel);
    connect(panel,&CardPanel::cardsSelectde,this,&GamePanel::onCardSelected);
}

void GamePanel::initButtonsGroup()
{
    ui->btnGroup->intiButtons();
    ui->btnGroup->selectPanel(ButtonGroup::Start);
    connect(ui->btnGroup,&ButtonGroup::startGame,this,[=](){
        //界面的初始化
        ui->btnGroup->selectPanel(ButtonGroup::Empty);
        m_gameCtl->clearPlayerScore();
        updatePlayerScore();
        //修改游戏状态为发牌
        gameStatusPrecess(GameControl::DispatchCard);
    });
    connect(ui->btnGroup,&ButtonGroup::playHand,this,&GamePanel::onUserPlayHand);
    connect(ui->btnGroup,&ButtonGroup::pass,this,&GamePanel::onUserPass);
    connect(ui->btnGroup,&ButtonGroup::betPoint,this,[=](int bet){
        m_gameCtl->getUserPlayer()->grabLordBet(bet);
        ui->btnGroup->selectPanel(ButtonGroup::Empty);
    });


}

void GamePanel::initPlayerContext()
{
    //1.放置扑克牌的区域
    QRect cardsRect[]={                                //左侧机器人
        QRect(90,130,100,height()-200),                //右侧机器人
        QRect(rect().right()-190,130,100,height() - 200),         //当前玩家
        QRect(250,rect().bottom()-120,width()-500,100)
    };
    //2.玩家出牌的区域
    QRect playerHandRect[]={                                //左侧机器人
        QRect(260,150,100,100),                //右侧机器人
        QRect(rect().right()-360,150,100,100),         //当前玩家
        QRect(150,rect().bottom()-290,width()-300,105)
    };
    //3.玩家头像显示的位置
    QPoint roleImgPos[]={
        QPoint(cardsRect[0].left()-80,cardsRect[0].height()/2+20),                //右侧机器人
        QPoint(cardsRect[1].right()+10,cardsRect[1].height()/2+20),         //左侧机器人
        QPoint(cardsRect[2].right()-10,cardsRect[2].top()-10)              //当前玩家
    };
    //循环把区域初始化给玩家
    for(int i=0;i<m_playerList.size();++i){
        int index=m_playerList.indexOf(m_gameCtl->getUserPlayer());
        PlayerContext context;
        context.align=i==index?Horizontal:Vertical;
        context.isFrontSide=i==index?true:false;
        context.cardRect=cardsRect[i];
        context.playHandRect=playerHandRect[i];
        //提示信息
        context.info=new QLabel(this);
        context.info->resize(160,98);
        context.info->hide();
        //显示到出牌区域的中心位置
        QRect rect=playerHandRect[i];
        QPoint pt(rect.left()+(rect.width()-context.info->width())/2,
                  rect.top()+(rect.height()-context.info->height())/2);
        context.info->move(pt);
        //玩家的头像
        context.roleImg=new QLabel(this);
        context.roleImg->resize(84,120);
        context.roleImg->hide();
        context.roleImg->move(roleImgPos[i]);
        m_contextMap.insert(m_playerList.at(i),context);
    }
}

void GamePanel::initGameScene()
{
        //1.发牌区得扑克牌
        m_baseCard=new CardPanel(this);
        m_baseCard->setImage(m_cardBackImg,m_cardBackImg);
        //2.发牌过程中移动得扑克牌
        m_moveCard=new CardPanel(this);
        m_moveCard->setImage(m_cardBackImg,m_cardBackImg);
        //3.最后三张底牌
        for(int i=0;i<3;++i){
            CardPanel* panel=new CardPanel(this);
            panel->setImage(m_cardBackImg,m_cardBackImg);
            m_last3Card.push_back(panel);
            panel->hide();
        }
        //扑克牌得位置
        m_baseCardPos=QPoint((width() - m_cardSize.width()) / 2,
                               height() / 2 - 100);
        m_baseCard->move(m_baseCardPos);
        m_moveCard->move(m_baseCardPos);

        int base=(width()-3*m_cardSize.width()-2*10)/2;
        for(int i=0;i<3;++i){
            m_last3Card[i]->move(base+(m_cardSize.width()+10)*i,20);
        }
}

void GamePanel::gameStatusPrecess(GameControl::GameStatus status)
{
    //记录游戏状态
    m_gameStatus=status;
    //处理游戏状态
    switch (status) {
    case GameControl::DispatchCard:
        startDispatchCard();
        break;
    case GameControl::CallingLord:
    {
        //取出底牌数据
        CardList last3Card=m_gameCtl->getSurplusCards().toCardList();
        //给底牌窗口设置图片
        for(int i=0;i<last3Card.size();++i){
            QPixmap front=m_cardMap[last3Card.at(i)]->getImage();
            m_last3Card[i]->setImage(front,m_cardBackImg);
            m_last3Card[i]->hide();
        }
        //开始叫地主
         m_gameCtl->startLordCard();
        break;
    }
    case GameControl::PlayingHand:
        //隐藏发牌区的底牌和移动的牌
        m_baseCard->hide();
        m_moveCard->hide();
        //显示三张底牌
        for(int i=0;i<m_last3Card.size();++i){
            m_last3Card.at(i)->show();
        }

        for(int i=0;i<m_playerList.size();++i){
            PlayerContext &context=m_contextMap[m_playerList.at(i)];
            //隐藏提示信息
            context.info->hide();
            //显示玩家头像
            Player* player=m_playerList.at(i);
            QPixmap pixmap=loadRoleImage(player->getSex()
                                        ,player->getDirection(),player->getRole());
            context.roleImg->setPixmap(pixmap);
            context.roleImg->show();
        }
        break;
    default:
        break;
    }
}

void GamePanel::startDispatchCard()
{
    //初始化每张卡牌
    for(auto it=m_cardMap.begin();it!=m_cardMap.end();++it){
        it.value()->setSelected(false);
        it.value()->setFrontSize(true);
        it.value()->hide();
    }
    //隐藏三张底牌
    for(int i=0;i<m_last3Card.size();++i){
        m_last3Card.at(i)->hide();
    }
    //重置玩家窗口上下文信息
    int index=m_playerList.indexOf(m_gameCtl->getUserPlayer());
    for(int i=0;i<m_playerList.size();++i){
        m_contextMap[m_playerList.at(i)].lastCards.clear();
        m_contextMap[m_playerList.at(i)].info->hide();
        m_contextMap[m_playerList.at(i)].roleImg->hide();
        m_contextMap[m_playerList.at(i)].isFrontSide=i==index?true:false;//机器人需要显示背面
    }
    //重置所有玩家卡牌数据
    m_gameCtl->resetCardDara();
    //显示底牌
    m_baseCard->show();
    //隐藏按钮面板
    ui->btnGroup->selectPanel(ButtonGroup::Empty);
    //启动定时器
    m_timer->start(15);//每隔15ms触发一次
    //播放背景音乐
}

QPixmap GamePanel::loadRoleImage(Player::Sex sex, Player::Direction direct, Player::Role role)
{
    //找图片
    QVector<QString> lordMan;
    QVector<QString> lordWoman;
    QVector<QString> farmerMan;
    QVector<QString> farmerWoman;
    lordMan<<":/images/lord_man_1.png"<<":/images/lord_man_2.png";
    lordWoman<<":/images/lord_woman_1.png"<<":/images/lord_woman_2.png";
    farmerMan<<":/images/farmer_man_1.png"<<":/images/farmer_man_2.png";
    farmerWoman<<":/images/farmer_woman_1.png"<<":/images/farmer_woman_2.png";
    //加载图片
    QImage image;
    int random=QRandomGenerator::global()->bounded(2);
    if(sex==Player::Man&&role==Player::Lord){
        image.load(lordMan.at(random));
    }else if(sex==Player::Man&&role==Player::Farmer){
        image.load(farmerMan.at(random));
    }else if(sex==Player::Woman&&role==Player::Lord){
        image.load(lordWoman.at(random));
    }else if(sex==Player::Woman&&role==Player::Farmer){
        image.load(farmerWoman.at(random));
    }
    QPixmap pixmap;
    if(direct==Player::Left){
        pixmap=QPixmap::fromImage(image);
    }else{
        pixmap=QPixmap::fromImage(image.mirrored(true,false));
    }
    return pixmap;
}

void GamePanel::onDispatchCard()
{
    //记录扑克牌得位置
    static int curMovePos=0;
    //当前玩家
    Player* curPlayer=m_gameCtl->getCurrentPlayer();
    if(curMovePos>=100){
        //给玩家发一张牌
        Card card=m_gameCtl->takeOneCard();
        curPlayer->storeDispatchCard(card);
        Cards cs(card);
        //disposCard(curPlayer,cs);
        //切换玩家
        m_gameCtl->setCurrentPlayer(curPlayer->getNextPlayer());
        curMovePos=0;
        //发牌动画
        cardMoveStep(curPlayer,curMovePos);
        //判断牌是否发完了
        if(m_gameCtl->getSurplusCards().CardCount()==3){
            //终止定时器
            m_timer->stop();
            //切换游戏状态
            gameStatusPrecess(GameControl::CallingLord);
            return;
        }
    }
    //移动扑克牌
    cardMoveStep(curPlayer,curMovePos);
    curMovePos+=15;
}

void GamePanel::cardMoveStep(Player* player,int curPos)
{
    //得到每个玩家得扑克牌展示区域
    QRect cardRect=m_contextMap[player].cardRect;
    //每个玩家得单元步长
    int unit[]={
        (m_baseCardPos.x()-cardRect.right())/100,
        (cardRect.left()-m_baseCardPos.x())/100,
        (cardRect.top()-m_baseCardPos.y())/100
    };
    //每次窗口移动得时候每个玩家对应得牌得实时位置
    QPoint pos[]={
        QPoint(m_baseCardPos.x()-curPos*unit[0],m_baseCardPos.y()),
        QPoint(m_baseCardPos.x()+curPos*unit[1],m_baseCardPos.y()),
        QPoint(m_baseCardPos.x(),m_baseCardPos.y()+curPos*unit[2]),
    };
    //移动扑克牌
    int index=m_playerList.indexOf(player);
    m_moveCard->move(pos[index]);
    //临界状态处理（走到终点之后要隐藏起来）
    if(curPos==0){
        m_moveCard->show();
    }
    if(curPos==100){
        m_moveCard->hide();
    }

}

void GamePanel::disposCard(Player *player, Cards &cards)
{
    CardList list=cards.toCardList();
    for(int i=0;i<list.size();++i){
        CardPanel*panel=m_cardMap[list.at(i)];
        panel->setOwner(player);
    }
    //更新扑克牌的显示
    updatePlayerCards(player);
}

void GamePanel::updatePlayerCards(Player *player)
{
    Cards cards=player->getCards();
    CardList list=cards.toCardList();
    m_cardsRect=QRect();
    m_userCards.clear();
    //取出展示扑克牌的区域
    int cardSpace=20;
    QRect cardsRect =m_contextMap[player].cardRect;
    for(int i=0;i<list.size();++i){
        CardPanel*panel =m_cardMap[list.at(i)];
        panel->show();
        panel->raise();//按顺序显示
        panel->setFrontSize(m_contextMap[player].isFrontSide);
        //水平还是垂直
        if(m_contextMap[player].align==Horizontal){
            int leftX=cardsRect.left()+(cardsRect.width()-(list.size()-1)*cardSpace-panel->width())/2;//这是手牌显示的最左端位置
            int topY=cardsRect.top()+(cardsRect.height()-m_cardSize.height())/2;
            if(panel->isSelected()){
                topY-=10;
            }
            panel->move(leftX+cardSpace*i,topY);
            m_cardsRect=QRect(leftX,topY,cardSpace*i+m_cardSize.width()
                                ,m_cardSize.height());
            int curWidth=0;
            if(list.size()-1==i){
                curWidth=m_cardSize.width();
            }else{
                curWidth=cardSpace;
            }
            QRect cardRect(leftX+cardSpace*i,topY,curWidth
                           ,m_cardSize.height());
            m_userCards.insert(panel,cardRect);
        }else{
            int leftX=cardsRect.left()+(cardsRect.width()-m_cardSize.width())/2;
            int topY=cardsRect.top()+(cardsRect.height()-(list.size()-1)*cardSpace-panel->height())/2;
            panel->move(leftX,topY+i*cardSpace);
        }
    }
    //显示玩家打出的牌
    QRect playCardRect=m_contextMap[player].playHandRect;
    Cards lastCards=m_contextMap[player].lastCards;
    if(!lastCards.isEmpty()){
        int playSpacing=24;
        CardList lastCardList=lastCards.toCardList();
        CardList::ConstIterator itplayed=lastCardList.constBegin();
        for(int i=0;itplayed!=lastCardList.constEnd();++itplayed,i++){
            CardPanel*panel=m_cardMap[*itplayed];
            panel->setFrontSize(true);
            panel->raise();
            //将打出的牌移动到出牌区域
            if(m_contextMap[player].align==Horizontal){
                int leftBase=playCardRect.left()+
                               (playCardRect.width()-(lastCardList.size()-1)*playSpacing-panel->width())/2;
                int top=playCardRect.top()+(playCardRect.height()-panel->height())/2;
                panel->move(leftBase+i*playSpacing,top);
            }else{
                int left=playCardRect.left()+
                               (playCardRect.width()-panel->width())/2;
                int top=playCardRect.top();
                panel->move(left,top+i*playSpacing);
            }
            panel->show();
        }
    }
}

void GamePanel::onPlayerStatusChanged(Player *player, GameControl::PlayerStatus status)
{
    switch(status){
    case GameControl::ThinkingForCallLord:
        if(player==m_gameCtl->getUserPlayer()){
            ui->btnGroup->selectPanel(ButtonGroup::CallLord,m_gameCtl->getPlayerMaxBet());
        }
        break;
    case GameControl::ThinkingForPlayHand:
        //隐藏上一轮打出的牌
        hidePlayerDropCards(player);
        if(player==m_gameCtl->getUserPlayer()){
            //取出出牌玩家的对象
            Player* pendPlayer=m_gameCtl->getPendPlayer();
            if(pendPlayer==m_gameCtl->getUserPlayer()||pendPlayer==nullptr){
                ui->btnGroup->selectPanel(ButtonGroup::PlayCard);
            }else{
                ui->btnGroup->selectPanel(ButtonGroup::PassOrPlay);
            }
        }else{
            ui->btnGroup->selectPanel(ButtonGroup::Empty);
        }
        break;
    case GameControl::Winning:
        m_contextMap[m_gameCtl->getLeftRobot()].isFrontSide=true;
        m_contextMap[m_gameCtl->getRightRobot()].isFrontSide=true;
        updatePlayerCards(m_gameCtl->getLeftRobot());
        updatePlayerCards(m_gameCtl->getRightRobot());
        updatePlayerScore();
        m_gameCtl->setCurrentPlayer(player);
        showEndingScorepanel();
        break;
    default:
        break;
    }
}

 GamePanel::~GamePanel()
 {
        delete ui;
 }

 void GamePanel::paintEvent(QPaintEvent *ev)
 {
     Q_UNUSED(ev)
     QPainter p(this);
     p.drawPixmap(rect(), m_bkImage);
 }

 void GamePanel::mouseMoveEvent(QMouseEvent *ev)
 {
     if(ev->buttons()&Qt::LeftButton){
         QPoint pt=ev->pos();
         if(!m_cardsRect.contains(pt)){
             m_curSelCard=nullptr;
         }else{
             QList<CardPanel*>list=m_userCards.keys();
             for(int i=0;i<list.size();++i){
                 CardPanel* panel=list.at(i);
                 if(m_userCards[panel].contains(pt)&&m_curSelCard!=panel){
                     //点击这张扑克牌
                     panel->clicked();
                     m_curSelCard=panel;

                 }
             }
         }
     }
 }
 void GamePanel::onGrabBet(Player *player, int bet, bool flag)
 {
     //显示抢地主的信息提示
     PlayerContext context=m_contextMap[player];
     if(bet==0){
         QPixmap pixmap(":/images/buqinag.png");
         context.info->setPixmap(pixmap);
     }else{
         if(flag){
             QPixmap pixmap(":/images/jiaodizhu.png");
             context.info->setPixmap(pixmap);
         }else{
             QPixmap pixmap(":/images/qiangdizhu.png");
             context.info->setPixmap(pixmap);
         }
     }
     context.info->show();
     //显示叫地主的分数
     showAnimation(Bet,bet);
     //播放相应的背景音乐
 }

 void GamePanel::showAnimation(AnimationType type, int bet)
 {
     switch(type){
     case AnimationType::SeqSingle:
         break;
     case AnimationType::SeqPair:
         break;
     case AnimationType::Plane:
         break;
     case AnimationType::JokerBomb:
         break;
     case AnimationType::Bomb:
         break;
     case AnimationType::Bet:
         m_animation->setFixedSize(160,98);
         m_animation->move((width()-m_animation->width())/2,(height()-m_animation->height())/2-140);
         m_animation->showBetScore(bet);
         break;
     }
     m_animation->show();
 }

 void GamePanel::onDisposePlayHand(Player *player, Cards &cards)
 {
     //存储玩家打出的牌
     auto it=m_contextMap.find(player);
     it->lastCards=cards;
     //2.根据牌型播放游戏特效
     PlayHand hand(cards);
     PlayHand::HandType type=hand.getHandType();
     if(type==PlayHand::Hand_Plane||type==PlayHand::Hand_Plane_Two_Pair
         ||type==PlayHand::Hand_Plane_Two_Single){
         showAnimation(Plane);
     }else if(type==PlayHand::Hand_Seq_Single){
         showAnimation(SeqSingle);
     }else if(type==PlayHand::Hand_Seq_Pair){
         showAnimation(SeqPair);
     }else if(type==PlayHand::Hand_Bomb){
         showAnimation(Bomb);
     }else if(type==PlayHand::Hand_Bomb_Jokers){
         showAnimation(JokerBomb);
     }
     //如果玩家打出的是空牌（不出牌）
     if(cards.isEmpty()){
         it->info->setPixmap(QPixmap(":/images/pass.png"));
         it->info->show();
     }
     //3.更新玩家剩余的牌
     updatePlayerCards(player);
     //4.播放提示音乐
 }

 void GamePanel::onUserPlayHand()
 {
   //判断游戏状态
     if(m_gameStatus!=GameControl::PlayingHand){
       qDebug()<<"游戏状态不是出牌站状态";
       return;
   }
   //判断玩家是不是用户玩家
   if(m_gameCtl->getCurrentPlayer()!=m_gameCtl->getUserPlayer()){
       qDebug()<<"不是用户玩家";
       return ;
   }
   //判断出的牌是否为空
   if(m_selectCards.isEmpty()){
       qDebug()<<"出的牌为空";
       return;
   }
   //得到要打出的牌型
   Cards cs;
   for(auto it=m_selectCards.begin();it!=m_selectCards.end();++it){
       Card card=(*it)->getCard();
       cs.add(card);
   }
   PlayHand hand(cs);
   PlayHand::HandType type=hand.getHandType();
   if(type==PlayHand::Hand_Unknown){
       qDebug()<<"牌型非法";
       return;
   }
   //判断当前玩家能不能压住上一家
   qDebug()<<"getPendPlayer()="<<m_gameCtl->getPendPlayer();
   qDebug()<<"getUserPlayer()="<<m_gameCtl->getUserPlayer();
   if(m_gameCtl->getPendPlayer()!=nullptr&&m_gameCtl->getPendPlayer()!=m_gameCtl->getUserPlayer()){
       Cards cards=m_gameCtl->getPendCards();
       if(!hand.canBeat(PlayHand(cards))){
           qDebug()<<"管不上上家";
           return;
       }
   }
   //通过玩家对象出牌
   qDebug()<<"准备打出牌";
   m_gameCtl->getUserPlayer()->playHand(cs);
   //清空容器
   m_selectCards.clear();
 }

 void GamePanel::onCardSelected(Qt::MouseButton button)
 {
     //1.判断游戏状态是不是出牌状态
     if(m_gameStatus==GameControl::DispatchCard||
         m_gameStatus==GameControl::CallingLord){
         return;
     }
     //2.判断发出信号的牌的所有者
     CardPanel*panel=(CardPanel*)sender();
     if(panel->getOwner()!=m_gameCtl->getUserPlayer()){
         return;
     }
     //3.保存当前被选中的牌的窗口对象
     m_curSelCard=panel;
     //4.判断参数的鼠标键是左键还是右键
     if(button==Qt::LeftButton){
         //设置扑克牌的选中状态
         panel->setSelected(!panel->isSelected());
         //更新扑克牌在窗口中的显示
         updatePlayerCards(panel->getOwner());
         //保存或删除扑克牌窗口对象
         QSet<CardPanel*>::const_iterator it=m_selectCards.find(panel);
         if(it==m_selectCards.constEnd()){
             m_selectCards.insert(panel);
         }else{
             m_selectCards.erase(it);
         }
     }else if(button==Qt::RightButton){
        //调用出牌按钮的槽函数
         onUserPlayHand();
     }
 }

 void GamePanel::hidePlayerDropCards(Player *player)
 {
     auto it=m_contextMap.find(player);
     if(it!=m_contextMap.end()){
          if(it->lastCards.isEmpty()){
             it->info->hide();
          }else{
              //Cards->card
              CardList list=it->lastCards.toCardList();
              for(auto last=list.begin();last!=list.end();++last){
                  m_cardMap[*last]->hide();
              }
          }
      }
     it->lastCards.clear();
 }

 void GamePanel::onUserPass()
 {
  //判断是不是用户玩家
     Player*curPlayer=m_gameCtl->getCurrentPlayer();
  Player*userPlayer=m_gameCtl->getUserPlayer();
     if(curPlayer!=userPlayer){
      return;
  }
  //判断当前用户玩家是不是上一次出牌的玩家
  Player*pendPlayer=m_gameCtl->getPendPlayer();
  if(pendPlayer==userPlayer||pendPlayer==nullptr){
      return;
  }
  //打出一个空的Cards对象
  Cards empty;
  userPlayer->playHand(empty);
  //清空用户选择的牌
  for(auto it=m_selectCards.begin();it!=m_selectCards.end();++it){
      (*it)->setSelected(false);
  }
  m_selectCards.clear();
      //更新待出牌区域的牌
  updatePlayerCards(userPlayer);
 }

 void GamePanel::showEndingScorepanel()
 {
         bool isLord=m_gameCtl->getUserPlayer()->getRole()==Player::Lord?true:false;
         bool isWin=m_gameCtl->getUserPlayer()->isWin();
         EndingPanel*panel=new EndingPanel(isLord,isWin,this);
         panel->show();
         panel->move((width() - panel->width()) / 2, -panel->height());
         panel->setPlayerScore(m_gameCtl->getLeftRobot()->getScore(),
                               m_gameCtl->getRightRobot()->getScore(),
                               m_gameCtl->getUserPlayer()->getScore());
     QPropertyAnimation *animation=new QPropertyAnimation(panel,"geometry",this);
     //动画持续时间
     animation->setDuration(1500);
     //设置窗口的起始和终止位置
     animation->setStartValue(QRect(panel->x(), panel->y(), panel->width(), panel->height()));
     animation->setEndValue(QRect((width() - panel->width()) / 2, (height() - panel->height()) / 2,
                                  panel->width(), panel->height()));
     //设置窗口运动曲线
     animation->setEasingCurve(QEasingCurve(QEasingCurve::OutBounce));
     //播放动画效果
     animation->start();
     //处理窗口信号
     connect(panel, &EndingPanel::continueGame, this, [=]()
             {
                 panel->close();
                 panel->deleteLater();
                 animation->deleteLater();
                 ui->btnGroup->selectPanel(ButtonGroup::Empty);
                 gameStatusPrecess(GameControl::DispatchCard);
                 //m_bgm->startBGM(80);
             });
 }
