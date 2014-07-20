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

#ifndef GZ80_EIGHTBITREGISTER_H_
#define	GZ80_EIGHTBITREGISTER_H_

#include "gz80_definitions.h"

namespace gz80
{
   
class EightBitRegister
{
public:
    EightBitRegister() : value_(0) { }
    void SetValue(u8 value);
    u8 GetValue() const;
    void Increment();
    void Decrement();

private:
    u8 value_;
};


inline void EightBitRegister::SetValue(u8 value)
{
    this->value_ = value;
}

inline u8 EightBitRegister::GetValue() const
{
    return value_;
}

inline void EightBitRegister::Increment()
{
    value_++;
}

inline void EightBitRegister::Decrement()
{
    value_--;
}

} // namespace gz80

#endif // GZ80_EIGHTBITREGISTER_H_

