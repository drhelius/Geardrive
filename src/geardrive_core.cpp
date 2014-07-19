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

#include "geardrive_core.h"
//#include "Memory.h"
//#include "Processor.h"
#include "audio.h"
#include "video.h"
#include "input.h"
#include "cartridge.h"
//#include "MemoryRule.h"
//#include "SegaMemoryRule.h"
//#include "CodemastersMemoryRule.h"
//#include "RomOnlyMemoryRule.h"
//#include "SmsIOPorts.h"
//#include "GameGearIOPorts.h"

GeardriveCore::GeardriveCore()
{
//    InitPointer(m_pMemory);
//    InitPointer(m_pProcessor);
    InitPointer(audio_);
    InitPointer(video_);
    InitPointer(input_);
    InitPointer(cartridge_);
//    InitPointer(m_pSegaMemoryRule);
//    InitPointer(m_pCodemastersMemoryRule);
//    InitPointer(m_pRomOnlyMemoryRule);
//    InitPointer(m_pSmsIOPorts);
//    InitPointer(m_pGameGearIOPorts);
    paused_ = true;
}

GeardriveCore::~GeardriveCore()
{
#ifdef DEBUG_GEARSYSTEM
    if (cartridge_->IsReady())
    {
        Log("Saving Memory Dump...");

        using namespace std;

        char path[512];

        strcpy(path, cartridge_->GetFilePath());
        strcat(path, ".dump");

        m_pMemory->MemoryDump(path);

        Log("Memory Dump Saved");
    }
#endif

//    SafeDelete(m_pGameGearIOPorts);
//    SafeDelete(m_pSmsIOPorts);
//    SafeDelete(m_pRomOnlyMemoryRule);
//    SafeDelete(m_pCodemastersMemoryRule);
//    SafeDelete(m_pSegaMemoryRule);
    SafeDelete(cartridge_);
    SafeDelete(input_);
    SafeDelete(video_);
    SafeDelete(audio_);
//    SafeDelete(m_pProcessor);
//    SafeDelete(m_pMemory);
}

void GeardriveCore::Init()
{
    Log("-=:: GEARDRIVE %1.1f ::=-", GEARDRIVE_VERSION);

//    m_pMemory = new Memory();
//    m_pProcessor = new Processor(m_pMemory);
    audio_ = new Audio();
    video_ = new Video();
    input_ = new Input();
    cartridge_ = new Cartridge();
//    m_pSmsIOPorts = new SmsIOPorts(audio_, video_, input_, m_pCartridge);
//    m_pGameGearIOPorts = new GameGearIOPorts(audio_, video_, input_, m_pCartridge);
//
//    m_pMemory->Init();
//    m_pProcessor->Init();
    audio_->Init();
    video_->Init();
    input_->Init();
    cartridge_->Init();

    InitMemoryRules();
}

void GeardriveCore::RunToVBlank(GD_Color* frame_Buffer)
{
    if (!paused_ && cartridge_->IsReady())
    {
        bool vblank = false;
        while (!vblank)
        {
            unsigned int clock_cycles = 0;
//            unsigned int clockCycles = m_pProcessor->Tick();
            vblank = video_->Tick(clock_cycles, frame_Buffer);
            audio_->Tick(clock_cycles);
            input_->Tick(clock_cycles);
        }
        audio_->EndFrame();
    }
}

bool GeardriveCore::LoadROM(const char* path)
{
#ifdef DEBUG_GEARSYSTEM
    if (cartridge_->IsReady())
    {
        Log("Saving Memory Dump...");

        using namespace std;

        char dmp_path[512];

        strcpy(dmp_path, cartridge_->GetFilePath());
        strcat(dmp_path, ".dump");

        m_pMemory->MemoryDump(dmp_path);

        Log("Memory Dump Saved");
    }
#endif

    bool loaded = cartridge_->LoadFromFile(path);
    if (loaded)
    {
        Reset();
//        m_pMemory->LoadSlotsFromROM(m_pCartridge->GetROM(), m_pCartridge->GetROMSize());
        bool romTypeOK = AddMemoryRules();

        if (!romTypeOK)
        {
            Log("There was a problem with the cartridge header. File: %s...", path);
        }

        return romTypeOK;
    }
    else
        return false;
}

//Memory* GearsystemCore::GetMemory()
//{
//    return m_pMemory;
//}

Cartridge* GeardriveCore::GetCartridge()
{
    return cartridge_;
}

void GeardriveCore::KeyPressed(GD_Joypads joypad, GD_Keys key)
{
    input_->KeyPressed(joypad, key);
}

void GeardriveCore::KeyReleased(GD_Joypads joypad, GD_Keys key)
{
    input_->KeyReleased(joypad, key);
}

void GeardriveCore::Pause(bool paused)
{
    if (paused)
    {
        Log("Geardrive PAUSED");
    }
    else
    {
        Log("Geardrive RESUMED");
    }
    paused_ = paused;
}

bool GeardriveCore::IsPaused()
{
    return paused_;
}

