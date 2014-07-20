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

#include "gz80_definitions.h"
#include "gz80_eight_bit_register.h"
#include "gz80_sixteen_bit_register.h"
#include "gz80_core.h"
#include "Memory.h"
#include "gz80_opcode_timing.h"
#include "gz80_opcode_daa.h"

namespace gz80
{
   
void GZ80::OPCode0x00()
{
    // NOP
}

void GZ80::OPCode0x01()
{
    // LD BC,nn
    OPCodes_LD(BC.GetLowRegister(), PC.GetValue());
    PC.Increment();
    OPCodes_LD(BC.GetHighRegister(), PC.GetValue());
    PC.Increment();
}

void GZ80::OPCode0x02()
{
    // LD (BC),A
    OPCodes_LD(BC.GetValue(), AF.GetHigh());
    XY.SetLow((BC.GetValue() + 1) & 0xFF);
    XY.SetHigh(AF.GetHigh());
}

void GZ80::OPCode0x03()
{
    // INC BC
    BC.Increment();
}

void GZ80::OPCode0x04()
{
    // INC B
    OPCodes_INC(BC.GetHighRegister());
}

void GZ80::OPCode0x05()
{
    // DEC B
    OPCodes_DEC(BC.GetHighRegister());
}

void GZ80::OPCode0x06()
{
    // LD B,n
    OPCodes_LD(BC.GetHighRegister(), PC.GetValue());
    PC.Increment();
}

void GZ80::OPCode0x07()
{
    // RLCA
    OPCodes_RLC(AF.GetHighRegister(), true);
}

void GZ80::OPCode0x08()
{
    // EX AF,AF’
    OPCodes_EX(&AF, &AF2);
}

void GZ80::OPCode0x09()
{
    // ADD HL,BC
    OPCodes_ADD_HL(BC.GetValue());
}

void GZ80::OPCode0x0A()
{
    // LD A,(BC)
    OPCodes_LD(AF.GetHighRegister(), BC.GetValue());
    XY.SetValue(BC.GetValue() + 1);
}

void GZ80::OPCode0x0B()
{
    // DEC BC
    BC.Decrement();
}

void GZ80::OPCode0x0C()
{
    // INC C
    OPCodes_INC(BC.GetLowRegister());
}

void GZ80::OPCode0x0D()
{
    // DEC C
    OPCodes_DEC(BC.GetLowRegister());
}

void GZ80::OPCode0x0E()
{
    // LD C,n
    OPCodes_LD(BC.GetLowRegister(), PC.GetValue());
    PC.Increment();
}

void GZ80::OPCode0x0F()
{
    // RRCA
    OPCodes_RRC(AF.GetHighRegister(), true);
}

void GZ80::OPCode0x10()
{
    // DJNZ (PC+e)
    BC.GetHighRegister()->Decrement();
    OPCodes_JR_n_conditional(BC.GetHigh() != 0);
}

void GZ80::OPCode0x11()
{
    // LD DE,nn
    OPCodes_LD(DE.GetLowRegister(), PC.GetValue());
    PC.Increment();
    OPCodes_LD(DE.GetHighRegister(), PC.GetValue());
    PC.Increment();
}

void GZ80::OPCode0x12()
{
    // LD (DE),A
    OPCodes_LD(DE.GetValue(), AF.GetHigh());
    XY.SetLow((DE.GetValue() + 1) & 0xFF);
    XY.SetHigh(AF.GetHigh());
}

void GZ80::OPCode0x13()
{
    // INC DE
    DE.Increment();
}

void GZ80::OPCode0x14()
{
    // INC D
    OPCodes_INC(DE.GetHighRegister());
}

void GZ80::OPCode0x15()
{
    // DEC D
    OPCodes_DEC(DE.GetHighRegister());
}

void GZ80::OPCode0x16()
{
    // LD D,n
    OPCodes_LD(DE.GetHighRegister(), PC.GetValue());
    PC.Increment();
}

void GZ80::OPCode0x17()
{
    // RLA
    OPCodes_RL(AF.GetHighRegister(), true);
}

void GZ80::OPCode0x18()
{
    // JR n
    OPCodes_JR_n();
}

void GZ80::OPCode0x19()
{
    // ADD HL,DE
    OPCodes_ADD_HL(DE.GetValue());
}

void GZ80::OPCode0x1A()
{
    // LD A,(DE)
    OPCodes_LD(AF.GetHighRegister(), DE.GetValue());
    XY.SetValue(DE.GetValue() + 1);
}

void GZ80::OPCode0x1B()
{
    // DEC DE
    DE.Decrement();
}

void GZ80::OPCode0x1C()
{
    // INC E
    OPCodes_INC(DE.GetLowRegister());
}

void GZ80::OPCode0x1D()
{
    // DEC E
    OPCodes_DEC(DE.GetLowRegister());
}

void GZ80::OPCode0x1E()
{
    // LD E,n
    OPCodes_LD(DE.GetLowRegister(), PC.GetValue());
    PC.Increment();
}

void GZ80::OPCode0x1F()
{
    // RRA
    OPCodes_RR(AF.GetHighRegister(), true);
}

void GZ80::OPCode0x20()
{
    // JR NZ,n
    OPCodes_JR_n_conditional(!IsSetFlag(FLAG_ZERO));
}

void GZ80::OPCode0x21()
{
    // LD HL,nn
    SixteenBitRegister* reg = GetPrefixedRegister();
    OPCodes_LD(reg->GetLowRegister(), PC.GetValue());
    PC.Increment();
    OPCodes_LD(reg->GetHighRegister(), PC.GetValue());
    PC.Increment();
}

void GZ80::OPCode0x22()
{
    // LD (nn),HL
    OPCodes_LD_nn_dd(GetPrefixedRegister());
}

void GZ80::OPCode0x23()
{
    // INC HL
    GetPrefixedRegister()->Increment();
}

void GZ80::OPCode0x24()
{
    // INC H
    OPCodes_INC(GetPrefixedRegister()->GetHighRegister());
}

void GZ80::OPCode0x25()
{
    // DEC H
    OPCodes_DEC(GetPrefixedRegister()->GetHighRegister());
}

void GZ80::OPCode0x26()
{
    // LD H,n
    OPCodes_LD(GetPrefixedRegister()->GetHighRegister(), PC.GetValue());
    PC.Increment();
}

void GZ80::OPCode0x27()
{
    // DAA
    int idx = AF.GetHigh();
    if (IsSetFlag(FLAG_CARRY))
        idx |= 0x100; 
    if (IsSetFlag(FLAG_HALF))
        idx |= 0x200;
    if (IsSetFlag(FLAG_NEGATIVE))
        idx |= 0x400;
    AF.SetValue(kOPCodeDAATable[idx]);
}

void GZ80::OPCode0x28()
{
    // JR Z,n
    OPCodes_JR_n_conditional(IsSetFlag(FLAG_ZERO));
}

void GZ80::OPCode0x29()
{
    // ADD HL,HL
    SixteenBitRegister* reg = GetPrefixedRegister();
    OPCodes_ADD_HL(reg->GetValue());
}

void GZ80::OPCode0x2A()
{
    // LD HL,(nn)
    OPCodes_LD_dd_nn(GetPrefixedRegister());
}

void GZ80::OPCode0x2B()
{
    // DEC HL
    GetPrefixedRegister()->Decrement();
}

void GZ80::OPCode0x2C()
{
    // INC L
    OPCodes_INC(GetPrefixedRegister()->GetLowRegister());
}

void GZ80::OPCode0x2D()
{
    // DEC L
    OPCodes_DEC(GetPrefixedRegister()->GetLowRegister());
}

void GZ80::OPCode0x2E()
{
    // LD L,n
    OPCodes_LD(GetPrefixedRegister()->GetLowRegister(), PC.GetValue());
    PC.Increment();

}

void GZ80::OPCode0x2F()
{
    // CPL
    AF.SetHigh(~AF.GetHigh());
    ToggleFlag(FLAG_HALF);
    ToggleFlag(FLAG_NEGATIVE);
    ToggleXYFlagsFromResult(AF.GetHigh());
}

void GZ80::OPCode0x30()
{
    // JR NC,n
    OPCodes_JR_n_conditional(!IsSetFlag(FLAG_CARRY));
}

void GZ80::OPCode0x31()
{
    // LD SP,nn
    SP.SetLow(memory_impl_->Read(PC.GetValue()));
    PC.Increment();
    SP.SetHigh(memory_impl_->Read(PC.GetValue()));
    PC.Increment();
}

void GZ80::OPCode0x32()
{
    // LD (nn),A
    u16 address = FetchArg16();
    memory_impl_->Write(address, AF.GetHigh());
    XY.SetLow((address + 1) & 0xFF);
    XY.SetHigh(AF.GetHigh());
}

void GZ80::OPCode0x33()
{
    // INC SP
    SP.Increment();
}

void GZ80::OPCode0x34()
{
    // INC (HL)
    OPCodes_INC_HL();
}

void GZ80::OPCode0x35()
{
    // DEC (HL)
    OPCodes_DEC_HL();
}

void GZ80::OPCode0x36()
{
    // LD (HL),n  
    if (actual_prefix_ == 0xDD)
    {
        u8 d = memory_impl_->Read(PC.GetValue());
        u8 n = memory_impl_->Read(PC.GetValue() + 1);
        u16 address = IX.GetValue() + static_cast<s8> (d);
        memory_impl_->Write(address, n);
        PC.Increment();
    }
    else if (actual_prefix_ == 0xFD)
    {
        u8 d = memory_impl_->Read(PC.GetValue());
        u8 n = memory_impl_->Read(PC.GetValue() + 1);
        u16 address = IY.GetValue() + static_cast<s8> (d);
        memory_impl_->Write(address, n);
        PC.Increment();
    }
    else
        memory_impl_->Write(HL.GetValue(), memory_impl_->Read(PC.GetValue()));
    PC.Increment();
}

void GZ80::OPCode0x37()
{
    // SCF
    ToggleFlag(FLAG_CARRY);
    ClearFlag(FLAG_HALF);
    ClearFlag(FLAG_NEGATIVE);
    ToggleXYFlagsFromResult(AF.GetHigh());
}

void GZ80::OPCode0x38()
{
    // JR C,n
    OPCodes_JR_n_conditional(IsSetFlag(FLAG_CARRY));
}

void GZ80::OPCode0x39()
{
    // ADD HL,SP
    OPCodes_ADD_HL(SP.GetValue());
}

void GZ80::OPCode0x3A()
{
    // LD A,(nn)
    u16 address = FetchArg16();
    AF.GetHighRegister()->SetValue(memory_impl_->Read(address));
    XY.SetValue(address + 1);
}

void GZ80::OPCode0x3B()
{
    // DEC SP
    SP.Decrement();
}

void GZ80::OPCode0x3C()
{
    // INC A
    OPCodes_INC(AF.GetHighRegister());
}

void GZ80::OPCode0x3D()
{
    // DEC A
    OPCodes_DEC(AF.GetHighRegister());
}

void GZ80::OPCode0x3E()
{
    // LD A,n
    OPCodes_LD(AF.GetHighRegister(), PC.GetValue());
    PC.Increment();
}

void GZ80::OPCode0x3F()
{
    // CCF
    bool half = IsSetFlag(FLAG_CARRY);
    FlipFlag(FLAG_CARRY);
    if (half)
        ToggleFlag(FLAG_HALF);
    else
        ClearFlag(FLAG_HALF);
    ClearFlag(FLAG_NEGATIVE);
    ToggleXYFlagsFromResult(AF.GetHigh());
}

void GZ80::OPCode0x40()
{
    // LD B,B
    OPCodes_LD(BC.GetHighRegister(), BC.GetHigh());
}

void GZ80::OPCode0x41()
{
    // LD B,C
    OPCodes_LD(BC.GetHighRegister(), BC.GetLow());
}

void GZ80::OPCode0x42()
{
    // LD B,D
    OPCodes_LD(BC.GetHighRegister(), DE.GetHigh());
}

void GZ80::OPCode0x43()
{
    // LD B,E
    OPCodes_LD(BC.GetHighRegister(), DE.GetLow());
}

void GZ80::OPCode0x44()
{
    // LD B,H
    OPCodes_LD(BC.GetHighRegister(), GetPrefixedRegister()->GetHigh());
}

void GZ80::OPCode0x45()
{
    // LD B,L
    OPCodes_LD(BC.GetHighRegister(), GetPrefixedRegister()->GetLow());
}

void GZ80::OPCode0x46()
{
    // LD B,(HL)
    OPCodes_LD(BC.GetHighRegister(), GetEffectiveAddress());
}

void GZ80::OPCode0x47()
{
    // LD B,A
    OPCodes_LD(BC.GetHighRegister(), AF.GetHigh());
}

void GZ80::OPCode0x48()
{
    // LD C,B
    OPCodes_LD(BC.GetLowRegister(), BC.GetHigh());
}

void GZ80::OPCode0x49()
{
    // LD C,C
    OPCodes_LD(BC.GetLowRegister(), BC.GetLow());
}

void GZ80::OPCode0x4A()
{
    // LD C,D
    OPCodes_LD(BC.GetLowRegister(), DE.GetHigh());
}

void GZ80::OPCode0x4B()
{
    // LD C,E
    OPCodes_LD(BC.GetLowRegister(), DE.GetLow());
}

void GZ80::OPCode0x4C()
{
    // LD C,H
    OPCodes_LD(BC.GetLowRegister(), GetPrefixedRegister()->GetHigh());
}

void GZ80::OPCode0x4D()
{
    // LD C,L
    OPCodes_LD(BC.GetLowRegister(), GetPrefixedRegister()->GetLow());
}

void GZ80::OPCode0x4E()
{
    // LD C,(HL)
    OPCodes_LD(BC.GetLowRegister(), GetEffectiveAddress());
}

void GZ80::OPCode0x4F()
{
    // LD C,A
    OPCodes_LD(BC.GetLowRegister(), AF.GetHigh());
}

void GZ80::OPCode0x50()
{
    // LD D,B
    OPCodes_LD(DE.GetHighRegister(), BC.GetHigh());
}

void GZ80::OPCode0x51()
{
    // LD D,C
    OPCodes_LD(DE.GetHighRegister(), BC.GetLow());
}

void GZ80::OPCode0x52()
{
    // LD D,D
    OPCodes_LD(DE.GetHighRegister(), DE.GetHigh());
}

void GZ80::OPCode0x53()
{
    // LD D,E
    OPCodes_LD(DE.GetHighRegister(), DE.GetLow());
}

void GZ80::OPCode0x54()
{
    // LD D,H
    OPCodes_LD(DE.GetHighRegister(), GetPrefixedRegister()->GetHigh());
}

void GZ80::OPCode0x55()
{
    // LD D,L
    OPCodes_LD(DE.GetHighRegister(), GetPrefixedRegister()->GetLow());
}

void GZ80::OPCode0x56()
{
    // LD D,(HL)
    OPCodes_LD(DE.GetHighRegister(), GetEffectiveAddress());
}

void GZ80::OPCode0x57()
{
    // LD D,A
    OPCodes_LD(DE.GetHighRegister(), AF.GetHigh());
}

void GZ80::OPCode0x58()
{
    // LD E,B
    OPCodes_LD(DE.GetLowRegister(), BC.GetHigh());
}

void GZ80::OPCode0x59()
{
    // LD E,C
    OPCodes_LD(DE.GetLowRegister(), BC.GetLow());
}

void GZ80::OPCode0x5A()
{
    // LD E,D
    OPCodes_LD(DE.GetLowRegister(), DE.GetHigh());
}

void GZ80::OPCode0x5B()
{
    // LD E,E
    OPCodes_LD(DE.GetLowRegister(), DE.GetLow());
}

void GZ80::OPCode0x5C()
{
    // LD E,H
    OPCodes_LD(DE.GetLowRegister(), GetPrefixedRegister()->GetHigh());
}

void GZ80::OPCode0x5D()
{
    // LD E,L
    OPCodes_LD(DE.GetLowRegister(), GetPrefixedRegister()->GetLow());
}

void GZ80::OPCode0x5E()
{
    // LD E,(HL)
    OPCodes_LD(DE.GetLowRegister(), GetEffectiveAddress());
}

void GZ80::OPCode0x5F()
{
    // LD E,A
    OPCodes_LD(DE.GetLowRegister(), AF.GetHigh());
}

void GZ80::OPCode0x60()
{
    // LD H,B
    OPCodes_LD(GetPrefixedRegister()->GetHighRegister(), BC.GetHigh());
}

void GZ80::OPCode0x61()
{
    // LD H,C
    OPCodes_LD(GetPrefixedRegister()->GetHighRegister(), BC.GetLow());
}

void GZ80::OPCode0x62()
{
    // LD H,D
    OPCodes_LD(GetPrefixedRegister()->GetHighRegister(), DE.GetHigh());
}

void GZ80::OPCode0x63()
{
    // LD H,E
    OPCodes_LD(GetPrefixedRegister()->GetHighRegister(), DE.GetLow());
}

void GZ80::OPCode0x64()
{
    // LD H,H
    OPCodes_LD(GetPrefixedRegister()->GetHighRegister(), GetPrefixedRegister()->GetHigh());
}

void GZ80::OPCode0x65()
{
    // LD H,L
    OPCodes_LD(GetPrefixedRegister()->GetHighRegister(), GetPrefixedRegister()->GetLow());
}

void GZ80::OPCode0x66()
{
    // LD H,(HL)
    OPCodes_LD(HL.GetHighRegister(), GetEffectiveAddress());
}

void GZ80::OPCode0x67()
{
    // LD H,A
    OPCodes_LD(GetPrefixedRegister()->GetHighRegister(), AF.GetHigh());
}

void GZ80::OPCode0x68()
{
    // LD L,B
    OPCodes_LD(GetPrefixedRegister()->GetLowRegister(), BC.GetHigh());
}

void GZ80::OPCode0x69()
{
    // LD L,C
    OPCodes_LD(GetPrefixedRegister()->GetLowRegister(), BC.GetLow());
}

void GZ80::OPCode0x6A()
{
    // LD L,D
    OPCodes_LD(GetPrefixedRegister()->GetLowRegister(), DE.GetHigh());
}

void GZ80::OPCode0x6B()
{
    // LD L,E
    OPCodes_LD(GetPrefixedRegister()->GetLowRegister(), DE.GetLow());
}

void GZ80::OPCode0x6C()
{
    // LD L,H
    OPCodes_LD(GetPrefixedRegister()->GetLowRegister(), GetPrefixedRegister()->GetHigh());
}

void GZ80::OPCode0x6D()
{
    // LD L,L
    OPCodes_LD(GetPrefixedRegister()->GetLowRegister(), GetPrefixedRegister()->GetLow());
}

void GZ80::OPCode0x6E()
{
    // LD L,(HL)
    OPCodes_LD(HL.GetLowRegister(), GetEffectiveAddress());
}

void GZ80::OPCode0x6F()
{
    // LD L,A
    OPCodes_LD(GetPrefixedRegister()->GetLowRegister(), AF.GetHigh());
}

void GZ80::OPCode0x70()
{
    // LD (HL),B
    OPCodes_LD(GetEffectiveAddress(), BC.GetHigh());
}

void GZ80::OPCode0x71()
{
    // LD (HL),C
    OPCodes_LD(GetEffectiveAddress(), BC.GetLow());
}

void GZ80::OPCode0x72()
{
    // LD (HL),D
    OPCodes_LD(GetEffectiveAddress(), DE.GetHigh());
}

void GZ80::OPCode0x73()
{
    // LD (HL),E
    OPCodes_LD(GetEffectiveAddress(), DE.GetLow());
}

void GZ80::OPCode0x74()
{
    // LD (HL),H
    OPCodes_LD(GetEffectiveAddress(), HL.GetHigh());
}

void GZ80::OPCode0x75()
{
    // LD (HL),L
    OPCodes_LD(GetEffectiveAddress(), HL.GetLow());
}

void GZ80::OPCode0x76()
{
    // HALT
    halt_ = true;
    PC.Decrement();
}

void GZ80::OPCode0x77()
{
    // LD (HL),A
    OPCodes_LD(GetEffectiveAddress(), AF.GetHigh());
}

void GZ80::OPCode0x78()
{
    // LD A,B
    OPCodes_LD(AF.GetHighRegister(), BC.GetHigh());
}

void GZ80::OPCode0x79()
{
    // LD A,C
    OPCodes_LD(AF.GetHighRegister(), BC.GetLow());
}

void GZ80::OPCode0x7A()
{
    // LD A,D
    OPCodes_LD(AF.GetHighRegister(), DE.GetHigh());
}

void GZ80::OPCode0x7B()
{
    // LD A,E
    OPCodes_LD(AF.GetHighRegister(), DE.GetLow());

}

void GZ80::OPCode0x7C()
{
    // LD A,H
    OPCodes_LD(AF.GetHighRegister(), GetPrefixedRegister()->GetHigh());
}

void GZ80::OPCode0x7D()
{
    // LD A,L
    OPCodes_LD(AF.GetHighRegister(), GetPrefixedRegister()->GetLow());
}

void GZ80::OPCode0x7E()
{
    // LD A,(HL)
    OPCodes_LD(AF.GetHighRegister(), GetEffectiveAddress());
}

void GZ80::OPCode0x7F()
{
    // LD A,A
    OPCodes_LD(AF.GetHighRegister(), AF.GetHigh());
}

void GZ80::OPCode0x80()
{
    // ADD A,B
    OPCodes_ADD(BC.GetHigh());
}

void GZ80::OPCode0x81()
{
    // ADD A,C
    OPCodes_ADD(BC.GetLow());
}

void GZ80::OPCode0x82()
{
    // ADD A,D
    OPCodes_ADD(DE.GetHigh());
}

void GZ80::OPCode0x83()
{
    // ADD A,E
    OPCodes_ADD(DE.GetLow());
}

void GZ80::OPCode0x84()
{
    // ADD A,H
    OPCodes_ADD(GetPrefixedRegister()->GetHigh());
}

void GZ80::OPCode0x85()
{
    // ADD A,L
    OPCodes_ADD(GetPrefixedRegister()->GetLow());
}

void GZ80::OPCode0x86()
{
    // ADD A,(HL)
    OPCodes_ADD(memory_impl_->Read(GetEffectiveAddress()));
}

void GZ80::OPCode0x87()
{
    // ADD A,A
    OPCodes_ADD(AF.GetHigh());
}

void GZ80::OPCode0x88()
{
    // ADC A,B
    OPCodes_ADC(BC.GetHigh());
}

void GZ80::OPCode0x89()
{
    // ADC A,C
    OPCodes_ADC(BC.GetLow());
}

void GZ80::OPCode0x8A()
{
    // ADC A,D
    OPCodes_ADC(DE.GetHigh());
}

void GZ80::OPCode0x8B()
{
    // ADC A,E
    OPCodes_ADC(DE.GetLow());
}

void GZ80::OPCode0x8C()
{
    // ADC A,H
    OPCodes_ADC(GetPrefixedRegister()->GetHigh());
}

void GZ80::OPCode0x8D()
{
    // ADC A,L
    OPCodes_ADC(GetPrefixedRegister()->GetLow());
}

void GZ80::OPCode0x8E()
{
    // ADC A,(HL)
    OPCodes_ADC(memory_impl_->Read(GetEffectiveAddress()));
}

void GZ80::OPCode0x8F()
{
    // ADC A,A
    OPCodes_ADC(AF.GetHigh());
}

void GZ80::OPCode0x90()
{
    // SUB B
    OPCodes_SUB(BC.GetHigh());
}

void GZ80::OPCode0x91()
{
    // SUB C
    OPCodes_SUB(BC.GetLow());
}

void GZ80::OPCode0x92()
{
    // SUB D
    OPCodes_SUB(DE.GetHigh());
}

void GZ80::OPCode0x93()
{
    // SUB E
    OPCodes_SUB(DE.GetLow());
}

void GZ80::OPCode0x94()
{
    // SUB H
    OPCodes_SUB(GetPrefixedRegister()->GetHigh());
}

void GZ80::OPCode0x95()
{
    // SUB L
    OPCodes_SUB(GetPrefixedRegister()->GetLow());
}

void GZ80::OPCode0x96()
{
    // SUB (HL)
    OPCodes_SUB(memory_impl_->Read(GetEffectiveAddress()));
}

void GZ80::OPCode0x97()
{
    // SUB A
    OPCodes_SUB(AF.GetHigh());
}

void GZ80::OPCode0x98()
{
    // SBC B
    OPCodes_SBC(BC.GetHigh());
}

void GZ80::OPCode0x99()
{
    // SBC C
    OPCodes_SBC(BC.GetLow());
}

void GZ80::OPCode0x9A()
{
    // SBC D
    OPCodes_SBC(DE.GetHigh());
}

void GZ80::OPCode0x9B()
{
    // SBC E
    OPCodes_SBC(DE.GetLow());
}

void GZ80::OPCode0x9C()
{
    // SBC H
    OPCodes_SBC(GetPrefixedRegister()->GetHigh());
}

void GZ80::OPCode0x9D()
{
    // SBC L
    OPCodes_SBC(GetPrefixedRegister()->GetLow());
}

void GZ80::OPCode0x9E()
{
    // SBC (HL)
    OPCodes_SBC(memory_impl_->Read(GetEffectiveAddress()));
}

void GZ80::OPCode0x9F()
{
    // SBC A
    OPCodes_SBC(AF.GetHigh());
}

void GZ80::OPCode0xA0()
{
    // AND B
    OPCodes_AND(BC.GetHigh());
}

void GZ80::OPCode0xA1()
{
    // AND C
    OPCodes_AND(BC.GetLow());
}

void GZ80::OPCode0xA2()
{
    // AND D
    OPCodes_AND(DE.GetHigh());
}

void GZ80::OPCode0xA3()
{
    // AND E
    OPCodes_AND(DE.GetLow());
}

void GZ80::OPCode0xA4()
{
    // AND H
    OPCodes_AND(GetPrefixedRegister()->GetHigh());
}

void GZ80::OPCode0xA5()
{
    // AND L
    OPCodes_AND(GetPrefixedRegister()->GetLow());
}

void GZ80::OPCode0xA6()
{
    // AND (HL)
    OPCodes_AND(memory_impl_->Read(GetEffectiveAddress()));
}

void GZ80::OPCode0xA7()
{
    // AND A
    OPCodes_AND(AF.GetHigh());
}

void GZ80::OPCode0xA8()
{
    // XOR B
    OPCodes_XOR(BC.GetHigh());
}

void GZ80::OPCode0xA9()
{
    // XOR C
    OPCodes_XOR(BC.GetLow());
}

void GZ80::OPCode0xAA()
{
    // XOR D
    OPCodes_XOR(DE.GetHigh());
}

void GZ80::OPCode0xAB()
{
    // XOR E
    OPCodes_XOR(DE.GetLow());
}

void GZ80::OPCode0xAC()
{
    // XOR H
    OPCodes_XOR(GetPrefixedRegister()->GetHigh());
}

void GZ80::OPCode0xAD()
{
    // XOR L
    OPCodes_XOR(GetPrefixedRegister()->GetLow());
}

void GZ80::OPCode0xAE()
{
    // XOR (HL)
    OPCodes_XOR(memory_impl_->Read(GetEffectiveAddress()));
}

void GZ80::OPCode0xAF()
{
    // XOR A
    OPCodes_XOR(AF.GetHigh());
}

void GZ80::OPCode0xB0()
{
    // OR B
    OPCodes_OR(BC.GetHigh());
}

void GZ80::OPCode0xB1()
{
    // OR C
    OPCodes_OR(BC.GetLow());
}

void GZ80::OPCode0xB2()
{
    // OR D
    OPCodes_OR(DE.GetHigh());
}

void GZ80::OPCode0xB3()
{
    // OR E
    OPCodes_OR(DE.GetLow());

}

void GZ80::OPCode0xB4()
{
    // OR H
    OPCodes_OR(GetPrefixedRegister()->GetHigh());
}

void GZ80::OPCode0xB5()
{
    // OR L
    OPCodes_OR(GetPrefixedRegister()->GetLow());
}

void GZ80::OPCode0xB6()
{
    // OR (HL)
    OPCodes_OR(memory_impl_->Read(GetEffectiveAddress()));
}

void GZ80::OPCode0xB7()
{
    // OR A
    OPCodes_OR(AF.GetHigh());
}

void GZ80::OPCode0xB8()
{
    // CP B
    OPCodes_CP(BC.GetHigh());
}

void GZ80::OPCode0xB9()
{
    // CP C
    OPCodes_CP(BC.GetLow());
}

void GZ80::OPCode0xBA()
{
    // CP D
    OPCodes_CP(DE.GetHigh());
}

void GZ80::OPCode0xBB()
{
    // CP E
    OPCodes_CP(DE.GetLow());
}

void GZ80::OPCode0xBC()
{
    // CP H
    OPCodes_CP(GetPrefixedRegister()->GetHigh());
}

void GZ80::OPCode0xBD()
{
    // CP L
    OPCodes_CP(GetPrefixedRegister()->GetLow());
}

void GZ80::OPCode0xBE()
{
    // CP (HL)
    OPCodes_CP(memory_impl_->Read(GetEffectiveAddress()));
}

void GZ80::OPCode0xBF()
{
    // CP A
    OPCodes_CP(AF.GetHigh());
}

void GZ80::OPCode0xC0()
{
    // RET NZ
    OPCodes_RET_Conditional(!IsSetFlag(FLAG_ZERO));
}

void GZ80::OPCode0xC1()
{
    // POP BC
    StackPop(&BC);
}

void GZ80::OPCode0xC2()
{
    // JP NZ,nn
    OPCodes_JP_nn_Conditional(!IsSetFlag(FLAG_ZERO));
}

void GZ80::OPCode0xC3()
{
    // JP nn
    OPCodes_JP_nn();
}

void GZ80::OPCode0xC4()
{
    // CALL NZ,nn
    OPCodes_CALL_nn_Conditional(!IsSetFlag(FLAG_ZERO));
}

void GZ80::OPCode0xC5()
{
    // PUSH BC
    StackPush(&BC);
}

void GZ80::OPCode0xC6()
{
    // ADD A,n
    OPCodes_ADD(memory_impl_->Read(PC.GetValue()));
    PC.Increment();
}

void GZ80::OPCode0xC7()
{
    // RST 00H
    OPCodes_RST(0x0000);
}

void GZ80::OPCode0xC8()
{
    // RET Z
    OPCodes_RET_Conditional(IsSetFlag(FLAG_ZERO));
}

void GZ80::OPCode0xC9()
{
    // RET
    OPCodes_RET();
}

void GZ80::OPCode0xCA()
{
    // JP Z,nn
    OPCodes_JP_nn_Conditional(IsSetFlag(FLAG_ZERO));
}

void GZ80::OPCode0xCB()
{
    // CB prefixed instruction
    InvalidOPCode();
}

void GZ80::OPCode0xCC()
{
    // CALL Z,nn
    OPCodes_CALL_nn_Conditional(IsSetFlag(FLAG_ZERO));
}

void GZ80::OPCode0xCD()
{
    // CALL nn
    OPCodes_CALL_nn();
}

void GZ80::OPCode0xCE()
{
    // ADC A,n
    OPCodes_ADC(memory_impl_->Read(PC.GetValue()));
    PC.Increment();
}

void GZ80::OPCode0xCF()
{
    // RST 08H
    OPCodes_RST(0x0008);
}

void GZ80::OPCode0xD0()
{
    // RET NC
    OPCodes_RET_Conditional(!IsSetFlag(FLAG_CARRY));
}

void GZ80::OPCode0xD1()
{
    // POP DE
    StackPop(&DE);
}

void GZ80::OPCode0xD2()
{
    // JP NC,nn
    OPCodes_JP_nn_Conditional(!IsSetFlag(FLAG_CARRY));
}

void GZ80::OPCode0xD3()
{
    // OUT (n),A
    u8 port = memory_impl_->Read(PC.GetValue());
    PC.Increment();
    io_ports_impl_->Output(port, AF.GetHigh());
    XY.SetLow((port + 1) & 0xFF);
    XY.SetHigh(AF.GetHigh());
}

void GZ80::OPCode0xD4()
{
    // CALL NC,nn
    OPCodes_CALL_nn_Conditional(!IsSetFlag(FLAG_CARRY));
}

void GZ80::OPCode0xD5()
{
    // PUSH DE
    StackPush(&DE);
}

void GZ80::OPCode0xD6()
{
    // SUB n
    OPCodes_SUB(memory_impl_->Read(PC.GetValue()));
    PC.Increment();
}

void GZ80::OPCode0xD7()
{
    // RST 10H
    OPCodes_RST(0x0010);
}

void GZ80::OPCode0xD8()
{
    // RET C
    OPCodes_RET_Conditional(IsSetFlag(FLAG_CARRY));
}

void GZ80::OPCode0xD9()
{
    // EXX
    OPCodes_EX(&BC, &BC2);
    OPCodes_EX(&DE, &DE2);
    OPCodes_EX(&HL, &HL2);
}

void GZ80::OPCode0xDA()
{
    // JP C,nn
    OPCodes_JP_nn_Conditional(IsSetFlag(FLAG_CARRY));
}

void GZ80::OPCode0xDB()
{
    // IN A,(n)
    u8 a = AF.GetHigh();
    u8 port = memory_impl_->Read(PC.GetValue());
    PC.Increment();
    AF.SetHigh(io_ports_impl_->Input(port));
    XY.SetValue((a << 8) | (port + 1));
}

void GZ80::OPCode0xDC()
{
    // CALL C,nn
    OPCodes_CALL_nn_Conditional(IsSetFlag(FLAG_CARRY));
}

void GZ80::OPCode0xDD()
{
    // DD prefixed instruction
    InvalidOPCode();
}

void GZ80::OPCode0xDE()
{
    // SBC n
    OPCodes_SBC(memory_impl_->Read(PC.GetValue()));
    PC.Increment();
}

void GZ80::OPCode0xDF()
{
    // RST 18H
    OPCodes_RST(0x0018);
}

void GZ80::OPCode0xE0()
{
    // RET PO
    OPCodes_RET_Conditional(!IsSetFlag(FLAG_PARITY));
}

void GZ80::OPCode0xE1()
{
    // POP HL
    StackPop(GetPrefixedRegister());
}

void GZ80::OPCode0xE2()
{
    // JP PO,nn
    OPCodes_JP_nn_Conditional(!IsSetFlag(FLAG_PARITY));
}

void GZ80::OPCode0xE3()
{
    // EX (SP),HL
    SixteenBitRegister* reg = GetPrefixedRegister();
    u8 l = reg->GetLow();
    u8 h = reg->GetHigh();
    reg->SetLow(memory_impl_->Read(SP.GetValue()));
    reg->SetHigh(memory_impl_->Read(SP.GetValue() + 1));
    memory_impl_->Write(SP.GetValue(), l);
    memory_impl_->Write(SP.GetValue() + 1, h);
    XY.SetValue(reg->GetValue());
}

void GZ80::OPCode0xE4()
{
    // CALL PO,nn
    OPCodes_CALL_nn_Conditional(!IsSetFlag(FLAG_PARITY));
}

void GZ80::OPCode0xE5()
{
    // PUSH HL
    StackPush(GetPrefixedRegister());
}

void GZ80::OPCode0xE6()
{
    // AND n
    OPCodes_AND(memory_impl_->Read(PC.GetValue()));
    PC.Increment();
}

void GZ80::OPCode0xE7()
{
    // RST 20H
    OPCodes_RST(0x0020);
}

void GZ80::OPCode0xE8()
{
    // RET PE
    OPCodes_RET_Conditional(IsSetFlag(FLAG_PARITY));
}

void GZ80::OPCode0xE9()
{
    // JP (HL)
    PC.SetValue(GetPrefixedRegister()->GetValue());
}

void GZ80::OPCode0xEA()
{
    // JP PE,nn
    OPCodes_JP_nn_Conditional(IsSetFlag(FLAG_PARITY));
}

void GZ80::OPCode0xEB()
{
    // EX DE,HL
    OPCodes_EX(&DE, &HL);
}

void GZ80::OPCode0xEC()
{
    // CALL PE,nn
    OPCodes_CALL_nn_Conditional(IsSetFlag(FLAG_PARITY));
}

void GZ80::OPCode0xED()
{
    // ED prefixed instruction
    InvalidOPCode();
}

void GZ80::OPCode0xEE()
{
    // XOR n
    OPCodes_XOR(memory_impl_->Read(PC.GetValue()));
    PC.Increment();
}

void GZ80::OPCode0xEF()
{
    // RST 28H
    OPCodes_RST(0x28);
}

void GZ80::OPCode0xF0()
{
    // RET P
    OPCodes_RET_Conditional(!IsSetFlag(FLAG_SIGN));
}

void GZ80::OPCode0xF1()
{
    // POP AF
    StackPop(&AF);
}

void GZ80::OPCode0xF2()
{
    // JP P,nn
    OPCodes_JP_nn_Conditional(!IsSetFlag(FLAG_SIGN));
}

void GZ80::OPCode0xF3()
{
    // DI
    iff1_ = false;
    iff2_ = false;
}

void GZ80::OPCode0xF4()
{
    // CALL P,nn
    OPCodes_CALL_nn_Conditional(!IsSetFlag(FLAG_SIGN));
}

void GZ80::OPCode0xF5()
{
    // PUSH AF
    StackPush(&AF);
}

void GZ80::OPCode0xF6()
{
    // OR n
    OPCodes_OR(memory_impl_->Read(PC.GetValue()));
    PC.Increment();
}

void GZ80::OPCode0xF7()
{
    // RST 30H
    OPCodes_RST(0x0030);
}

void GZ80::OPCode0xF8()
{
    // RET M
    OPCodes_RET_Conditional(IsSetFlag(FLAG_SIGN));
}

void GZ80::OPCode0xF9()
{
    // LD SP,HL
    SP.SetValue(GetPrefixedRegister()->GetValue());
}

void GZ80::OPCode0xFA()
{
    // JP M,nn
    OPCodes_JP_nn_Conditional(IsSetFlag(FLAG_SIGN));
}

void GZ80::OPCode0xFB()
{
    // EI
    iff1_ = true;
    iff2_ = true;
    just_after_ei_ = true;
}

void GZ80::OPCode0xFC()
{
    // CALL M,nn
    OPCodes_CALL_nn_Conditional(IsSetFlag(FLAG_SIGN));
}

void GZ80::OPCode0xFD()
{
    // FD prefixed instruction
    InvalidOPCode();
}

void GZ80::OPCode0xFE()
{
    // CP n
    OPCodes_CP(memory_impl_->Read(PC.GetValue()));
    PC.Increment();
}

void GZ80::OPCode0xFF()
{
    // RST 38H
    OPCodes_RST(0x0038);
}

} // namespace gz80
