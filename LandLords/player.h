#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include"cards.h"

class Player : public QObject
{
    Q_OBJECT
public:
    //角色 性别 头像方位 玩家类型
    enum Role{Lord,Farmer};
    enum Sex{Man,Woman};
    enum Direction{Left,Right};
    enum Type{Robot,User,UnKnow};
    //构造函数
    explicit Player(QObject *parent = nullptr);
    explicit Player(QString name, QObject *parent = nullptr);

    void setName(QString name);
    QString getName() const;
    void setRole(Role role);
    Role getRole() const;
    void setSex(Sex sex);
    Sex getSex() const;
    void setDirection(Direction direction);
    Direction getDirection() const;
    void setType(Type type);
    Type getType()const;

    void setScore(int score);
    int getScore() const;

    void setWin(bool flag);
    bool isWin() const;
    //提供当前对象的上家/下家
    void setPrevPlayer(Player* player);
    void setNextPlayer(Player* player);
    Player* getPrevPlayer() const;
    Player* getNextPlayer() const;

    //叫地主/抢地主 通过叫分来实现
    void grabLordBet(int point);
    //存储自己的扑克牌
    void storeDispatchCard(Card& card);
    void storeDispatchCard(Cards& cards);
    //得到所有牌
    Cards getCards() const;
    //清空所有手牌
    void clearCards();
    //出牌,出的牌待下家处理
    void playHand(Cards& cards);
    //设置出牌的玩家已经待处理的扑克牌
    //void setPendingInfo(Player*player,Cards&cards);
    Player* getPendPlayer() const;
    Cards getPendCards() const;
    //存储出牌的玩家和其打出的牌
    void storePendingInfo(Player*player,Cards&cards);
    //虚函数
    virtual void prepareCallLord();
    virtual void preparePlayHand();
    virtual void thingkingCallLord();
    virtual void thinkPlayHand();
signals:
    //通知已经叫地主下注
    void notifyGrabLordBet(Player* player,int bet);
    //通知已经出牌
    void notifyPlayHand(Player*player,Cards&cards);
    //通知已经发牌了
    void notifyPickCards(Player*player,Cards&cards);
protected:
    int m_score;
    QString m_name;
    Role m_role;
    Sex m_sex;
    Direction m_direction;
    Type m_type;
    bool m_isWin=false;
    Player* m_prev=nullptr;
    Player* m_next=nullptr;
    Cards m_cards;//放自己手里的扑克牌
    Cards m_pendCards;//当前出的牌
    Player* m_pendPlayer=nullptr;//这是谁出的牌
};

#endif // PLAYER_H
