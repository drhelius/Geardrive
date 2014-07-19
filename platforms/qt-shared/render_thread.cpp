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

#include "render_thread.h"

#include "gl_frame.h"
#include "emulator.h"

RenderThread::RenderThread(GLFrame* gl_frame) : QThread(), gl_frame_(gl_frame)
{
    paused_ = false;
    do_actual_rendering_ = true;
    frame_buffer_ = new GD_Color[GD_GEN_WIDTH * GD_GEN_HEIGHT];
    width_ = 0;
    height_ = 0;
    InitPointer(emulator_);
    filtering_ = false;
    resize_event_ = false;
    texture_ = 0;
}

RenderThread::~RenderThread()
{
}

void RenderThread::ResizeViewport(const QSize &size)
{
    width_ = size.width();
    height_ = size.height();
    resize_event_ = true;
}

void RenderThread::Stop()
{
    do_actual_rendering_ = false;
}

void RenderThread::Pause()
{
    paused_ = true;
}

void RenderThread::Resume()
{
    paused_ = false;
}

bool RenderThread::IsRunningEmulator()
{
    return do_actual_rendering_;
}

void RenderThread::SetEmulator(Emulator* emulator)
{
    emulator_ = emulator;
}

void RenderThread::run()
{
    gl_frame_->makeCurrent();
    
    Init();

    while (do_actual_rendering_)
    {
        if (!paused_)
        {
            emulator_->RunToVBlank(frame_buffer_);

            if (resize_event_)
            {
                resize_event_ = false;
            }

            RenderFrame();

            gl_frame_->swapBuffers();
        }
    }
    
    SafeDeleteArray(frame_buffer_);
    glDeleteTextures(1, &texture_);
}

void RenderThread::Init()
{
    for (int y = 0; y < GD_GEN_HEIGHT; ++y)
    {
        for (int x = 0; x < GD_GEN_WIDTH; ++x)
        {
            int pixel = (y * GD_GEN_WIDTH) + x;
            frame_buffer_[pixel].red = frame_buffer_[pixel].green =
                    frame_buffer_[pixel].blue = 0x00;
            frame_buffer_[pixel].alpha = 0xFF;
        }
    }

#ifndef __APPLE__
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        Log("GLEW Error: %s\n", glewGetErrorString(err));
    }
    Log("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif

    glGenTextures(1, &texture_);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_);
    SetupTexture((void*) frame_buffer_);
}

void RenderThread::SetupTexture(void* data)
{
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GD_GEN_WIDTH, GD_GEN_HEIGHT, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, (void*) data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
}

void RenderThread::RenderFrame()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, texture_);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, GD_GEN_WIDTH, GD_GEN_HEIGHT,
            GL_RGBA, GL_UNSIGNED_BYTE, (void*) frame_buffer_);
    if (filtering_)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }
    RenderQuad(width_, height_);
}

void RenderThread::RenderQuad(int viewport_width, int viewport_height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, viewport_width, viewport_height, 0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, viewport_width, viewport_height);

    glBegin(GL_QUADS);
    glTexCoord2d(0.0, 0.0);
    glVertex2d(0.0, 0.0);
    glTexCoord2d(1.0, 0.0);
    glVertex2d(viewport_width, 0.0);
    glTexCoord2d(1.0, 1.0);
    glVertex2d(viewport_width, viewport_height);
    glTexCoord2d(0.0, 1.0);
    glVertex2d(0.0, viewport_height);
    glEnd();
}

void RenderThread::SetBilinearFiletering(bool enabled)
{
    filtering_ = enabled;
}

