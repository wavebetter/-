#include "gamepanel.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 注册自定义类型
    qRegisterMetaType<Cards>("Cards");
    qRegisterMetaType<Cards>("Cards&");
    GamePanel w;
    w.show();
    return a.exec();
}
