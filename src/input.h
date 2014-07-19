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

#ifndef GD_INPUT_H_
#define	GD_INPUT_H_

#include "definitions.h"

enum GD_Keys
{
    kKeyUp,
    kKeyDown,
    kKeyLeft,
    kKeyRight,
    kKeyA,
    kKeyB,
    kKeyC,
    kKeyStart
};

enum GD_Joypads
{
    kJoypad1,
    kJoypad2
};

class Input
{
public:
    Input();
    void Init();
    void Reset();
    void Tick(unsigned int clock_cycles);
    void KeyPressed(GD_Joypads joypad, GD_Keys key);
    void KeyReleased(GD_Joypads joypad, GD_Keys key);

private:
    void Update();
};

#endif // GD_INPUT_H_

