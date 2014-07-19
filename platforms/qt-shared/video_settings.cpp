/*
 * Gearsystem - Sega Master System / Game Gear Emulator
 * Copyright (C) 2013  Ignacio Sanchez

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

#include "video_settings.h"

#include <QColorDialog>
#include "gl_frame.h"
#include "emulator.h"
#include "main_window.h"

VideoSettings::VideoSettings(GLFrame* pGLFrame, Emulator* pEmulator)
{
    widget.setupUi(this);
    emulator_ = pEmulator;
    gl_frame_ = pGLFrame;
}

VideoSettings::~VideoSettings()
{
}

void VideoSettings::PressedOK()
{
    gl_frame_->SetBilinearFiletering(widget.checkBoxFilter->isChecked());
    gl_frame_->ResumeRenderThread();
    this->accept();
}

void VideoSettings::PressedCancel()
{
    gl_frame_->ResumeRenderThread();
    this->reject();
}

void VideoSettings::SaveSettings(QSettings& settings)
{
    settings.setValue("BilinearFiltering", widget.checkBoxFilter->isChecked());
}

void VideoSettings::LoadSettings(QSettings& settings)
{
    widget.checkBoxFilter->setChecked(settings.value("BilinearFiltering", false).toBool());
    gl_frame_->SetBilinearFiletering(widget.checkBoxFilter->isChecked());
}
