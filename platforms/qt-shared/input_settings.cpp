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

#include "input_settings.h"

#include <QKeyEvent>
#include "gl_frame.h"

InputSettings::InputSettings(GLFrame* gl_frame)
{
    gl_frame_ = gl_frame;
    widget.setupUi(this);
    widget.lineEditUp->installEventFilter(this);
    widget.lineEditRight->installEventFilter(this);
    widget.lineEditDown->installEventFilter(this);
    widget.lineEditLeft->installEventFilter(this);
    widget.lineEdit1->installEventFilter(this);
    widget.lineEdit2->installEventFilter(this);
    widget.lineEditStart->installEventFilter(this);
}

InputSettings::~InputSettings()
{
}

int InputSettings::GetKey(int key)
{
    for (int i = 0; i < 7; i++)
    {
        if (keys_[i].keyCode == key)
            return i;
    }
    return -1;
}

void InputSettings::SaveKeys()
{
    for (int i = 0; i < 7; i++)
    {
        keys_[i].keyCode = temp_keys_[i].keyCode;
        strcpy(keys_[i].text, temp_keys_[i].text);
    }
    gl_frame_->ResumeRenderThread();
    this->accept();
}

void InputSettings::RestoreKeys()
{
    for (int i = 0; i < 8; i++)
    {
        temp_keys_[i].keyCode = keys_[i].keyCode;
        strcpy(temp_keys_[i].text, keys_[i].text);
    }
    widget.lineEditUp->setText(keys_[0].text);
    widget.lineEditRight->setText(keys_[1].text);
    widget.lineEditDown->setText(keys_[2].text);
    widget.lineEditLeft->setText(keys_[3].text);
    widget.lineEdit1->setText(keys_[4].text);
    widget.lineEdit2->setText(keys_[5].text);
    widget.lineEditStart->setText(keys_[6].text);
    gl_frame_->ResumeRenderThread();
    this->reject();
}

bool InputSettings::eventFilter(QObject* obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* key_event = static_cast<QKeyEvent*> (event);

        char text[32];

        if (strcmp(obj->metaObject()->className(), "QLineEdit") == 0)
        {
            PrintKey(*key_event, text);
            QLineEdit* line_edit = static_cast<QLineEdit*> (obj);
            line_edit->setText(text);

            if (obj == widget.lineEditUp)
            {
                temp_keys_[0].keyCode = key_event->key();
                strcpy(temp_keys_[0].text, text);
            }
            else if (obj == widget.lineEditRight)
            {
                temp_keys_[1].keyCode = key_event->key();
                strcpy(temp_keys_[1].text, text);
            }
            else if (obj == widget.lineEditDown)
            {
                temp_keys_[2].keyCode = key_event->key();
                strcpy(temp_keys_[2].text, text);
            }
            else if (obj == widget.lineEditLeft)
            {
                temp_keys_[3].keyCode = key_event->key();
                strcpy(temp_keys_[3].text, text);
            }
            else if (obj == widget.lineEdit1)
            {
                temp_keys_[4].keyCode = key_event->key();
                strcpy(temp_keys_[4].text, text);
            }
            else if (obj == widget.lineEdit2)
            {
                temp_keys_[5].keyCode = key_event->key();
                strcpy(temp_keys_[5].text, text);
            }
            else if (obj == widget.lineEditStart)
            {
                temp_keys_[6].keyCode = key_event->key();
                strcpy(temp_keys_[6].text, text);
            }

            return true;
        }
    }

    return QDialog::eventFilter(obj, event);
}

