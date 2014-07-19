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

#ifndef GD_VIDEO_H_
#define	GD_VIDEO_H_

#include "definitions.h"

#define GD_GEN_WIDTH 320
#define GD_GEN_HEIGHT 224
#define VDP_READ_VRAM_OPERATION 0x00
#define VDP_WRITE_VRAM_OPERATION 0x01
#define VDP_WRITE_REG_OPERATION 0x02
#define VDP_WRITE_CRAM_OPERATION 0x03

struct GD_Color
{
    u8 red;
    u8 green;
    u8 blue;
    u8 alpha;
};

class Video
{
public:
    Video();
    ~Video();
    void Init();
    void Reset(bool is_pal);
    bool Tick(unsigned int clock_cycles, GD_Color* frame_buffer);
    u8 GetVCounter();
    u8 GetHCounter();
    u8 GetDataPort();
    u8 GetStatusFlags();
    void WriteData(u8 data);
    void WriteControl(u8 control);
    void LatchHCounter();

private:
    void ScanLine(int line);
};

#endif // GD_VIDEO_H_

