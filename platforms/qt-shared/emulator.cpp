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

#include "emulator.h"

Emulator::Emulator()
{
    InitPointer(gearsystem_core_);
}

Emulator::~Emulator()
{
    SafeDelete(gearsystem_core_);
}

void Emulator::Init()
{
    gearsystem_core_ = new GeardriveCore();
    gearsystem_core_->Init();
}

void Emulator::LoadRom(const char* path)
{
    mutex_.lock();
    gearsystem_core_->SaveRam();
    gearsystem_core_->LoadROM(path);
    gearsystem_core_->LoadRam();
    mutex_.unlock();
}

void Emulator::RunToVBlank(GD_Color* frame_buffer)
{
    mutex_.lock();
    gearsystem_core_->RunToVBlank(frame_buffer);
    mutex_.unlock();
}

void Emulator::KeyPressed(GD_Joypads joypad, GD_Keys key)
{
    mutex_.lock();
    gearsystem_core_->KeyPressed(joypad, key);
    mutex_.unlock();
}

void Emulator::KeyReleased(GD_Joypads joypad, GD_Keys key)
{
    mutex_.lock();
    gearsystem_core_->KeyReleased(joypad, key);
    mutex_.unlock();
}

void Emulator::Pause()
{
    mutex_.lock();
    gearsystem_core_->Pause(true);
    mutex_.unlock();
}

void Emulator::Resume()
{
    mutex_.lock();
    gearsystem_core_->Pause(false);
    mutex_.unlock();
}

bool Emulator::IsPaused()
{
    mutex_.lock();
    bool paused = gearsystem_core_->IsPaused();
    mutex_.unlock();
    return paused;
}

void Emulator::Reset()
{
    mutex_.lock();
    gearsystem_core_->SaveRam();
    gearsystem_core_->ResetROM();
    gearsystem_core_->LoadRam();
    mutex_.unlock();
}

void Emulator::MemoryDump()
{
//    m_Mutex.lock();
//    m_pGearsystemCore->GetMemory()->MemoryDump("memdump.txt");
//    m_Mutex.unlock();
}

void Emulator::SetSoundSettings(bool enabled, int rate)
{
    mutex_.lock();
    gearsystem_core_->EnableSound(enabled);
    gearsystem_core_->SetSoundSampleRate(rate);
    mutex_.unlock();
}

void Emulator::SaveRam()
{
    mutex_.lock();
    gearsystem_core_->SaveRam();
    mutex_.unlock();
}