void InputSettings::PrintKey(QKeyEvent& event, char* buffer)
{
    switch (event.key())
    {
        case Qt::Key_Control:
            strcpy(buffer, "CONTROL");
            break;
        case Qt::Key_Alt:
            strcpy(buffer, "ALT");
            break;
        case Qt::Key_Enter:
            strcpy(buffer, "ENTER");
            break;
        case Qt::Key_Shift:
            strcpy(buffer, "SHIFT");
            break;
        case Qt::Key_Backspace:
            strcpy(buffer, "BACKSPACE");
            break;
        case Qt::Key_Up:
            strcpy(buffer, "UP");
            break;
        case Qt::Key_Left:
            strcpy(buffer, "LEFT");
            break;
        case Qt::Key_Right:
            strcpy(buffer, "RIGHT");
            break;
        case Qt::Key_Down:
            strcpy(buffer, "DOWN");
            break;
        case Qt::Key_Return:
            strcpy(buffer, "RETURN");
            break;
        case Qt::Key_Space:
            strcpy(buffer, "SPACE");
            break;
        case Qt::Key_Tab:
            strcpy(buffer, "TAB");
            break;
        case Qt::Key_Home:
            strcpy(buffer, "HOME");
            break;
        case Qt::Key_End:
            strcpy(buffer, "END");
            break;
        case Qt::Key_PageUp:
            strcpy(buffer, "PAGE UP");
            break;
        case Qt::Key_PageDown:
            strcpy(buffer, "PAGE DOWN");
            break;
        case Qt::Key_Insert:
            strcpy(buffer, "INSERT");
            break;
        case Qt::Key_Delete:
            strcpy(buffer, "DELETE");
            break;
        default:
            strcpy(buffer, event.text().toUpper().toLatin1());
    }
}

void InputSettings::SaveSettings(QSettings& settings)
{
    settings.setValue("KeyUP", keys_[0].keyCode);
    settings.setValue("KeyRIGHT", keys_[1].keyCode);
    settings.setValue("KeyDOWN", keys_[2].keyCode);
    settings.setValue("KeyLEFT", keys_[3].keyCode);
    settings.setValue("KeyA", keys_[4].keyCode);
    settings.setValue("KeyB", keys_[5].keyCode);
    settings.setValue("KeySTART", keys_[6].keyCode);

    settings.setValue("KeyNameUP", keys_[0].text);
    settings.setValue("KeyNameRIGHT", keys_[1].text);
    settings.setValue("KeyNameDOWN", keys_[2].text);
    settings.setValue("KeyNameLEFT", keys_[3].text);
    settings.setValue("KeyNameA", keys_[4].text);
    settings.setValue("KeyNameB", keys_[5].text);
    settings.setValue("KeyNameSTART", keys_[6].text);
}

void InputSettings::LoadSettings(QSettings& settings)
{
    keys_[0].keyCode = settings.value("KeyUP", Qt::Key_Up).toInt();
    keys_[1].keyCode = settings.value("KeyRIGHT", Qt::Key_Right).toInt();
    keys_[2].keyCode = settings.value("KeyDOWN", Qt::Key_Down).toInt();
    keys_[3].keyCode = settings.value("KeyLEFT", Qt::Key_Left).toInt();
    keys_[4].keyCode = settings.value("KeyA", Qt::Key_S).toInt();
    keys_[5].keyCode = settings.value("KeyB", Qt::Key_A).toInt();
    keys_[6].keyCode = settings.value("KeySTART", Qt::Key_Return).toInt();

    strcpy(keys_[0].text, settings.value("KeyNameUP", "UP").toString().toLatin1().constData());
    strcpy(keys_[1].text, settings.value("KeyNameRIGHT", "RIGHT").toString().toLatin1().constData());
    strcpy(keys_[2].text, settings.value("KeyNameDOWN", "DOWN").toString().toLatin1().constData());
    strcpy(keys_[3].text, settings.value("KeyNameLEFT", "LEFT").toString().toLatin1().constData());
    strcpy(keys_[4].text, settings.value("KeyNameA", "S").toString().toLatin1().constData());
    strcpy(keys_[5].text, settings.value("KeyNameB", "A").toString().toLatin1().constData());
    strcpy(keys_[6].text, settings.value("KeyNameSTART", "RETURN").toString().toLatin1().constData());

    widget.lineEditUp->setText(keys_[0].text);
    widget.lineEditRight->setText(keys_[1].text);
    widget.lineEditDown->setText(keys_[2].text);
    widget.lineEditLeft->setText(keys_[3].text);
    widget.lineEdit1->setText(keys_[4].text);
    widget.lineEdit2->setText(keys_[5].text);
    widget.lineEditStart->setText(keys_[6].text);

    for (int i = 0; i < 7; i++)
    {
        temp_keys_[i].keyCode = keys_[i].keyCode;
        strcpy(temp_keys_[i].text, keys_[i].text);
    }
}
