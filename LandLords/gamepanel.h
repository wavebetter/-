#ifndef GAMEPANEL_H
#define GAMEPANEL_H

#include <QMainWindow>
#include "gamecontrol.h"
#include "cardpanel.h"
#include <QLabel>
#include<QMap>
#include<QTimer>
#include"animationwindow.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class GamePanel;
}
QT_END_NAMESPACE

class GamePanel : public QMainWindow
{
    Q_OBJECT

public:
    GamePanel(QWidget *parent = nullptr);
    enum AnimationType{SeqSingle,SeqPair,Plane,JokerBomb,Bomb,Bet};
    void GameControlInit();
    //更新面板分数
    void updatePlayerScore();
    //切割并存储图片（扑克牌）
    void initCardMap();
    //裁剪图片
    void cropImage(QPixmap &pix,int x,int y,Card& c);
    //初始化游戏按钮
    void initButtonsGroup();
    //初始化窗口中的各种位置
    void initPlayerContext();
    //初始化游戏场景
    void initGameScene();
    //处理游戏状态
    void gameStatusPrecess(GameControl::GameStatus status);
    //发牌
    void startDispatchCard();
    //加载玩家头像
    QPixmap loadRoleImage(Player::Sex sex,Player::Direction direct,Player::Role role);
    //定时器处理动作
    void onDispatchCard();
    //处理牌得移动
    void cardMoveStep(Player* player,int curPos);
    //处理分发得到的扑克牌
    void disposCard(Player*player,Cards&cards);
    //更新扑克牌在窗口中的显示
    void updatePlayerCards(Player*player);
    //处理玩家状态的变化
    void onPlayerStatusChanged(Player*player,GameControl::PlayerStatus status);
    //处理玩家抢地主
    void onGrabBet(Player* player,int bet,bool flag);
    //显示各种动画
    void showAnimation(AnimationType type,int bet=0);
    //处理玩家的出牌
    void onDisposePlayHand(Player *player, Cards &cards);
    //处理用户玩家出牌
    void onUserPlayHand();
    //处理玩家选牌
    void onCardSelected(Qt::MouseButton button);
    void hidePlayerDropCards(Player *player);
    void onUserPass();
    //显示玩家最终得分
    void showEndingScorepanel();
    ~GamePanel();
protected:
    void paintEvent(QPaintEvent *ev);
    void mouseMoveEvent(QMouseEvent*ev);

private:
    enum CardAlign{Horizontal,Vertical};
    struct PlayerContext{
        //1.玩家扑克牌显示的区域
        QRect cardRect;
        //2.出牌的区域
        QRect playHandRect;
        //3.扑克牌的对齐方式
        CardAlign align;
        //4.显示的是背面还是正面
        bool isFrontSide;
        //5.提示信息 如不要
        QLabel* info;
        //6.玩家的头像
        QLabel* roleImg;
        //7.玩家打出的牌
        Cards lastCards;

    };
    Ui::GamePanel *ui;
    QPixmap m_bkImage;
    GameControl *m_gameCtl;
    QVector<Player*> m_playerList;
    QMap<Card,CardPanel*>m_cardMap;//一个卡牌信息对应一个卡牌窗口，这个得重载一下<运算符
    QSize m_cardSize;
    QPixmap m_cardBackImg;
    QMap<Player*,PlayerContext>m_contextMap;//指针类型可以直接比较
    CardPanel* m_baseCard;
    CardPanel* m_moveCard;
    QVector<CardPanel*> m_last3Card;
    QPoint m_baseCardPos;
    GameControl::GameStatus m_gameStatus;
    QTimer* m_timer;
    AnimationWindow*m_animation;
    CardPanel *m_curSelCard;
    QSet<CardPanel*>m_selectCards;
    QRect m_cardsRect;
    QHash<CardPanel*,QRect> m_userCards;
};
#endif // GAMEPANEL_H
