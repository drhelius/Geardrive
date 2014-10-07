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

#ifndef GZ80_COREINL_H_
#define	GZ80_COREINL_H_

#include "gz80_definitions.h"
#include "gz80_eight_bit_register.h"
#include "gz80_sixteen_bit_register.h"
#include "gz80_opcode_timing.h"
#include "gz80_opcode_names.h"

namespace gz80
{
   
inline u8 GZ80::FetchOPCode()
{
    u8 opcode = memory_impl_->Read(PC_.GetValue());
    PC_.Increment();
    return opcode;
}

inline u16 GZ80::FetchArg16()
{
    u16 pc = PC_.GetValue();
    u8 l = memory_impl_->Read(pc);
    u8 h = memory_impl_->Read(pc + 1);
    PC_.SetValue(pc + 2);
    return (h << 8) | l;
}

inline void GZ80::LeaveHalt()
{
    if (halt_)
    {
        halt_ = false;
        PC_.Increment();
    }
}

inline void GZ80::ClearAllFlags()
{
    SetFlag(FLAG_NONE);
}

inline void GZ80::ToggleZeroFlagFromResult(u16 result)
{
    if (result == 0)
        ToggleFlag(FLAG_ZERO);
    else
        ClearFlag(FLAG_ZERO);
}

inline void GZ80::ToggleSignFlagFromResult(u8 result)
{
    if ((result & 0x80) != 0)
        ToggleFlag(FLAG_SIGN);
    else
        ClearFlag(FLAG_SIGN);
}

inline void GZ80::ToggleXYFlagsFromResult(u8 result)
{
    if ((result & 0x08) != 0)
        ToggleFlag(FLAG_X);
    else
        ClearFlag(FLAG_X);
    if ((result & 0x20) != 0)
        ToggleFlag(FLAG_Y);
    else
        ClearFlag(FLAG_Y);
}

inline void GZ80::ToggleParityFlagFromResult(u8 result)
{
    if (kZ80ParityTable[result])
        ToggleFlag(FLAG_PARITY);
    else
        ClearFlag(FLAG_PARITY);
}

inline void GZ80::SetFlag(u8 flag)
{
    AF_.SetLow(flag);
}

inline void GZ80::FlipFlag(u8 flag)
{
    AF_.SetLow(AF_.GetLow() ^ flag);
}

inline void GZ80::ToggleFlag(u8 flag)
{
    AF_.SetLow(AF_.GetLow() | flag);
}

inline void GZ80::ClearFlag(u8 flag)
{
    AF_.SetLow(AF_.GetLow() & (~flag));
}

inline bool GZ80::IsSetFlag(u8 flag)
{
    return (AF_.GetLow() & flag) != 0;
}

inline void GZ80::StackPush(SixteenBitRegister* reg)
{
    SP_.Decrement();
    memory_impl_->Write(SP_.GetValue(), reg->GetHigh());
    SP_.Decrement();
    memory_impl_->Write(SP_.GetValue(), reg->GetLow());
}

inline void GZ80::StackPop(SixteenBitRegister* reg)
{
    reg->SetLow(memory_impl_->Read(SP_.GetValue()));
    SP_.Increment();
    reg->SetHigh(memory_impl_->Read(SP_.GetValue()));
    SP_.Increment();
}

inline void GZ80::SetInterruptMode(int mode)
{
    if (mode == 1)
    {
        interrupt_mode_ = mode;
    }
    else
    {
        Log("--> ** Attempting to set interrupt mode %d", mode);
    }
}

inline SixteenBitRegister* GZ80::GetPrefixedRegister()
{
    switch (actual_prefix_)
    {
        case 0xDD:
            return &IX_;
        case 0xFD:
            return &IY_;
        default:
            return &HL_;
    }
}

inline u16 GZ80::GetEffectiveAddress()
{
    switch (actual_prefix_)
    {
        case 0xDD:
        {
            u16 address = IX_.GetValue();
            if (prefixed_cb_opcode_)
            {
                address += static_cast<s8> (prefixed_cb_value_);
            }
            else
            {
                address += static_cast<s8> (memory_impl_->Read(PC_.GetValue()));
                PC_.Increment();
                XY_.SetValue(address);
            }
            return address;
        }
        case 0xFD:
        {
            u16 address = IY_.GetValue();
            if (prefixed_cb_opcode_)
            {
                address += static_cast<s8> (prefixed_cb_value_);
            }
            else
            {
                address += static_cast<s8> (memory_impl_->Read(PC_.GetValue()));
                PC_.Increment();
                XY_.SetValue(address);
            }
            return address;
        }
        default:
            return HL_.GetValue();
    }
}

inline bool GZ80::IsPrefixedInstruction()
{
    return (actual_prefix_ == 0xDD) || (actual_prefix_ == 0xFD);
}

inline void GZ80::IncreaseR()
{
    u8 r = R_.GetValue();
    R_.SetValue(((r + 1) & 0x7F) | (r & 0x80));
}

inline void GZ80::OPCodes_LD(EightBitRegister* reg1, u8 value)
{
    reg1->SetValue(value);
}

inline void GZ80::OPCodes_LD(EightBitRegister* reg, u16 address)
{
    reg->SetValue(memory_impl_->Read(address));
}

inline void GZ80::OPCodes_LD(u16 address, u8 reg)
{
    memory_impl_->Write(address, reg);
}

inline void GZ80::OPCodes_LD_dd_nn(SixteenBitRegister* reg)
{
    u16 address = FetchArg16();
    reg->SetLow(memory_impl_->Read(address));
    address++;
    reg->SetHigh(memory_impl_->Read(address));
    XY_.SetValue(address);
}

inline void GZ80::OPCodes_LD_nn_dd(SixteenBitRegister* reg)
{
    u16 address = FetchArg16();
    memory_impl_->Write(address, reg->GetLow());
    address++;
    memory_impl_->Write(address, reg->GetHigh());
    XY_.SetValue(address);
}

inline void GZ80::OPCodes_LDI()
{
    u8 result = memory_impl_->Read(HL_.GetValue());
    memory_impl_->Write(DE_.GetValue(), result);
    DE_.Increment();
    HL_.Increment();
    BC_.Decrement();
    ClearFlag(FLAG_NEGATIVE);
    ClearFlag(FLAG_HALF);
    if (BC_.GetValue() != 0)
        ToggleFlag(FLAG_PARITY);
    else
        ClearFlag(FLAG_PARITY);
    u16 n = AF_.GetHigh() + result;
    if ((n & 0x08) != 0)
        ToggleFlag(FLAG_X);
    else
        ClearFlag(FLAG_X);
    if ((n & 0x02) != 0)
        ToggleFlag(FLAG_Y);
    else
        ClearFlag(FLAG_Y);
}

inline void GZ80::OPCodes_LDD()
{
    u8 result = memory_impl_->Read(HL_.GetValue());
    memory_impl_->Write(DE_.GetValue(), result);
    DE_.Decrement();
    HL_.Decrement();
    BC_.Decrement();
    ClearFlag(FLAG_NEGATIVE);
    ClearFlag(FLAG_HALF);
    if (BC_.GetValue() != 0)
        ToggleFlag(FLAG_PARITY);
    else
        ClearFlag(FLAG_PARITY);
    u16 n = AF_.GetHigh() + result;
    if ((n & 0x08) != 0)
        ToggleFlag(FLAG_X);
    else
        ClearFlag(FLAG_X);
    if ((n & 0x02) != 0)
        ToggleFlag(FLAG_Y);
    else
        ClearFlag(FLAG_Y);
}

inline void GZ80::OPCodes_RST(u16 address)
{
    StackPush(&PC_);
    PC_.SetValue(address);
    XY_.SetValue(address);
}

inline void GZ80::OPCodes_CALL_nn()
{
    u16 address = FetchArg16();
    StackPush(&PC_);
    PC_.SetValue(address);
    XY_.SetValue(address);
}

inline void GZ80::OPCodes_CALL_nn_Conditional(bool condition)
{
    u16 address = FetchArg16();
    if (condition)
    {
        StackPush(&PC_);
        PC_.SetValue(address);
        branched_ = true;
    }
    XY_.SetValue(address);
}

inline void GZ80::OPCodes_JP_nn()
{
    u8 l = memory_impl_->Read(PC_.GetValue());
    u8 h = memory_impl_->Read(PC_.GetValue() + 1);
    u16 address = (h << 8) | l;
    PC_.SetValue(address);
    XY_.SetValue(address);
}

inline void GZ80::OPCodes_JP_nn_Conditional(bool condition)
{
    u8 l = memory_impl_->Read(PC_.GetValue());
    u8 h = memory_impl_->Read(PC_.GetValue() + 1);
    u16 address = (h << 8) | l;
    if (condition)
    {
        PC_.SetValue(address);
        branched_ = true;
    }
    else
    {
        PC_.Increment();
        PC_.Increment();
    }
    XY_.SetValue(address);
}

inline void GZ80::OPCodes_JR_n()
{
    u16 pc = PC_.GetValue();
    PC_.SetValue(pc + 1 + (static_cast<s8> (memory_impl_->Read(pc))));
}

inline void GZ80::OPCodes_JR_n_conditional(bool condition)
{
    if (condition)
    {
        OPCodes_JR_n();
        branched_ = true;
    }
    else
        PC_.Increment();
}

inline void GZ80::OPCodes_RET()
{
    StackPop(&PC_);
    XY_.SetValue(PC_.GetValue());
}

inline void GZ80::OPCodes_RET_Conditional(bool condition)
{
    if (condition)
    {
        OPCodes_RET();
        branched_ = true;
    }
}

inline void GZ80::OPCodes_IN_C(EightBitRegister* reg)
{
    u8 result = io_ports_impl_->Input(BC_.GetLow());
    if (IsValidPointer(reg))
        reg->SetValue(result);
    IsSetFlag(FLAG_CARRY) ? SetFlag(FLAG_CARRY) : ClearAllFlags();
    ToggleZeroFlagFromResult(result);
    ToggleSignFlagFromResult(result);
    ToggleParityFlagFromResult(result);
    ToggleXYFlagsFromResult(result);
}

inline void GZ80::OPCodes_INI()
{
    XY_.SetValue(BC_.GetValue() + 1);
    u8 result = io_ports_impl_->Input(BC_.GetLow());
    memory_impl_->Write(HL_.GetValue(), result);
    OPCodes_DEC(BC_.GetHighRegister());
    HL_.Increment();
    if ((result & 0x80) != 0)
        ToggleFlag(FLAG_NEGATIVE);
    else
        ClearFlag(FLAG_NEGATIVE);
    if ((result + ((BC_.GetLow() + 1) & 0xFF)) > 0xFF)
    {
        ToggleFlag(FLAG_CARRY);
        ToggleFlag(FLAG_HALF);
    }
    else
    {
        ClearFlag(FLAG_CARRY);
        ClearFlag(FLAG_HALF);
    }
    if (((result + ((BC_.GetLow() + 1) & 0xFF)) & 0x07) ^ BC_.GetHigh())
        ToggleFlag(FLAG_PARITY);
    else
        ClearFlag(FLAG_PARITY);
}

inline void GZ80::OPCodes_IND()
{
    XY_.SetValue(BC_.GetValue() - 1);
    u8 result = io_ports_impl_->Input(BC_.GetLow());
    memory_impl_->Write(HL_.GetValue(), result);
    OPCodes_DEC(BC_.GetHighRegister());
    HL_.Decrement();
    if ((result & 0x80) != 0)
        ToggleFlag(FLAG_NEGATIVE);
    else
        ClearFlag(FLAG_NEGATIVE);
    if ((result + ((BC_.GetLow() - 1) & 0xFF)) > 0xFF)
    {
        ToggleFlag(FLAG_CARRY);
        ToggleFlag(FLAG_HALF);
    }
    else
    {
        ClearFlag(FLAG_CARRY);
        ClearFlag(FLAG_HALF);
    }
    if (((result + ((BC_.GetLow() + 1) & 0xFF)) & 0x07) ^ BC_.GetHigh())
        ToggleFlag(FLAG_PARITY);
    else
        ClearFlag(FLAG_PARITY);
}

inline void GZ80::OPCodes_OUT_C(EightBitRegister* reg)
{
    io_ports_impl_->Output(BC_.GetLow(), reg->GetValue());
}

inline void GZ80::OPCodes_OUTI()
{
    u8 result = memory_impl_->Read(HL_.GetValue());
    io_ports_impl_->Output(BC_.GetLow(), result);
    OPCodes_DEC(BC_.GetHighRegister());
    XY_.SetValue(BC_.GetValue() + 1);
    HL_.Increment();
    if ((result & 0x80) != 0)
        ToggleFlag(FLAG_NEGATIVE);
    else
        ClearFlag(FLAG_NEGATIVE);
    if ((HL_.GetLow() + result) > 0xFF)
    {
        ToggleFlag(FLAG_CARRY);
        ToggleFlag(FLAG_HALF);
    }
    else
    {
        ClearFlag(FLAG_CARRY);
        ClearFlag(FLAG_HALF);
    }
    if (((HL_.GetLow() + result) & 0x07) ^ BC_.GetHigh())
        ToggleFlag(FLAG_PARITY);
    else
        ClearFlag(FLAG_PARITY);
}

inline void GZ80::OPCodes_OUTD()
{
    u8 result = memory_impl_->Read(HL_.GetValue());
    io_ports_impl_->Output(BC_.GetLow(), result);
    OPCodes_DEC(BC_.GetHighRegister());
    XY_.SetValue(BC_.GetValue() - 1);
    HL_.Decrement();
    if ((result & 0x80) != 0)
        ToggleFlag(FLAG_NEGATIVE);
    else
        ClearFlag(FLAG_NEGATIVE);
    if ((HL_.GetLow() + result) > 0xFF)
    {
        ToggleFlag(FLAG_CARRY);
        ToggleFlag(FLAG_HALF);
    }
    else
    {
        ClearFlag(FLAG_CARRY);
        ClearFlag(FLAG_HALF);
    }
    if (((HL_.GetLow() + result) & 0x07) ^ BC_.GetHigh())
        ToggleFlag(FLAG_PARITY);
    else
        ClearFlag(FLAG_PARITY);
}

inline void GZ80::OPCodes_EX(SixteenBitRegister* reg1, SixteenBitRegister* reg2)
{
    u16 tmp = reg1->GetValue();
    reg1->SetValue(reg2->GetValue());
    reg2->SetValue(tmp);
}

inline void GZ80::OPCodes_OR(u8 number)
{
    u8 result = AF_.GetHigh() | number;
    AF_.SetHigh(result);
    ClearAllFlags();
    ToggleZeroFlagFromResult(result);
    ToggleSignFlagFromResult(result);
    ToggleXYFlagsFromResult(result);
    ToggleParityFlagFromResult(result);
}

inline void GZ80::OPCodes_XOR(u8 number)
{
    u8 result = AF_.GetHigh() ^ number;
    AF_.SetHigh(result);
    ClearAllFlags();
    ToggleZeroFlagFromResult(result);
    ToggleSignFlagFromResult(result);
    ToggleXYFlagsFromResult(result);
    ToggleParityFlagFromResult(result);
}

inline void GZ80::OPCodes_AND(u8 number)
{
    u8 result = AF_.GetHigh() & number;
    AF_.SetHigh(result);
    SetFlag(FLAG_HALF);
    ToggleZeroFlagFromResult(result);
    ToggleSignFlagFromResult(result);
    ToggleXYFlagsFromResult(result);
    ToggleParityFlagFromResult(result);
}

inline void GZ80::OPCodes_CP(u8 number)
{
    int result = AF_.GetHigh() - number;
    int carrybits = AF_.GetHigh() ^ number ^ result;
    u8 final_result = static_cast<u8> (result);
    SetFlag(FLAG_NEGATIVE);
    ToggleZeroFlagFromResult(final_result);
    ToggleSignFlagFromResult(final_result);
    ToggleXYFlagsFromResult(number);
    if ((carrybits & 0x100) != 0)
        ToggleFlag(FLAG_CARRY);
    if ((carrybits & 0x10) != 0)
        ToggleFlag(FLAG_HALF);
    if ((((carrybits << 1) ^ carrybits) & 0x100) != 0)
        ToggleFlag(FLAG_PARITY);
}

inline void GZ80::OPCodes_CPI()
{
    u8 number = memory_impl_->Read(HL_.GetValue());
    int result = AF_.GetHigh() - number;
    int carrybits = AF_.GetHigh() ^ number ^ result;
    u8 final_result = static_cast<u8> (result);
    ToggleFlag(FLAG_NEGATIVE);
    ToggleZeroFlagFromResult(final_result);
    ToggleSignFlagFromResult(final_result);
    if ((carrybits & 0x10) != 0)
        ToggleFlag(FLAG_HALF);
    else
        ClearFlag(FLAG_HALF);
    HL_.Increment();
    BC_.Decrement();
    if (BC_.GetValue() != 0)
        ToggleFlag(FLAG_PARITY);
    else
        ClearFlag(FLAG_PARITY);
    int n = AF_.GetHigh() - number - (IsSetFlag(FLAG_HALF) ? 1 : 0);
    if ((n & 0x08) != 0)
        ToggleFlag(FLAG_X);
    else
        ClearFlag(FLAG_X);
    if ((n & 0x02) != 0)
        ToggleFlag(FLAG_Y);
    else
        ClearFlag(FLAG_Y);
    XY_.Increment();
}

inline void GZ80::OPCodes_CPD()
{
    u8 number = memory_impl_->Read(HL_.GetValue());
    int result = AF_.GetHigh() - number;
    int carrybits = AF_.GetHigh() ^ number ^ result;
    u8 final_result = static_cast<u8> (result);
    ToggleFlag(FLAG_NEGATIVE);
    ToggleZeroFlagFromResult(final_result);
    ToggleSignFlagFromResult(final_result);
    if ((carrybits & 0x10) != 0)
        ToggleFlag(FLAG_HALF);
    else
        ClearFlag(FLAG_HALF);
    HL_.Decrement();
    BC_.Decrement();
    if (BC_.GetValue() != 0)
        ToggleFlag(FLAG_PARITY);
    else
        ClearFlag(FLAG_PARITY);
    int n = AF_.GetHigh() - number - (IsSetFlag(FLAG_HALF) ? 1 : 0);
    if ((n & 0x08) != 0)
        ToggleFlag(FLAG_X);
    else
        ClearFlag(FLAG_X);
    if ((n & 0x02) != 0)
        ToggleFlag(FLAG_Y);
    else
        ClearFlag(FLAG_Y);
    XY_.Decrement();
}

inline void GZ80::OPCodes_INC(EightBitRegister* reg)
{
    u8 result = reg->GetValue() + 1;
    reg->SetValue(result);
    IsSetFlag(FLAG_CARRY) ? SetFlag(FLAG_CARRY) : ClearAllFlags();
    ToggleZeroFlagFromResult(result);
    ToggleSignFlagFromResult(result);
    ToggleXYFlagsFromResult(result);
    if ((result & 0x0F) == 0x00)
        ToggleFlag(FLAG_HALF);
    if (result == 0x80)
        ToggleFlag(FLAG_PARITY);
}

inline void GZ80::OPCodes_INC_HL()
{
    u16 address = GetEffectiveAddress();
    u8 result = memory_impl_->Read(address) + 1;
    memory_impl_->Write(address, result);
    IsSetFlag(FLAG_CARRY) ? SetFlag(FLAG_CARRY) : ClearAllFlags();
    ToggleZeroFlagFromResult(result);
    ToggleSignFlagFromResult(result);
    ToggleXYFlagsFromResult(result);
    if ((result & 0x0F) == 0x00)
        ToggleFlag(FLAG_HALF);
    if (result == 0x80)
        ToggleFlag(FLAG_PARITY);
}

inline void GZ80::OPCodes_DEC(EightBitRegister* reg)
{
    u8 result = reg->GetValue() - 1;
    reg->SetValue(result);
    IsSetFlag(FLAG_CARRY) ? SetFlag(FLAG_CARRY) : ClearAllFlags();
    ToggleFlag(FLAG_NEGATIVE);
    ToggleZeroFlagFromResult(result);
    ToggleSignFlagFromResult(result);
    ToggleXYFlagsFromResult(result);
    if ((result & 0x0F) == 0x0F)
        ToggleFlag(FLAG_HALF);
    if (result == 0x7F)
        ToggleFlag(FLAG_PARITY);
}

inline void GZ80::OPCodes_DEC_HL()
{
    u16 address = GetEffectiveAddress();
    u8 result = memory_impl_->Read(address) - 1;
    memory_impl_->Write(address, result);
    IsSetFlag(FLAG_CARRY) ? SetFlag(FLAG_CARRY) : ClearAllFlags();
    ToggleFlag(FLAG_NEGATIVE);
    ToggleZeroFlagFromResult(result);
    ToggleSignFlagFromResult(result);
    ToggleXYFlagsFromResult(result);
    if ((result & 0x0F) == 0x0F)
        ToggleFlag(FLAG_HALF);
    if (result == 0x7F)
        ToggleFlag(FLAG_PARITY);
}

inline void GZ80::OPCodes_ADD(u8 number)
{
    int result = AF_.GetHigh() + number;
    int carrybits = AF_.GetHigh() ^ number ^ result;
    u8 final_result = static_cast<u8> (result);
    AF_.SetHigh(final_result);
    ClearAllFlags();
    ToggleZeroFlagFromResult(final_result);
    ToggleSignFlagFromResult(final_result);
    ToggleXYFlagsFromResult(final_result);
    if ((carrybits & 0x100) != 0)
        ToggleFlag(FLAG_CARRY);
    if ((carrybits & 0x10) != 0)
        ToggleFlag(FLAG_HALF);
    if ((((carrybits << 1) ^ carrybits) & 0x100) != 0)
        ToggleFlag(FLAG_PARITY);
}

inline void GZ80::OPCodes_ADC(u8 number)
{
    int result = AF_.GetHigh() + number + (IsSetFlag(FLAG_CARRY) ? 1 : 0);
    int carrybits = AF_.GetHigh() ^ number ^ result;
    u8 final_result = static_cast<u8> (result);
    AF_.SetHigh(final_result);
    ClearAllFlags();
    ToggleZeroFlagFromResult(final_result);
    ToggleSignFlagFromResult(final_result);
    ToggleXYFlagsFromResult(final_result);
    if ((carrybits & 0x100) != 0)
        ToggleFlag(FLAG_CARRY);
    if ((carrybits & 0x10) != 0)
        ToggleFlag(FLAG_HALF);
    if ((((carrybits << 1) ^ carrybits) & 0x100) != 0)
        ToggleFlag(FLAG_PARITY);
}

inline void GZ80::OPCodes_SUB(u8 number)
{
    int result = AF_.GetHigh() - number;
    int carrybits = AF_.GetHigh() ^ number ^ result;
    u8 final_result = static_cast<u8> (result);
    AF_.SetHigh(final_result);
    SetFlag(FLAG_NEGATIVE);
    ToggleZeroFlagFromResult(final_result);
    ToggleSignFlagFromResult(final_result);
    ToggleXYFlagsFromResult(final_result);
    if ((carrybits & 0x100) != 0)
        ToggleFlag(FLAG_CARRY);
    if ((carrybits & 0x10) != 0)
        ToggleFlag(FLAG_HALF);
    if ((((carrybits << 1) ^ carrybits) & 0x100) != 0)
        ToggleFlag(FLAG_PARITY);
}

inline void GZ80::OPCodes_SBC(u8 number)
{
    int result = AF_.GetHigh() - number - (IsSetFlag(FLAG_CARRY) ? 1 : 0);
    int carrybits = AF_.GetHigh() ^ number ^ result;
    u8 final_result = static_cast<u8> (result);
    AF_.SetHigh(final_result);
    SetFlag(FLAG_NEGATIVE);
    ToggleZeroFlagFromResult(final_result);
    ToggleSignFlagFromResult(final_result);
    ToggleXYFlagsFromResult(final_result);
    if ((carrybits & 0x100) != 0)
        ToggleFlag(FLAG_CARRY);
    if ((carrybits & 0x10) != 0)
        ToggleFlag(FLAG_HALF);
    if ((((carrybits << 1) ^ carrybits) & 0x100) != 0)
        ToggleFlag(FLAG_PARITY);
}

inline void GZ80::OPCodes_ADD_HL(u16 number)
{
    SixteenBitRegister* reg = GetPrefixedRegister();
    XY_.SetValue(reg->GetValue() + 1);
    int result = reg->GetValue() + number;
    int carrybits = reg->GetValue() ^ number ^ result;
    reg->SetValue(static_cast<u16> (result));
    ClearFlag(FLAG_NEGATIVE);
    ToggleXYFlagsFromResult(reg->GetHigh());
    if ((carrybits & 0x10000) != 0)
        ToggleFlag(FLAG_CARRY);
    else
        ClearFlag(FLAG_CARRY);
    if ((carrybits & 0x1000) != 0)
        ToggleFlag(FLAG_HALF);
    else
        ClearFlag(FLAG_HALF);
}

inline void GZ80::OPCodes_ADC_HL(u16 number)
{
    XY_.SetValue(HL_.GetValue() + 1);
    int result = HL_.GetValue() + number + (IsSetFlag(FLAG_CARRY) ? 1 : 0);
    int carrybits = HL_.GetValue() ^ number ^ result;
    u16 final_result = static_cast<u16> (result);
    HL_.SetValue(final_result);
    ClearAllFlags();
    ToggleXYFlagsFromResult(HL_.GetHigh());
    ToggleSignFlagFromResult(HL_.GetHigh());
    ToggleZeroFlagFromResult(final_result);
    if ((carrybits & 0x10000) != 0)
        ToggleFlag(FLAG_CARRY);
    if ((carrybits & 0x1000) != 0)
        ToggleFlag(FLAG_HALF);
    if ((((carrybits << 1) ^ carrybits) & 0x10000) != 0)
        ToggleFlag(FLAG_PARITY);
}

inline void GZ80::OPCodes_SBC_HL(u16 number)
{
    XY_.SetValue(HL_.GetValue() + 1);
    int result = HL_.GetValue() - number - (IsSetFlag(FLAG_CARRY) ? 1 : 0);
    int carrybits = HL_.GetValue() ^ number ^ result;
    u16 final_result = static_cast<u16> (result);
    HL_.SetValue(final_result);
    SetFlag(FLAG_NEGATIVE);
    ToggleXYFlagsFromResult(HL_.GetHigh());
    ToggleSignFlagFromResult(HL_.GetHigh());
    ToggleZeroFlagFromResult(final_result);
    if ((carrybits & 0x10000) != 0)
        ToggleFlag(FLAG_CARRY);
    if ((carrybits & 0x1000) != 0)
        ToggleFlag(FLAG_HALF);
    if ((((carrybits << 1) ^ carrybits) & 0x10000) != 0)
        ToggleFlag(FLAG_PARITY);
}

inline void GZ80::OPCodes_SLL(EightBitRegister* reg)
{
    u16 address = 0x0000;
    if (IsPrefixedInstruction())
    {
        address = GetEffectiveAddress();
        reg->SetValue(memory_impl_->Read(address));
    }
    (reg->GetValue() & 0x80) != 0 ? SetFlag(FLAG_CARRY) : ClearAllFlags();
    u8 result = (reg->GetValue() << 1) | 0x01;
    reg->SetValue(result);
    if (IsPrefixedInstruction())
        memory_impl_->Write(address, reg->GetValue());
    ToggleZeroFlagFromResult(result);
    ToggleSignFlagFromResult(result);
    ToggleParityFlagFromResult(result);
    ToggleXYFlagsFromResult(result);
}

inline void GZ80::OPCodes_SLL_HL()
{
    u16 address = GetEffectiveAddress();
    u8 result = memory_impl_->Read(address);
    (result & 0x80) != 0 ? SetFlag(FLAG_CARRY) : ClearAllFlags();
    result = (result << 1) | 0x01;
    memory_impl_->Write(address, result);
    ToggleZeroFlagFromResult(result);
    ToggleSignFlagFromResult(result);
    ToggleParityFlagFromResult(result);
    ToggleXYFlagsFromResult(result);
}

inline void GZ80::OPCodes_SLA(EightBitRegister* reg)
{
    u16 address = 0x0000;
    if (IsPrefixedInstruction())
    {
        address = GetEffectiveAddress();
        reg->SetValue(memory_impl_->Read(address));
    }
    (reg->GetValue() & 0x80) != 0 ? SetFlag(FLAG_CARRY) : ClearAllFlags();
    u8 result = reg->GetValue() << 1;
    reg->SetValue(result);
    if (IsPrefixedInstruction())
        memory_impl_->Write(address, reg->GetValue());
    ToggleZeroFlagFromResult(result);
    ToggleSignFlagFromResult(result);
    ToggleParityFlagFromResult(result);
    ToggleXYFlagsFromResult(result);
}

inline void GZ80::OPCodes_SLA_HL()
{
    u16 address = GetEffectiveAddress();
    u8 result = memory_impl_->Read(address);
    (result & 0x80) != 0 ? SetFlag(FLAG_CARRY) : ClearAllFlags();
    result <<= 1;
    memory_impl_->Write(address, result);
    ToggleZeroFlagFromResult(result);
    ToggleSignFlagFromResult(result);
    ToggleParityFlagFromResult(result);
    ToggleXYFlagsFromResult(result);
}

inline void GZ80::OPCodes_SRA(EightBitRegister* reg)
{
    u16 address = 0x0000;
    if (IsPrefixedInstruction())
    {
        address = GetEffectiveAddress();
        reg->SetValue(memory_impl_->Read(address));
    }
    u8 result = reg->GetValue();
    (result & 0x01) != 0 ? SetFlag(FLAG_CARRY) : ClearAllFlags();
    if ((result & 0x80) != 0)
    {
        result >>= 1;
        result |= 0x80;
    }
    else
        result >>= 1;
    reg->SetValue(result);
    if (IsPrefixedInstruction())
        memory_impl_->Write(address, reg->GetValue());
    ToggleZeroFlagFromResult(result);
    ToggleSignFlagFromResult(result);
    ToggleParityFlagFromResult(result);
    ToggleXYFlagsFromResult(result);
}

inline void GZ80::OPCodes_SRA_HL()
{
    u16 address = GetEffectiveAddress();
    u8 result = memory_impl_->Read(address);
    (result & 0x01) != 0 ? SetFlag(FLAG_CARRY) : ClearAllFlags();
    if ((result & 0x80) != 0)
    {
        result >>= 1;
        result |= 0x80;
    }
    else
        result >>= 1;
    memory_impl_->Write(address, result);
    ToggleZeroFlagFromResult(result);
    ToggleSignFlagFromResult(result);
    ToggleParityFlagFromResult(result);
    ToggleXYFlagsFromResult(result);
}

inline void GZ80::OPCodes_SRL(EightBitRegister* reg)
{
    u16 address = 0x0000;
    if (IsPrefixedInstruction())
    {
        address = GetEffectiveAddress();
        reg->SetValue(memory_impl_->Read(address));
    }
    u8 result = reg->GetValue();
    (result & 0x01) != 0 ? SetFlag(FLAG_CARRY) : ClearAllFlags();
    result >>= 1;
    reg->SetValue(result);
    if (IsPrefixedInstruction())
        memory_impl_->Write(address, reg->GetValue());
    ToggleZeroFlagFromResult(result);
    ToggleSignFlagFromResult(result);
    ToggleParityFlagFromResult(result);
    ToggleXYFlagsFromResult(result);
}

inline void GZ80::OPCodes_SRL_HL()
{
    u16 address = GetEffectiveAddress();
    u8 result = memory_impl_->Read(address);
    (result & 0x01) != 0 ? SetFlag(FLAG_CARRY) : ClearAllFlags();
    result >>= 1;
    memory_impl_->Write(address, result);
    ToggleZeroFlagFromResult(result);
    ToggleSignFlagFromResult(result);
    ToggleParityFlagFromResult(result);
    ToggleXYFlagsFromResult(result);
}

inline void GZ80::OPCodes_RLC(EightBitRegister* reg, bool isRegisterA)
{
    u16 address = 0x0000;
    if (!isRegisterA && IsPrefixedInstruction())
    {
        address = GetEffectiveAddress();
        reg->SetValue(memory_impl_->Read(address));
    }
    u8 result = reg->GetValue();
    if ((result & 0x80) != 0)
    {
        ToggleFlag(FLAG_CARRY);
        result <<= 1;
        result |= 0x1;
    }
    else
    {
        ClearFlag(FLAG_CARRY);
        result <<= 1;
    }
    reg->SetValue(result);
    if (!isRegisterA && IsPrefixedInstruction())
        memory_impl_->Write(address, reg->GetValue());
    ClearFlag(FLAG_HALF);
    ClearFlag(FLAG_NEGATIVE);
    ToggleXYFlagsFromResult(result);
    if (!isRegisterA)
    {
        ToggleZeroFlagFromResult(result);
        ToggleSignFlagFromResult(result);
        ToggleParityFlagFromResult(result);
    }
}

inline void GZ80::OPCodes_RLC_HL()
{
    u16 address = GetEffectiveAddress();
    u8 result = memory_impl_->Read(address);
    if ((result & 0x80) != 0)
    {
        SetFlag(FLAG_CARRY);
        result <<= 1;
        result |= 0x1;
    }
    else
    {
        ClearAllFlags();
        result <<= 1;
    }
    memory_impl_->Write(address, result);
    ToggleZeroFlagFromResult(result);
    ToggleSignFlagFromResult(result);
    ToggleParityFlagFromResult(result);
    ToggleXYFlagsFromResult(result);
}

inline void GZ80::OPCodes_RL(EightBitRegister* reg, bool isRegisterA)
{
    u16 address = 0x0000;
    if (!isRegisterA && IsPrefixedInstruction())
    {
        address = GetEffectiveAddress();
        reg->SetValue(memory_impl_->Read(address));
    }
    u8 carry = IsSetFlag(FLAG_CARRY) ? 1 : 0;
    u8 result = reg->GetValue();
    ((result & 0x80) != 0) ? ToggleFlag(FLAG_CARRY) : ClearFlag(FLAG_CARRY);
    result <<= 1;
    result |= carry;
    reg->SetValue(result);
    if (!isRegisterA && IsPrefixedInstruction())
        memory_impl_->Write(address, reg->GetValue());
    ClearFlag(FLAG_HALF);
    ClearFlag(FLAG_NEGATIVE);
    ToggleXYFlagsFromResult(result);
    if (!isRegisterA)
    {
        ToggleZeroFlagFromResult(result);
        ToggleSignFlagFromResult(result);
        ToggleParityFlagFromResult(result);
    }
}

inline void GZ80::OPCodes_RL_HL()
{
    u16 address = GetEffectiveAddress();
    u8 carry = IsSetFlag(FLAG_CARRY) ? 1 : 0;
    u8 result = memory_impl_->Read(address);
    ((result & 0x80) != 0) ? SetFlag(FLAG_CARRY) : ClearAllFlags();
    result <<= 1;
    result |= carry;
    memory_impl_->Write(address, result);
    ToggleZeroFlagFromResult(result);
    ToggleSignFlagFromResult(result);
    ToggleParityFlagFromResult(result);
    ToggleXYFlagsFromResult(result);
}

inline void GZ80::OPCodes_RRC(EightBitRegister* reg, bool isRegisterA)
{
    u16 address = 0x0000;
    if (!isRegisterA && IsPrefixedInstruction())
    {
        address = GetEffectiveAddress();
        reg->SetValue(memory_impl_->Read(address));
    }
    u8 result = reg->GetValue();
    if ((result & 0x01) != 0)
    {
        ToggleFlag(FLAG_CARRY);
        result >>= 1;
        result |= 0x80;
    }
    else
    {
        ClearFlag(FLAG_CARRY);
        result >>= 1;
    }
    reg->SetValue(result);
    if (!isRegisterA && IsPrefixedInstruction())
        memory_impl_->Write(address, reg->GetValue());
    ClearFlag(FLAG_HALF);
    ClearFlag(FLAG_NEGATIVE);
    ToggleXYFlagsFromResult(result);
    if (!isRegisterA)
    {
        ToggleZeroFlagFromResult(result);
        ToggleSignFlagFromResult(result);
        ToggleParityFlagFromResult(result);
    }
}

inline void GZ80::OPCodes_RRC_HL()
{
    u16 address = GetEffectiveAddress();
    u8 result = memory_impl_->Read(address);
    if ((result & 0x01) != 0)
    {
        SetFlag(FLAG_CARRY);
        result >>= 1;
        result |= 0x80;
    }
    else
    {
        ClearAllFlags();
        result >>= 1;
    }
    memory_impl_->Write(address, result);
    ToggleZeroFlagFromResult(result);
    ToggleSignFlagFromResult(result);
    ToggleParityFlagFromResult(result);
    ToggleXYFlagsFromResult(result);
}

inline void GZ80::OPCodes_RR(EightBitRegister* reg, bool isRegisterA)
{
    u16 address = 0x0000;
    if (!isRegisterA && IsPrefixedInstruction())
    {
        address = GetEffectiveAddress();
        reg->SetValue(memory_impl_->Read(address));
    }
    u8 carry = IsSetFlag(FLAG_CARRY) ? 0x80 : 0x00;
    u8 result = reg->GetValue();
    ((result & 0x01) != 0) ? ToggleFlag(FLAG_CARRY) : ClearFlag(FLAG_CARRY);
    result >>= 1;
    result |= carry;
    reg->SetValue(result);
    if (!isRegisterA && IsPrefixedInstruction())
        memory_impl_->Write(address, reg->GetValue());
    ClearFlag(FLAG_HALF);
    ClearFlag(FLAG_NEGATIVE);
    ToggleXYFlagsFromResult(result);
    if (!isRegisterA)
    {
        ToggleZeroFlagFromResult(result);
        ToggleSignFlagFromResult(result);
        ToggleParityFlagFromResult(result);
    }
}

inline void GZ80::OPCodes_RR_HL()
{
    u16 address = GetEffectiveAddress();
    u8 carry = IsSetFlag(FLAG_CARRY) ? 0x80 : 0x00;
    u8 result = memory_impl_->Read(address);
    ((result & 0x01) != 0) ? SetFlag(FLAG_CARRY) : ClearAllFlags();
    result >>= 1;
    result |= carry;
    memory_impl_->Write(address, result);
    ToggleZeroFlagFromResult(result);
    ToggleSignFlagFromResult(result);
    ToggleParityFlagFromResult(result);
    ToggleXYFlagsFromResult(result);
}

inline void GZ80::OPCodes_BIT(EightBitRegister* reg, int bit)
{
    IsSetFlag(FLAG_CARRY) ? SetFlag(FLAG_CARRY) : ClearAllFlags();
    u8 value = reg->GetValue();
    if (IsPrefixedInstruction())
        value = memory_impl_->Read(GetEffectiveAddress());
    if (!IsSetBit(value, bit))
    {
        ToggleFlag(FLAG_ZERO);
        ToggleFlag(FLAG_PARITY);
    }    
    else if (bit == 7)
        ToggleFlag(FLAG_SIGN);
    if (IsSetBit(value, 3))
        ToggleFlag(FLAG_X);
    if (IsSetBit(value, 5))
        ToggleFlag(FLAG_Y);
    ToggleFlag(FLAG_HALF);
}

inline void GZ80::OPCodes_BIT_HL(int bit)
{
    IsSetFlag(FLAG_CARRY) ? SetFlag(FLAG_CARRY) : ClearAllFlags();
    u16 address = GetEffectiveAddress();
    if (!IsSetBit(memory_impl_->Read(address), bit))
    {
        ToggleFlag(FLAG_ZERO);
        ToggleFlag(FLAG_PARITY);
    }
    else if (bit == 7)
        ToggleFlag(FLAG_SIGN);
    u8 xy = IsPrefixedInstruction() ? ((address >> 8) & 0xFF) : XY_.GetHigh();
    if (IsSetBit(xy, 3))
        ToggleFlag(FLAG_X);
    if (IsSetBit(xy, 5))
        ToggleFlag(FLAG_Y);
    ToggleFlag(FLAG_HALF);
}

inline void GZ80::OPCodes_SET(EightBitRegister* reg, int bit)
{
    u16 address = 0x0000;
    if (IsPrefixedInstruction())
    {
        address = GetEffectiveAddress();
        reg->SetValue(memory_impl_->Read(address));
    }
    reg->SetValue(reg->GetValue() | (0x1 << bit));
    if (IsPrefixedInstruction())
        memory_impl_->Write(address, reg->GetValue());
}

inline void GZ80::OPCodes_SET_HL(int bit)
{
    u16 address = GetEffectiveAddress();
    u8 result = memory_impl_->Read(address);
    result |= (0x1 << bit);
    memory_impl_->Write(address, result);
}

inline void GZ80::OPCodes_RES(EightBitRegister* reg, int bit)
{
    u16 address = 0x0000;
    if (IsPrefixedInstruction())
    {
        address = GetEffectiveAddress();
        reg->SetValue(memory_impl_->Read(address));
    }
    reg->SetValue(reg->GetValue() & (~(0x1 << bit)));
    if (IsPrefixedInstruction())
        memory_impl_->Write(address, reg->GetValue());
}

inline void GZ80::OPCodes_RES_HL(int bit)
{
    u16 address = GetEffectiveAddress();
    u8 result = memory_impl_->Read(address);
    result &= ~(0x1 << bit);
    memory_impl_->Write(address, result);
}

} // namespace gz80

#endif // GZ80_COREINL_H_

