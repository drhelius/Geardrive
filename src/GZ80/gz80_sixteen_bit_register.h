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

#ifndef GZ80_SIXTEENBITREGISTER_H_
#define	GZ80_SIXTEENBITREGISTER_H_

#include "gz80_definitions.h"
#include "gz80_eight_bit_register.h"

namespace gz80
{
    
class SixteenBitRegister
{
public:

    SixteenBitRegister() { }
    void SetLow(u8 low);
    u8 GetLow() const;
    void SetHigh(u8 high);
    u8 GetHigh() const;
    EightBitRegister* GetHighRegister();
    EightBitRegister* GetLowRegister();
    void SetValue(u16 value);
    u16 GetValue() const;
    void Increment();
    void Decrement();

private:
    EightBitRegister high_;
    EightBitRegister low_;
};

inline void SixteenBitRegister::SetLow(u8 low)
{
    this->low_.SetValue(low);
}

inline u8 SixteenBitRegister::GetLow() const
{
    return low_.GetValue();
}

inline void SixteenBitRegister::SetHigh(u8 high)
{
    this->high_.SetValue(high);
}

inline u8 SixteenBitRegister::GetHigh() const
{
    return high_.GetValue();
}

inline EightBitRegister* SixteenBitRegister::GetHighRegister()
{
    return &high_;
}

inline EightBitRegister* SixteenBitRegister::GetLowRegister()
{
    return &low_;
}

inline void SixteenBitRegister::SetValue(u16 value)
{
    low_.SetValue(static_cast<u8> (value));
    high_.SetValue(static_cast<u8> (value >> 8));
}

inline u16 SixteenBitRegister::GetValue() const
{
    u8 high = high_.GetValue();
    u8 low = low_.GetValue();
    return (high << 8) | low;
}

inline void SixteenBitRegister::Increment()
{
    u16 value = this->GetValue();
    value++;
    this->SetValue(value);
}

inline void SixteenBitRegister::Decrement()
{
    u16 value = this->GetValue();
    value--;
    this->SetValue(value);
}

} // namespace gz80

#endif // GZ80_SIXTEENBITREGISTER_H_

