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

#ifndef GD_RENDERTHREAD_H_
#define GD_RENDERTHREAD_H_

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glew.h>
#endif
#include <QThread>
#include "../../src/geardrive.h"

class Emulator;
class GLFrame;
class QSize;

class RenderThread : public QThread
{
    Q_OBJECT

public:
    explicit RenderThread(GLFrame* gl_frame = 0);
    virtual ~RenderThread();
    void ResizeViewport(const QSize& size);
    void run();
    void Stop();
    void Pause();
    void Resume();
    void SetEmulator(Emulator* emulator);
    bool IsRunningEmulator();
    void SetBilinearFiletering(bool enabled);

protected:
    void Init();
    void RenderFrame();
    void RenderQuad(int viewport_width, int viewport_height);
    void SetupTexture(void* data);

private:
    bool do_actual_rendering_;
    bool paused_;
    int width_;
    int height_;
    GLFrame* gl_frame_;
    Emulator* emulator_;
    GD_Color* frame_buffer_;
    bool filtering_;
    bool resize_event_;
    GLuint texture_;
};

#endif // GD_RENDERTHREAD_H_
