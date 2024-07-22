#include "robotgraplord.h"

RobotGrapLord::RobotGrapLord(Player*player,QObject *parent)
    : QThread{parent}
{
    m_player=player;
}

RobotGrapLord::~RobotGrapLord()
{

}

void RobotGrapLord::run()
{
    msleep(1000);
    m_player->thingkingCallLord();
}
