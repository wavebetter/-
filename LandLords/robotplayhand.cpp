#include "robotplayhand.h"
#include<QDebug>
RobotPlayHand::RobotPlayHand(Player*player,QObject *parent)
    : QThread{parent}
{
    m_player=player;
}

void RobotPlayHand::run()
{
    msleep(1000);
    m_player->thinkPlayHand();
}
