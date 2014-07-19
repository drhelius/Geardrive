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

#include "sound_settings.h"

#include "gl_frame.h"
#include "emulator.h"

SoundSettings::SoundSettings(GLFrame* gl_frame, Emulator* emulator)
{
    rate_ = 1;
    enabled_ = true;
    gl_frame_ = gl_frame;
    emulator_ = emulator;
    widget.setupUi(this);

    widget.comboBoxSampleRate->addItem("48000");
    widget.comboBoxSampleRate->addItem("44100");
    widget.comboBoxSampleRate->addItem("22050");

    widget.comboBoxSampleRate->setCurrentIndex(rate_);
    widget.checkBoxSoundEnabled->setChecked(enabled_);
}

SoundSettings::~SoundSettings()
{
}

void SoundSettings::PressedOK()
{
    rate_ = widget.comboBoxSampleRate->currentIndex();
    enabled_ = widget.checkBoxSoundEnabled->isChecked();

    int sampleRate = 0;
    switch (rate_)
    {
        case 0:
            sampleRate = 48000;
            break;
        case 1:
            sampleRate = 44100;
            break;
        case 2:
            sampleRate = 22050;
            break;
        default:
            sampleRate = 44100;
    }

    emulator_->SetSoundSettings(enabled_, sampleRate);
    gl_frame_->ResumeRenderThread();
    this->accept();
}

void SoundSettings::PressedCancel()
{
    widget.comboBoxSampleRate->setCurrentIndex(rate_);
    widget.checkBoxSoundEnabled->setChecked(enabled_);
    gl_frame_->ResumeRenderThread();
    this->reject();
}

void SoundSettings::SaveSettings(QSettings& settings)
{
    settings.setValue("SampleRate", rate_);
    settings.setValue("SoundEnabled", enabled_);
}

void SoundSettings::LoadSettings(QSettings& settings)
{
    rate_ = settings.value("SampleRate", 1).toInt();
    enabled_ = settings.value("SoundEnabled", true).toBool();
    widget.comboBoxSampleRate->setCurrentIndex(rate_);
    widget.checkBoxSoundEnabled->setChecked(enabled_);

    int sampleRate = 0;
    switch (rate_)
    {
        case 0:
            sampleRate = 48000;
            break;
        case 1:
            sampleRate = 44100;
            break;
        case 2:
            sampleRate = 22050;
            break;
        default:
            sampleRate = 44100;
    }

    emulator_->SetSoundSettings(enabled_, sampleRate);
}
