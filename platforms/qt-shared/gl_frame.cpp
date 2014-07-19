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

#include "gl_frame.h"

#include <QResizeEvent>
#include <QGLContext>

GLFrame::GLFrame(QWidget *parent) : QGLWidget(parent), render_thread_(this)
{
    setAutoBufferSwap(false);
}

GLFrame::~GLFrame()
{
}

void GLFrame::InitRenderThread(Emulator* emulator)
{
    doneCurrent();
    context()->moveToThread(&render_thread_);
    render_thread_.SetEmulator(emulator);
    render_thread_.start();
}

void GLFrame::StopRenderThread()
{
    render_thread_.Stop();
    render_thread_.wait();
}

void GLFrame::PauseRenderThread()
{
    render_thread_.Pause();
}

void GLFrame::ResumeRenderThread()
{
    render_thread_.Resume();
}

bool GLFrame::IsRunningRenderThread()
{
    return render_thread_.IsRunningEmulator();
}

void GLFrame::resizeEvent(QResizeEvent *evt)
{
    render_thread_.ResizeViewport(evt->size());
}

void GLFrame::paintEvent(QPaintEvent *)
{
    // Do nothing. Let the thread do the work
}

void GLFrame::closeEvent(QCloseEvent *evt)
{
    StopRenderThread();
    QGLWidget::closeEvent(evt);
}

void GLFrame::SetBilinearFiletering(bool enabled)
{
    render_thread_.SetBilinearFiletering(enabled);
}

