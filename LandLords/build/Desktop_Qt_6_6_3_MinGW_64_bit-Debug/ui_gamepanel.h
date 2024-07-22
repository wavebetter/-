/********************************************************************************
** Form generated from reading UI file 'gamepanel.ui'
**
** Created by: Qt User Interface Compiler version 6.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAMEPANEL_H
#define UI_GAMEPANEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "buttongroup.h"
#include "scorepanel.h"

QT_BEGIN_NAMESPACE

class Ui_GamePanel
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_2;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    ScorePanel *scorePanel;
    QSpacerItem *verticalSpacer;
    ButtonGroup *btnGroup;
    QSpacerItem *verticalSpacer_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *GamePanel)
    {
        if (GamePanel->objectName().isEmpty())
            GamePanel->setObjectName("GamePanel");
        GamePanel->resize(783, 598);
        centralwidget = new QWidget(GamePanel);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout_2 = new QHBoxLayout(centralwidget);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        widget = new QWidget(centralwidget);
        widget->setObjectName("widget");
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName("verticalLayout");
        widget_2 = new QWidget(widget);
        widget_2->setObjectName("widget_2");
        horizontalLayout = new QHBoxLayout(widget_2);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(710, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        scorePanel = new ScorePanel(widget_2);
        scorePanel->setObjectName("scorePanel");

        horizontalLayout->addWidget(scorePanel);


        verticalLayout->addWidget(widget_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        btnGroup = new ButtonGroup(widget);
        btnGroup->setObjectName("btnGroup");

        verticalLayout->addWidget(btnGroup);

        verticalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout->addItem(verticalSpacer_2);


        horizontalLayout_2->addWidget(widget);

        GamePanel->setCentralWidget(centralwidget);
        menubar = new QMenuBar(GamePanel);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 783, 21));
        GamePanel->setMenuBar(menubar);
        statusbar = new QStatusBar(GamePanel);
        statusbar->setObjectName("statusbar");
        GamePanel->setStatusBar(statusbar);

        retranslateUi(GamePanel);

        QMetaObject::connectSlotsByName(GamePanel);
    } // setupUi

    void retranslateUi(QMainWindow *GamePanel)
    {
        GamePanel->setWindowTitle(QCoreApplication::translate("GamePanel", "GamePanel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GamePanel: public Ui_GamePanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMEPANEL_H
