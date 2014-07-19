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

#ifndef GD_INPUTSETTINGS_H_
#define	GD_INPUTSETTINGS_H_

#include <QSettings>
#include "ui_InputSettings.h"

struct stCustomKey
{
    int keyCode;
    char text[32];
};

class GLFrame;

class InputSettings : public QDialog
{
    Q_OBJECT

public:
    InputSettings(GLFrame* gl_frame_);
    ~InputSettings();
    bool eventFilter(QObject* obj, QEvent* event);
    int GetKey(int key);
    void SaveSettings(QSettings& settings);
    void LoadSettings(QSettings& settings);

public slots:
    void SaveKeys();
    void RestoreKeys();

private:
    void PrintKey(QKeyEvent& event, char* buffer);

private:
    Ui::InputSettings widget;
    stCustomKey keys_[7];
    stCustomKey temp_keys_[7];
    GLFrame* gl_frame_;
};

#endif // GD_INPUTSETTINGS_H_
