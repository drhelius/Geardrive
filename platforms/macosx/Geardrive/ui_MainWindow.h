/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionInput;
    QAction *actionVideo;
    QAction *actionSound;
    QAction *actionForce_Game_Boy_DMG;
    QAction *actionDisassembler;
    QAction *actionOAM_Viewer;
    QAction *actionMap_Viewer;
    QAction *actionLoad_ROM;
    QAction *action1;
    QAction *action2;
    QAction *action3;
    QAction *action4;
    QAction *action5;
    QAction *actionSave_State;
    QAction *actionLoad_State;
    QAction *actionSave_State_as;
    QAction *actionLad_State_from;
    QAction *actionReset;
    QAction *actionPause;
    QAction *actionFullscreen;
    QAction *actionPalette_Viewer;
    QAction *actionX_1;
    QAction *actionX_2;
    QAction *actionX_3;
    QAction *actionX_4;
    QAction *actionX_5;
    QAction *actionAbout;
    QWidget *centralwidget;
    QMenuBar *menubar;
    QMenu *menuSettings;
    QMenu *menuWindow_Size;
    QMenu *menuHelp;
    QMenu *menuDebug;
    QMenu *menuGame_Boy;
    QMenu *menuSelect_State_Slot;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(342, 321);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        actionInput = new QAction(MainWindow);
        actionInput->setObjectName(QStringLiteral("actionInput"));
        actionVideo = new QAction(MainWindow);
        actionVideo->setObjectName(QStringLiteral("actionVideo"));
        actionSound = new QAction(MainWindow);
        actionSound->setObjectName(QStringLiteral("actionSound"));
        actionForce_Game_Boy_DMG = new QAction(MainWindow);
        actionForce_Game_Boy_DMG->setObjectName(QStringLiteral("actionForce_Game_Boy_DMG"));
        actionForce_Game_Boy_DMG->setCheckable(true);
        actionDisassembler = new QAction(MainWindow);
        actionDisassembler->setObjectName(QStringLiteral("actionDisassembler"));
        actionOAM_Viewer = new QAction(MainWindow);
        actionOAM_Viewer->setObjectName(QStringLiteral("actionOAM_Viewer"));
        actionMap_Viewer = new QAction(MainWindow);
        actionMap_Viewer->setObjectName(QStringLiteral("actionMap_Viewer"));
        actionLoad_ROM = new QAction(MainWindow);
        actionLoad_ROM->setObjectName(QStringLiteral("actionLoad_ROM"));
        action1 = new QAction(MainWindow);
        action1->setObjectName(QStringLiteral("action1"));
        action1->setCheckable(true);
        action2 = new QAction(MainWindow);
        action2->setObjectName(QStringLiteral("action2"));
        action2->setCheckable(true);
        action3 = new QAction(MainWindow);
        action3->setObjectName(QStringLiteral("action3"));
        action3->setCheckable(true);
        action4 = new QAction(MainWindow);
        action4->setObjectName(QStringLiteral("action4"));
        action4->setCheckable(true);
        action5 = new QAction(MainWindow);
        action5->setObjectName(QStringLiteral("action5"));
        action5->setCheckable(true);
        actionSave_State = new QAction(MainWindow);
        actionSave_State->setObjectName(QStringLiteral("actionSave_State"));
        actionLoad_State = new QAction(MainWindow);
        actionLoad_State->setObjectName(QStringLiteral("actionLoad_State"));
        actionSave_State_as = new QAction(MainWindow);
        actionSave_State_as->setObjectName(QStringLiteral("actionSave_State_as"));
        actionLad_State_from = new QAction(MainWindow);
        actionLad_State_from->setObjectName(QStringLiteral("actionLad_State_from"));
        actionReset = new QAction(MainWindow);
        actionReset->setObjectName(QStringLiteral("actionReset"));
        actionPause = new QAction(MainWindow);
        actionPause->setObjectName(QStringLiteral("actionPause"));
        actionPause->setCheckable(true);
        actionPause->setChecked(false);
        actionFullscreen = new QAction(MainWindow);
        actionFullscreen->setObjectName(QStringLiteral("actionFullscreen"));
        actionFullscreen->setShortcutContext(Qt::ApplicationShortcut);
        actionPalette_Viewer = new QAction(MainWindow);
        actionPalette_Viewer->setObjectName(QStringLiteral("actionPalette_Viewer"));
        actionX_1 = new QAction(MainWindow);
        actionX_1->setObjectName(QStringLiteral("actionX_1"));
        actionX_1->setCheckable(true);
        actionX_2 = new QAction(MainWindow);
        actionX_2->setObjectName(QStringLiteral("actionX_2"));
        actionX_2->setCheckable(true);
        actionX_3 = new QAction(MainWindow);
        actionX_3->setObjectName(QStringLiteral("actionX_3"));
        actionX_3->setCheckable(true);
        actionX_4 = new QAction(MainWindow);
        actionX_4->setObjectName(QStringLiteral("actionX_4"));
        actionX_4->setCheckable(true);
        actionX_5 = new QAction(MainWindow);
        actionX_5->setObjectName(QStringLiteral("actionX_5"));
        actionX_5->setCheckable(true);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy1);
        centralwidget->setLayoutDirection(Qt::LeftToRight);
        centralwidget->setAutoFillBackground(false);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 342, 25));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(menubar->sizePolicy().hasHeightForWidth());
        menubar->setSizePolicy(sizePolicy2);
        menuSettings = new QMenu(menubar);
        menuSettings->setObjectName(QStringLiteral("menuSettings"));
        menuWindow_Size = new QMenu(menuSettings);
        menuWindow_Size->setObjectName(QStringLiteral("menuWindow_Size"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        menuDebug = new QMenu(menubar);
        menuDebug->setObjectName(QStringLiteral("menuDebug"));
        menuGame_Boy = new QMenu(menubar);
        menuGame_Boy->setObjectName(QStringLiteral("menuGame_Boy"));
        menuSelect_State_Slot = new QMenu(menuGame_Boy);
        menuSelect_State_Slot->setObjectName(QStringLiteral("menuSelect_State_Slot"));
        MainWindow->setMenuBar(menubar);

        menubar->addAction(menuGame_Boy->menuAction());
        menubar->addAction(menuSettings->menuAction());
        menubar->addAction(menuDebug->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuSettings->addAction(actionInput);
        menuSettings->addAction(actionVideo);
        menuSettings->addAction(actionSound);
        menuSettings->addSeparator();
        menuSettings->addAction(menuWindow_Size->menuAction());
        menuSettings->addAction(actionFullscreen);
        menuWindow_Size->addAction(actionX_1);
        menuWindow_Size->addAction(actionX_2);
        menuWindow_Size->addAction(actionX_3);
        menuWindow_Size->addAction(actionX_4);
        menuWindow_Size->addAction(actionX_5);
        menuHelp->addAction(actionAbout);
        menuDebug->addAction(actionDisassembler);
        menuDebug->addAction(actionOAM_Viewer);
        menuDebug->addAction(actionMap_Viewer);
        menuDebug->addAction(actionPalette_Viewer);
        menuGame_Boy->addAction(actionLoad_ROM);
        menuGame_Boy->addAction(actionPause);
        menuGame_Boy->addAction(actionReset);
        menuGame_Boy->addSeparator();
        menuGame_Boy->addAction(menuSelect_State_Slot->menuAction());
        menuGame_Boy->addAction(actionSave_State);
        menuGame_Boy->addAction(actionLoad_State);
        menuGame_Boy->addSeparator();
        menuGame_Boy->addAction(actionSave_State_as);
        menuGame_Boy->addAction(actionLad_State_from);
        menuSelect_State_Slot->addAction(action1);
        menuSelect_State_Slot->addAction(action2);
        menuSelect_State_Slot->addAction(action3);
        menuSelect_State_Slot->addAction(action4);
        menuSelect_State_Slot->addAction(action5);

        retranslateUi(MainWindow);
        QObject::connect(actionLoad_ROM, SIGNAL(triggered()), MainWindow, SLOT(MenuGameBoyLoadROM()));
        QObject::connect(actionPause, SIGNAL(triggered(bool)), MainWindow, SLOT(MenuGameBoyPause()));
        QObject::connect(actionReset, SIGNAL(triggered()), MainWindow, SLOT(MenuGameBoyReset()));
        QObject::connect(actionSave_State, SIGNAL(triggered()), MainWindow, SLOT(MenuGameBoySaveState()));
        QObject::connect(actionLoad_State, SIGNAL(triggered()), MainWindow, SLOT(MenuGameBoyLoadState()));
        QObject::connect(actionSave_State_as, SIGNAL(triggered()), MainWindow, SLOT(MenuGameBoySaveStateAs()));
        QObject::connect(actionLad_State_from, SIGNAL(triggered()), MainWindow, SLOT(MenuGameBoyLoadStateFrom()));
        QObject::connect(actionInput, SIGNAL(triggered()), MainWindow, SLOT(MenuSettingsInput()));
        QObject::connect(actionVideo, SIGNAL(triggered()), MainWindow, SLOT(MenuSettingsVideo()));
        QObject::connect(actionSound, SIGNAL(triggered()), MainWindow, SLOT(MenuSettingsSound()));
        QObject::connect(actionFullscreen, SIGNAL(triggered()), MainWindow, SLOT(MenuSettingsFullscreen()));
        QObject::connect(actionForce_Game_Boy_DMG, SIGNAL(triggered()), MainWindow, SLOT(MenuSettingsForceDMG()));
        QObject::connect(actionDisassembler, SIGNAL(triggered()), MainWindow, SLOT(MenuDebugDisassembler()));
        QObject::connect(actionOAM_Viewer, SIGNAL(triggered()), MainWindow, SLOT(MenuDebugOAM()));
        QObject::connect(actionMap_Viewer, SIGNAL(triggered()), MainWindow, SLOT(MenuDebugMap()));
        QObject::connect(actionPalette_Viewer, SIGNAL(triggered()), MainWindow, SLOT(MenuDebugPalette()));
        QObject::connect(actionAbout, SIGNAL(triggered()), MainWindow, SLOT(MenuAbout()));
        QObject::connect(menuWindow_Size, SIGNAL(triggered(QAction*)), MainWindow, SLOT(MenuSettingsWindowSize(QAction*)));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Gearsystem", 0));
        actionInput->setText(QApplication::translate("MainWindow", "Input...", 0));
        actionVideo->setText(QApplication::translate("MainWindow", "Video...", 0));
        actionSound->setText(QApplication::translate("MainWindow", "Sound...", 0));
        actionForce_Game_Boy_DMG->setText(QApplication::translate("MainWindow", "Force Game Boy DMG", 0));
        actionDisassembler->setText(QApplication::translate("MainWindow", "Disassembler...", 0));
        actionOAM_Viewer->setText(QApplication::translate("MainWindow", "OAM Viewer...", 0));
        actionMap_Viewer->setText(QApplication::translate("MainWindow", "Map Viewer...", 0));
        actionLoad_ROM->setText(QApplication::translate("MainWindow", "Load ROM...", 0));
        action1->setText(QApplication::translate("MainWindow", "1", 0));
        action2->setText(QApplication::translate("MainWindow", "2", 0));
        action3->setText(QApplication::translate("MainWindow", "3", 0));
        action4->setText(QApplication::translate("MainWindow", "4", 0));
        action5->setText(QApplication::translate("MainWindow", "5", 0));
        actionSave_State->setText(QApplication::translate("MainWindow", "Save State", 0));
        actionSave_State->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0));
        actionLoad_State->setText(QApplication::translate("MainWindow", "Load State", 0));
        actionLoad_State->setShortcut(QApplication::translate("MainWindow", "Ctrl+L", 0));
        actionSave_State_as->setText(QApplication::translate("MainWindow", "Save State As...", 0));
        actionLad_State_from->setText(QApplication::translate("MainWindow", "Load State From...", 0));
        actionReset->setText(QApplication::translate("MainWindow", "Reset", 0));
#ifndef QT_NO_WHATSTHIS
        actionReset->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
        actionReset->setShortcut(QApplication::translate("MainWindow", "Ctrl+R", 0));
        actionPause->setText(QApplication::translate("MainWindow", "Pause", 0));
        actionPause->setShortcut(QApplication::translate("MainWindow", "Ctrl+P", 0));
        actionFullscreen->setText(QApplication::translate("MainWindow", "Fullscreen", 0));
        actionFullscreen->setShortcut(QApplication::translate("MainWindow", "Ctrl+F", 0));
        actionPalette_Viewer->setText(QApplication::translate("MainWindow", "Palette Viewer...", 0));
        actionX_1->setText(QApplication::translate("MainWindow", "x 1", 0));
        actionX_2->setText(QApplication::translate("MainWindow", "x 2", 0));
        actionX_3->setText(QApplication::translate("MainWindow", "x 3", 0));
        actionX_4->setText(QApplication::translate("MainWindow", "x 4", 0));
        actionX_5->setText(QApplication::translate("MainWindow", "x 5", 0));
        actionAbout->setText(QApplication::translate("MainWindow", "About", 0));
        menuSettings->setTitle(QApplication::translate("MainWindow", "Settings", 0));
        menuWindow_Size->setTitle(QApplication::translate("MainWindow", "Window Size", 0));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0));
        menuDebug->setTitle(QApplication::translate("MainWindow", "Debug", 0));
        menuGame_Boy->setTitle(QApplication::translate("MainWindow", "Emulator", 0));
        menuSelect_State_Slot->setTitle(QApplication::translate("MainWindow", "Select State Slot", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
