/*
 * GZ80 - Zilog Z80 Emulator
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

#ifndef GZ80_DEFINITIONS_H_
#define	GZ80_DEFINITIONS_H_

#include <cstdarg>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <stdint.h>
#include <iostream>
#include <fstream>

#define GZ80_DEBUG 1

#ifdef GZ80_DEBUG
    #define GZ80_DISASM 1
#endif

namespace gz80
{
    
typedef uint8_t u8;
typedef int8_t s8;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t u32;
typedef int32_t s32;
typedef uint64_t u64;
typedef int64_t s64;

#define SafeDelete(pointer) if(pointer != NULL) {delete pointer; pointer = NULL;}
#define SafeDeleteArray(pointer) if(pointer != NULL) {delete [] pointer; pointer = NULL;}

#define InitPointer(pointer) ((pointer) = NULL)
#define IsValidPointer(pointer) ((pointer) != NULL)

#define FLAG_CARRY 0x01
#define FLAG_NEGATIVE 0x02
#define FLAG_PARITY 0x04
#define FLAG_X 0x08
#define FLAG_HALF 0x10
#define FLAG_Y 0x20
#define FLAG_ZERO 0x40
#define FLAG_SIGN 0x80
#define FLAG_NONE 0

#ifdef GZ80_DEBUG
    #define Log(msg, ...) (LogImpl(msg, ##__VA_ARGS__))
#else
    #define Log(msg, ...)  
#endif

inline void LogImpl(const char* const msg, ...)
{
    static int count = 1;
    char szBuf[512];
    va_list args;
    va_start(args, msg);
    vsprintf(szBuf, msg, args);
    va_end(args);
    printf("%d: %s\n", count, szBuf);
    count++;
}

inline u8 SetBit(const u8 value, const u8 bit)
{
    return value | (0x01 << bit);
}

inline u8 UnsetBit(const u8 value, const u8 bit)
{
    return value & (~(0x01 << bit));
}

inline bool IsSetBit(const u8 value, const u8 bit)
{
    return (value & (0x01 << bit)) != 0;
}

inline u8 FlipBit(const u8 value, const u8 bit)
{
    return value ^ (0x01 << bit);
}

} // namespace gz80

#endif // GZ80_DEFINITIONS_H_

