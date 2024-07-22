#ifndef GAMECONTROL_H
#define GAMECONTROL_H

#include <QObject>
#include "robot.h"
#include "userplayer.h"
#include  "cards.h"
struct BetRecord{
    BetRecord(){
        reset();
    }
    void reset(){
        player=nullptr;
        bet=0;
        times=0;
    }
    Player*player;
    int bet;
    int times;//记录一下这是第几次叫地主
};

class GameControl : public QObject
{
    Q_OBJECT
public:
    explicit GameControl(QObject *parent = nullptr);
    //游戏状态三种：发牌 叫地主 出牌
    enum GameStatus{
        DispatchCard,
        CallingLord,
        PlayingHand
    };
    //玩家状态三种 考虑叫地主 考虑出牌 胜利
    enum PlayerStatus{
        ThinkingForCallLord,
        ThinkingForPlayHand,
        Winning
    };
    //初始化玩家
    void playerInit();
    //得到玩家的实例对象
    Robot* getLeftRobot();
    Robot* getRightRobot();
    UserPlayer* getUserPlayer();

    void setCurrentPlayer(Player* player);
    Player* getCurrentPlayer()const;
    //得到出牌玩家和打出的牌
    Player* getPendPlayer() const;
    Cards getPendCards() const;

    //初始化扑克牌
    void initAllCards();
    //每次发一张牌
    Card takeOneCard();
    //得到最后三张底牌
    Cards getSurplusCards() const;
    //重置卡牌数据
    void resetCardDara();
    //准备叫地主
    void startLordCard();
    //成为地主
    void becomeLord(Player *player,int bet);
    //清空所有玩家得分
    void clearPlayerScore();
    //得到玩家下注的最高分数
    int getPlayerMaxBet();
    //处理叫地主
    void onGrabBet(Player*player,int bet);
    //处理出牌
    void onPlayHand(Player*player,Cards &cards);

signals:
    void playerStatusChanged(Player* player,PlayerStatus status);
    //通知已经叫地主下注
    void notifyGrabLordBet(Player*player,int bet,bool flag);
    void gameStatusChanged(GameStatus status);
    //通知已经出牌
    void notifyPlayHand(Player*player,Cards&cards);
    void pendingInfo(Player*player,Cards&cards);

private:
    //对应着三个玩家
    Robot* m_robotLeft;
    Robot* m_robotRight;
    UserPlayer *m_user;
    Player* m_curPlayer;
    Player* m_pendPlayer;
    Cards m_pendCards;
    Cards m_allCards;
    BetRecord m_betRecord;
    int m_curBet;
};

#endif // GAMECONTROL_H
