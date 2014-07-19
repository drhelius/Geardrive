/*
 * Geardrive - Sega Mega Drive / Genesis Emulator
 * Copyright (C) 2014  Ignacio Sanchez Gines

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/ 
 * 
 */

#ifndef GD_MAINWINDOW_H_
#define GD_MAINWINDOW_H_

#include <QMainWindow>
#include <QKeyEvent>
#include <QShortcut>
#include "../../src/geardrive.h"

class GLFrame;
class Emulator;
class InputSettings;
class SoundSettings;
class VideoSettings;
class About;

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool event(QEvent *ev);
    bool eventFilter(QObject * watched, QEvent * event);

public slots:
    void Exit();
    void MenuGameBoyLoadROM();
    void MenuGameBoyPause();
    void MenuGameBoyReset();
    void MenuGameBoySelectStateSlot();
    void MenuGameBoySaveState();
    void MenuGameBoyLoadState();
    void MenuGameBoySaveStateAs();
    void MenuGameBoyLoadStateFrom();
    void MenuSettingsInput();
    void MenuSettingsVideo();
    void MenuSettingsSound();
    void MenuSettingsWindowSize(QAction* action);
    void MenuSettingsFullscreen();
    void MenuSettingsForceDMG();
    void MenuDebugDisassembler();
    void MenuDebugOAM();
    void MenuDebugMap();
    void MenuDebugPalette();
    void MenuAbout();
    void MenuGameBoyPressed();
    void MenuGameBoyReleased();
    void MenuSettingsPressed();
    void MenuSettingsReleased();
    void MenuDebugPressed();
    void MenuDebugReleased();

protected:
    void closeEvent(QCloseEvent *evt);
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);
    void MenuReleased();
    void ResizeWindow(int factor);

private:
    void LoadSettings();
    void SaveSettings();

private:
    Ui::MainWindow *ui_;
    GLFrame *gl_frame_;
    Emulator* emulator_;
    bool menu_pressed_[3];
    int screen_size_;
    bool fullscreen_;
    QShortcut* exit_shortcut_;
    InputSettings* input_settings_;
    SoundSettings* sound_settings_;
    VideoSettings* video_settings_;
    About* about_;
};

#endif // GD_MAINWINDOW_H_
