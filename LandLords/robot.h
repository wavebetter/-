#ifndef ROBOT_H
#define ROBOT_H

#include <QObject>
#include "player.h"

class Robot : public Player
{
    Q_OBJECT
public:
    using Player::Player;//继承父类的构造函数
    explicit Robot(QObject *parent = nullptr);

    void prepareCallLord() override;
    void preparePlayHand() override;
    void thingkingCallLord()override;//考虑叫地主
    void thinkPlayHand()override;
};

#endif // ROBOT_H