void GeardriveCore::ResetROM()
{
    if (cartridge_->IsReady())
    {
        Log("Geardrive RESET");
        Reset();
//        m_pMemory->LoadSlotsFromROM(m_pCartridge->GetROM(), m_pCartridge->GetROMSize());
        AddMemoryRules();
    }
}

void GeardriveCore::EnableSound(bool enabled)
{
    if (enabled)
    {
        Log("Geardrive sound ENABLED");
    }
    else
    {
        Log("Geardrive sound DISABLED");
    }
    audio_->Enable(enabled);
}

void GeardriveCore::SetSoundSampleRate(int rate)
{
    Log("Geardrive sound sample rate: %d", rate);
    audio_->SetSampleRate(rate);
}

void GeardriveCore::SaveRam()
{
    SaveRam(NULL);
}

void GeardriveCore::SaveRam(const char* path)
{
//    if (m_pCartridge->IsReady() && IsValidPointer(m_pMemory->GetCurrentRule()) && m_pMemory->GetCurrentRule()->PersistedRAM())
//    {
//        Log("Saving RAM...");
//
//        using namespace std;
//
//        char path[512];
//
//        if (IsValidPointer(szPath))
//        {
//            strcpy(path, szPath);
//            strcat(path, "/");
//            strcat(path, m_pCartridge->GetFileName());
//        }
//        else
//        {
//            strcpy(path, m_pCartridge->GetFilePath());
//        }
//
//        strcat(path, ".gearsystem");
//
//        Log("Save file: %s", path);
//
//        ofstream file(path, ios::out | ios::binary);
//
//        m_pMemory->GetCurrentRule()->SaveRam(file);
//
//        Log("RAM saved");
//    }
}

void GeardriveCore::LoadRam()
{
    LoadRam(NULL);
}

void GeardriveCore::LoadRam(const char* path)
{
//    if (m_pCartridge->IsReady() && IsValidPointer(m_pMemory->GetCurrentRule()))
//    {
//        Log("Loading RAM...");
//
//        using namespace std;
//
//        char path[512];
//
//        if (IsValidPointer(szPath))
//        {
//            strcpy(path, szPath);
//            strcat(path, "/");
//            strcat(path, m_pCartridge->GetFileName());
//        }
//        else
//        {
//            strcpy(path, m_pCartridge->GetFilePath());
//        }
//
//        strcat(path, ".gearsystem");
//
//        Log("Opening save file: %s", path);
//
//        ifstream file(path, ios::in | ios::binary);
//
//        if (!file.fail())
//        {
//            char signature[16];
//
//            file.read(signature, 16);
//
//            file.seekg(0, file.end);
//            s32 fileSize = (s32) file.tellg();
//            file.seekg(0, file.beg);
//
//            if (m_pMemory->GetCurrentRule()->LoadRam(file, fileSize))
//            {
//                Log("RAM loaded");
//            }
//            else
//            {
//                Log("Save file size incorrect: %d", fileSize);
//            }
//        }
//        else
//        {
//            Log("Save file doesn't exist");
//        }
//    }
}

float GeardriveCore::GetVersion()
{
    return GEARDRIVE_VERSION;
}

void GeardriveCore::InitMemoryRules()
{
//    m_pCodemastersMemoryRule = new CodemastersMemoryRule(m_pMemory, m_pCartridge);
//    m_pSegaMemoryRule = new SegaMemoryRule(m_pMemory, m_pCartridge);
//    m_pRomOnlyMemoryRule = new RomOnlyMemoryRule(m_pMemory, m_pCartridge);
}

bool GeardriveCore::AddMemoryRules()
{
//    Cartridge::CartridgeTypes type = m_pCartridge->GetType();
//
//    bool notSupported = false;
//
//    switch (type)
//    {
//        case Cartridge::CartridgeRomOnlyMapper:
//            m_pMemory->SetCurrentRule(m_pRomOnlyMemoryRule);
//            break;
//        case Cartridge::CartridgeSegaMapper:
//            m_pMemory->SetCurrentRule(m_pSegaMemoryRule);
//            break;
//        case Cartridge::CartridgeCodemastersMapper:
//            m_pMemory->SetCurrentRule(m_pCodemastersMemoryRule);
//            break;
//        case Cartridge::CartridgeNotSupported:
//            notSupported = true;
//            break;
//        default:
//            notSupported = true;
//    }
//
//    if (m_pCartridge->IsGameGear())
//    {
//        Log("Game Gear Mode enabled");
//        m_pProcessor->SetIOPOrts(m_pGameGearIOPorts);
//    }
//    else
//    {
//        Log("Master System Mode enabled");
//        m_pProcessor->SetIOPOrts(m_pSmsIOPorts);
//    }
//
//    return !notSupported;
    return true;
}

void GeardriveCore::Reset()
{
//    m_pMemory->Reset();
//    m_pProcessor->Reset();
    audio_->Reset();
    video_->Reset(cartridge_->IsPAL());
    input_->Reset();
//    m_pSegaMemoryRule->Reset();
//    m_pCodemastersMemoryRule->Reset();
//    m_pRomOnlyMemoryRule->Reset();
//    m_pGameGearIOPorts->Reset();
//    m_pSmsIOPorts->Reset();
    paused_ = false;
}

