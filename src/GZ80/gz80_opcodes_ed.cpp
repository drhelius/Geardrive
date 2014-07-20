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

#include "gz80_core.h"

namespace gz80
{
   
void GZ80::OPCodeED0x40()
{
    // IN B,(C)
    OPCodes_IN_C(BC.GetHighRegister());
}

void GZ80::OPCodeED0x41()
{
    // OUT (C),B
    OPCodes_OUT_C(BC.GetHighRegister());
}

void GZ80::OPCodeED0x42()
{
    // SBC HL,BC
    OPCodes_SBC_HL(BC.GetValue());
}

void GZ80::OPCodeED0x43()
{
    // LD (nn),BC
    OPCodes_LD_nn_dd(&BC);
}

void GZ80::OPCodeED0x44()
{
    // NEG
    u8 value = AF.GetHigh();
    AF.SetHigh(0x00);
    OPCodes_SUB(value);
}

void GZ80::OPCodeED0x45()
{
    // RETN
    OPCodes_RET();
    iff1_ = iff2_;
}

void GZ80::OPCodeED0x46()
{
    // IM 0
    SetInterruptMode(0);
}

void GZ80::OPCodeED0x47()
{
    // LD I,A
    OPCodes_LD(&I, AF.GetHigh());
}

void GZ80::OPCodeED0x48()
{
    // IN C,(C)
    OPCodes_IN_C(BC.GetLowRegister());
}

void GZ80::OPCodeED0x49()
{
    // OUT (C),C
    OPCodes_OUT_C(BC.GetLowRegister());
}

void GZ80::OPCodeED0x4A()
{
    // ADC HL,BC
    OPCodes_ADC_HL(BC.GetValue());
}

void GZ80::OPCodeED0x4B()
{
    // LD BC,(nn)
    OPCodes_LD_dd_nn(&BC);
}

void GZ80::OPCodeED0x4C()
{
    // NEG*
    UndocumentedOPCode();
    OPCodeED0x44();
}

void GZ80::OPCodeED0x4D()
{
    // RETI
    OPCodes_RET();
    iff1_ = iff2_;
}

void GZ80::OPCodeED0x4E()
{
    // IM 0*
    UndocumentedOPCode();
    OPCodeED0x46();
}

void GZ80::OPCodeED0x4F()
{
    // LD R,A
    OPCodes_LD(&R, AF.GetHigh());
}

void GZ80::OPCodeED0x50()
{
    // IN D,(C)
    OPCodes_IN_C(DE.GetHighRegister());
}

void GZ80::OPCodeED0x51()
{
    // OUT (C),D
    OPCodes_OUT_C(DE.GetHighRegister());
}

void GZ80::OPCodeED0x52()
{
    // SBC HL,DE
    OPCodes_SBC_HL(DE.GetValue());
}

void GZ80::OPCodeED0x53()
{
    // LD (nn),DE
    OPCodes_LD_nn_dd(&DE);
}

void GZ80::OPCodeED0x54()
{
    // NEG*
    UndocumentedOPCode();
    OPCodeED0x44();
}

void GZ80::OPCodeED0x55()
{
    // RETN*
    UndocumentedOPCode();
    OPCodeED0x45();
}

void GZ80::OPCodeED0x56()
{
    // IM 1
    SetInterruptMode(1);
}

void GZ80::OPCodeED0x57()
{
    // LD A,I
    u8 value = I.GetValue();
    OPCodes_LD(AF.GetHighRegister(), value);
    ToggleSignFlagFromResult(value);
    ToggleZeroFlagFromResult(value);
    ToggleXYFlagsFromResult(value);
    ClearFlag(FLAG_HALF);
    ClearFlag(FLAG_NEGATIVE);
    if (iff2_)
        ToggleFlag(FLAG_PARITY);
    else
        ClearFlag(FLAG_PARITY);
}

void GZ80::OPCodeED0x58()
{
    // IN E,(C)
    OPCodes_IN_C(DE.GetLowRegister());
}

void GZ80::OPCodeED0x59()
{
    // OUT (C),E
    OPCodes_OUT_C(DE.GetLowRegister());
}

void GZ80::OPCodeED0x5A()
{
    // ADC HL,DE
    OPCodes_ADC_HL(DE.GetValue());
}

void GZ80::OPCodeED0x5B()
{
    // LD DE,(nn)
    OPCodes_LD_dd_nn(&DE);
}

void GZ80::OPCodeED0x5C()
{
    // NEG*
    UndocumentedOPCode();
    OPCodeED0x44();
}

void GZ80::OPCodeED0x5D()
{
    // RETN*
    UndocumentedOPCode();
    OPCodeED0x45();
}

void GZ80::OPCodeED0x5E()
{
    // IM 2
    SetInterruptMode(2);
}

void GZ80::OPCodeED0x5F()
{
    // LD A,R
    u8 value = R.GetValue();
    OPCodes_LD(AF.GetHighRegister(), value);
    ToggleSignFlagFromResult(value);
    ToggleZeroFlagFromResult(value);
    ToggleXYFlagsFromResult(value);
    ClearFlag(FLAG_HALF);
    ClearFlag(FLAG_NEGATIVE);
    if (iff2_)
        ToggleFlag(FLAG_PARITY);
    else
        ClearFlag(FLAG_PARITY);
}

void GZ80::OPCodeED0x60()
{
    // IN H,(C)
    OPCodes_IN_C(HL.GetHighRegister());
}

void GZ80::OPCodeED0x61()
{
    // OUT (C),H
    OPCodes_OUT_C(HL.GetHighRegister());
}

void GZ80::OPCodeED0x62()
{
    // SBC HL,HL
    OPCodes_SBC_HL(HL.GetValue());
}

void GZ80::OPCodeED0x63()
{
    // LD (nn),HL
    OPCodes_LD_nn_dd(&HL);
}

void GZ80::OPCodeED0x64()
{
    // NEG*
    UndocumentedOPCode();
    OPCodeED0x44();
}

void GZ80::OPCodeED0x65()
{
    // RETN*
    UndocumentedOPCode();
    OPCodeED0x45();
}

void GZ80::OPCodeED0x66()
{
    // IM 0*
    UndocumentedOPCode();
    OPCodeED0x46();
}

void GZ80::OPCodeED0x67()
{
    // RRD
    u16 address = HL.GetValue();
    u8 value = memory_impl_->Read(address);
    u8 result = (AF.GetHigh() & 0xF0) | (value & 0x0F);
    memory_impl_->Write(address, ((AF.GetHigh() << 4) & 0xF0) | ((value >> 4) & 0x0F));
    AF.SetHigh(result);
    IsSetFlag(FLAG_CARRY) ? SetFlag(FLAG_CARRY) : ClearAllFlags();
    ToggleZeroFlagFromResult(result);
    ToggleSignFlagFromResult(result);
    ToggleParityFlagFromResult(result);
    ToggleXYFlagsFromResult(result);
    XY.SetValue(address + 1);
}

void GZ80::OPCodeED0x68()
{
    // IN L,(C)
    OPCodes_IN_C(HL.GetLowRegister());
}

void GZ80::OPCodeED0x69()
{
    // OUT (C),L
    OPCodes_OUT_C(HL.GetLowRegister());
}

void GZ80::OPCodeED0x6A()
{
    // ADC HL,HL
    OPCodes_ADC_HL(HL.GetValue());
}

void GZ80::OPCodeED0x6B()
{
    // LD HL,(nn)
    OPCodes_LD_dd_nn(&HL);
}

void GZ80::OPCodeED0x6C()
{
    // NEG*
    UndocumentedOPCode();
    OPCodeED0x44();
}

void GZ80::OPCodeED0x6D()
{
    // RETN*
    UndocumentedOPCode();
    OPCodeED0x45();
}

void GZ80::OPCodeED0x6E()
{
    // IM 0*
    UndocumentedOPCode();
    OPCodeED0x46();
}

void GZ80::OPCodeED0x6F()
{
    // RLD
    u16 address = HL.GetValue();
    u8 value = memory_impl_->Read(address);
    u8 result = (AF.GetHigh() & 0xF0) | ((value >> 4) & 0x0F);
    memory_impl_->Write(address, ((value << 4) & 0xF0) | (AF.GetHigh() & 0x0F));
    AF.SetHigh(result);
    IsSetFlag(FLAG_CARRY) ? SetFlag(FLAG_CARRY) : ClearAllFlags();
    ToggleZeroFlagFromResult(result);
    ToggleSignFlagFromResult(result);
    ToggleParityFlagFromResult(result);
    ToggleXYFlagsFromResult(result);
    XY.SetValue(address + 1);
}

void GZ80::OPCodeED0x70()
{
    // IN F,(C)*
    UndocumentedOPCode();
    OPCodes_IN_C(NULL);
}

void GZ80::OPCodeED0x71()
{
    // OUT (C),0*
    UndocumentedOPCode();
    io_ports_impl_->Output(BC.GetLow(), 0);
}

void GZ80::OPCodeED0x72()
{
    // SBC HL,SP
    OPCodes_SBC_HL(SP.GetValue());
}

void GZ80::OPCodeED0x73()
{
    // LD (nn),SP
    OPCodes_LD_nn_dd(&SP);
}

void GZ80::OPCodeED0x74()
{
    // NEG*
    UndocumentedOPCode();
    OPCodeED0x44();
}

void GZ80::OPCodeED0x75()
{
    // RETN*
    UndocumentedOPCode();
    OPCodeED0x45();
}

void GZ80::OPCodeED0x76()
{
    // IM 1*
    UndocumentedOPCode();
    OPCodeED0x56();
}

void GZ80::OPCodeED0x78()
{
    // IN A,(C)
    OPCodes_IN_C(AF.GetHighRegister());
    XY.SetValue(BC.GetValue() + 1);
}

void GZ80::OPCodeED0x79()
{
    // OUT (C),A
    OPCodes_OUT_C(AF.GetHighRegister());
    XY.SetValue(BC.GetValue() + 1);
}

void GZ80::OPCodeED0x7A()
{
    // ADC HL,SP
    OPCodes_ADC_HL(SP.GetValue());
}

void GZ80::OPCodeED0x7B()
{
    // LD SP,(nn)
    OPCodes_LD_dd_nn(&SP);
}

void GZ80::OPCodeED0x7C()
{
    // NEG*
    UndocumentedOPCode();
    OPCodeED0x44();
}

void GZ80::OPCodeED0x7D()
{
    // RETN*
    UndocumentedOPCode();
    OPCodeED0x45();
}

void GZ80::OPCodeED0x7E()
{
    // IM 2*
    UndocumentedOPCode();
    OPCodeED0x5E();
}

void GZ80::OPCodeED0xA0()
{
    // LDI
    OPCodes_LDI();
}

void GZ80::OPCodeED0xA1()
{
    // CPI
    OPCodes_CPI();
}

void GZ80::OPCodeED0xA2()
{
    // INI
    OPCodes_INI();
}

void GZ80::OPCodeED0xA3()
{
    // OUTI
    OPCodes_OUTI();
}

void GZ80::OPCodeED0xA8()
{
    // LDD
    OPCodes_LDD();
}

void GZ80::OPCodeED0xA9()
{
    // CPD
    OPCodes_CPD();
}

void GZ80::OPCodeED0xAA()
{
    // IND
    OPCodes_IND();
}

void GZ80::OPCodeED0xAB()
{
    // OUTD
    OPCodes_OUTD();
}

void GZ80::OPCodeED0xB0()
{
    // LDIR
    OPCodes_LDI();
    if (BC.GetValue() != 0)
    {
        PC.Decrement();
        PC.Decrement();
        XY.SetValue(PC.GetValue() + 1);
        t_states_ += 5;
    }
}

void GZ80::OPCodeED0xB1()
{
    // CPIR
    OPCodes_CPI();
    if ((BC.GetValue() != 0) && !IsSetFlag(FLAG_ZERO))
    {
        PC.Decrement();
        PC.Decrement();
        XY.SetValue(PC.GetValue() + 1);
        t_states_ += 5;
    }
}

void GZ80::OPCodeED0xB2()
{
    // INIR
    OPCodes_INI();
    if (BC.GetHigh() != 0)
    {
        PC.Decrement();
        PC.Decrement();
        t_states_ += 5;
    }
}

void GZ80::OPCodeED0xB3()
{
    // OTIR
    OPCodes_OUTI();
    if (BC.GetHigh() != 0)
    {
        PC.Decrement();
        PC.Decrement();
        t_states_ += 5;
    }
}

void GZ80::OPCodeED0xB8()
{
    // LDDR
    OPCodes_LDD();
    if (BC.GetValue() != 0)
    {
        PC.Decrement();
        PC.Decrement();
        XY.SetValue(PC.GetValue() + 1);
        t_states_ += 5;
    }
}

void GZ80::OPCodeED0xB9()
{
    // CPDR
    OPCodes_CPD();
    if ((BC.GetValue() != 0) && !IsSetFlag(FLAG_ZERO))
    {
        PC.Decrement();
        PC.Decrement();
        XY.SetValue(PC.GetValue() + 1);
        t_states_ += 5;
    }
}

void GZ80::OPCodeED0xBA()
{
    // INDR
    OPCodes_IND();
    if (BC.GetHigh() != 0)
    {
        PC.Decrement();
        PC.Decrement();
        t_states_ += 5;
    }
}

void GZ80::OPCodeED0xBB()
{
    // OTDR
    OPCodes_OUTD();
    if (BC.GetHigh() != 0)
    {
        PC.Decrement();
        PC.Decrement();
        t_states_ += 5;
    }
}

} // namespace gz80
