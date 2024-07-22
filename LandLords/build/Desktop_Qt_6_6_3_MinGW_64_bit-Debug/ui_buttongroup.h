/********************************************************************************
** Form generated from reading UI file 'buttongroup.ui'
**
** Created by: Qt User Interface Compiler version 6.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BUTTONGROUP_H
#define UI_BUTTONGROUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "mybutton.h"

QT_BEGIN_NAMESPACE

class Ui_ButtonGroup
{
public:
    QVBoxLayout *verticalLayout;
    QStackedWidget *stackedWidget;
    QWidget *startPage;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    MyButton *start;
    QSpacerItem *horizontalSpacer_2;
    QWidget *playCardPage;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_3;
    MyButton *playCard;
    QSpacerItem *horizontalSpacer_4;
    QWidget *PassorPlayPage;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_6;
    MyButton *pass;
    QSpacerItem *horizontalSpacer_5;
    MyButton *playCard2;
    QSpacerItem *horizontalSpacer_7;
    QWidget *callLordPage;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_11;
    MyButton *giveup;
    QSpacerItem *horizontalSpacer_8;
    MyButton *oneScore;
    QSpacerItem *horizontalSpacer_9;
    MyButton *twoScore;
    QSpacerItem *horizontalSpacer_10;
    MyButton *threeScore;
    QSpacerItem *horizontalSpacer_12;
    QWidget *nullPage;

    void setupUi(QWidget *ButtonGroup)
    {
        if (ButtonGroup->objectName().isEmpty())
            ButtonGroup->setObjectName("ButtonGroup");
        ButtonGroup->resize(514, 48);
        verticalLayout = new QVBoxLayout(ButtonGroup);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        stackedWidget = new QStackedWidget(ButtonGroup);
        stackedWidget->setObjectName("stackedWidget");
        startPage = new QWidget();
        startPage->setObjectName("startPage");
        horizontalLayout = new QHBoxLayout(startPage);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(202, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        start = new MyButton(startPage);
        start->setObjectName("start");

        horizontalLayout->addWidget(start);

        horizontalSpacer_2 = new QSpacerItem(201, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        stackedWidget->addWidget(startPage);
        playCardPage = new QWidget();
        playCardPage->setObjectName("playCardPage");
        horizontalLayout_3 = new QHBoxLayout(playCardPage);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalSpacer_3 = new QSpacerItem(202, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        playCard = new MyButton(playCardPage);
        playCard->setObjectName("playCard");

        horizontalLayout_3->addWidget(playCard);

        horizontalSpacer_4 = new QSpacerItem(201, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);

        stackedWidget->addWidget(playCardPage);
        PassorPlayPage = new QWidget();
        PassorPlayPage->setObjectName("PassorPlayPage");
        horizontalLayout_2 = new QHBoxLayout(PassorPlayPage);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalSpacer_6 = new QSpacerItem(144, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_6);

        pass = new MyButton(PassorPlayPage);
        pass->setObjectName("pass");

        horizontalLayout_2->addWidget(pass);

        horizontalSpacer_5 = new QSpacerItem(25, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_5);

        playCard2 = new MyButton(PassorPlayPage);
        playCard2->setObjectName("playCard2");

        horizontalLayout_2->addWidget(playCard2);

        horizontalSpacer_7 = new QSpacerItem(144, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_7);

        stackedWidget->addWidget(PassorPlayPage);
        callLordPage = new QWidget();
        callLordPage->setObjectName("callLordPage");
        horizontalLayout_4 = new QHBoxLayout(callLordPage);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalSpacer_11 = new QSpacerItem(52, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_11);

        giveup = new MyButton(callLordPage);
        giveup->setObjectName("giveup");

        horizontalLayout_4->addWidget(giveup);

        horizontalSpacer_8 = new QSpacerItem(10, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_8);

        oneScore = new MyButton(callLordPage);
        oneScore->setObjectName("oneScore");

        horizontalLayout_4->addWidget(oneScore);

        horizontalSpacer_9 = new QSpacerItem(10, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_9);

        twoScore = new MyButton(callLordPage);
        twoScore->setObjectName("twoScore");

        horizontalLayout_4->addWidget(twoScore);

        horizontalSpacer_10 = new QSpacerItem(10, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_10);

        threeScore = new MyButton(callLordPage);
        threeScore->setObjectName("threeScore");

        horizontalLayout_4->addWidget(threeScore);

        horizontalSpacer_12 = new QSpacerItem(51, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_12);

        stackedWidget->addWidget(callLordPage);
        nullPage = new QWidget();
        nullPage->setObjectName("nullPage");
        stackedWidget->addWidget(nullPage);

        verticalLayout->addWidget(stackedWidget);


        retranslateUi(ButtonGroup);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ButtonGroup);
    } // setupUi

    void retranslateUi(QWidget *ButtonGroup)
    {
        ButtonGroup->setWindowTitle(QCoreApplication::translate("ButtonGroup", "Form", nullptr));
        start->setText(QCoreApplication::translate("ButtonGroup", "PushButton", nullptr));
        playCard->setText(QCoreApplication::translate("ButtonGroup", "PushButton", nullptr));
        pass->setText(QCoreApplication::translate("ButtonGroup", "PushButton", nullptr));
        playCard2->setText(QCoreApplication::translate("ButtonGroup", "PushButton", nullptr));
        giveup->setText(QCoreApplication::translate("ButtonGroup", "PushButton", nullptr));
        oneScore->setText(QCoreApplication::translate("ButtonGroup", "PushButton", nullptr));
        twoScore->setText(QCoreApplication::translate("ButtonGroup", "PushButton", nullptr));
        threeScore->setText(QCoreApplication::translate("ButtonGroup", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ButtonGroup: public Ui_ButtonGroup {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BUTTONGROUP_H
