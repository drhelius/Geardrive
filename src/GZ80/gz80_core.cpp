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

#include "gz80_definitions.h"
#include "gz80_eight_bit_register.h"
#include "gz80_sixteen_bit_register.h"
#include "gz80_opcode_timing.h"
#include "gz80_opcode_names.h"

namespace gz80 {
    
GZ80::GZ80()
{
    InitPointer(io_ports_impl_);
    InitPointer(memory_impl_);
    InitOPCodeFunctors();
    iff1_ = false;
    iff2_ = false;
    halt_ = false;
    branched_ = false;
    t_states_ = 0;
    just_after_ei_ = false;
    interrupt_mode_ = 0;
    interrupt_requested_ = false;
    nmi_interrupt_requested_ = false;
    prefixed_cb_opcode_ = false;
    prefixed_cb_value_ = 0;
}

GZ80::~GZ80()
{
}

void GZ80::Init()
{
    Reset();
}

void GZ80::Reset()
{
    iff1_ = false;
    iff2_ = false;
    halt_ = false;
    branched_ = false;
    t_states_ = 0;
    just_after_ei_ = false;
    interrupt_mode_ = 1;
    PC_.SetValue(0x0000);
    SP_.SetValue(0xDFF0);
    IX_.SetValue(0xFFFF);
    IY_.SetValue(0xFFFF);
    AF_.SetValue(0x0040);  // Zero flag set
    BC_.SetValue(0x0000);
    DE_.SetValue(0x0000);
    HL_.SetValue(0x0000);
    AF2_.SetValue(0x0000);
    BC2_.SetValue(0x0000);
    DE2_.SetValue(0x0000);
    HL2_.SetValue(0x0000);
    XY_.SetValue(0x0000);
    I_.SetValue(0x00);
    R_.SetValue(0x00);
    interrupt_requested_ = false;
    nmi_interrupt_requested_ = false;
    prefixed_cb_opcode_ = false;
    prefixed_cb_value_ = 0;
}

unsigned int GZ80::RunFor(unsigned int t_states)
{
    unsigned int count = 0;
    
    while (count < t_states)
    {
        count += Tick();
    }
    
    return count - t_states;
}

unsigned int GZ80::Tick()
{
    t_states_ = 0;
    
    if (nmi_interrupt_requested_)
    {
        LeaveHalt();
        nmi_interrupt_requested_ = false;
        iff1_ = false;
        StackPush(&PC_);
        PC_.SetValue(0x0066);
        t_states_ += 11;
        IncreaseR();
        XY_.SetValue(PC_.GetValue());
        return t_states_;
    }
    else if (iff1_ && interrupt_requested_ && !just_after_ei_)
    {
        LeaveHalt();
        iff1_ = false;
        iff2_ = false;
        StackPush(&PC_);
        PC_.SetValue(0x0038);
        t_states_ += 13;
        IncreaseR();
        XY_.SetValue(PC_.GetValue());
        return t_states_;
    } 

    just_after_ei_ = false;

    ExecuteOPCode();

    return t_states_;
}

void GZ80::RequestINT(bool assert)
{
    interrupt_requested_ = assert;
}

void GZ80::RequestNMI()
{
    nmi_interrupt_requested_ = true;
}

void GZ80::SetIOPOrtsImpl(IOPortsInterface* io_ports_impl)
{
    io_ports_impl_ = io_ports_impl;
}

void GZ80::SetMemoryImpl(MemoryInterface* memory_impl)
{
    memory_impl_ = memory_impl;
}

void GZ80::ExecuteOPCode()
{
    u8 opcode = FetchOPCode();
    
    switch (opcode)
    {
        case 0xDD:
        case 0xFD:
        {
            int more_prefixes = false;
            while ((opcode == 0xDD) | (opcode == 0xFD))
            {
                actual_prefix_ = opcode;
                opcode = FetchOPCode();
                if (more_prefixes)
                    t_states_ += 4;
                more_prefixes = true;
                IncreaseR();
            }
            break;
        }
        default:
        {
            actual_prefix_ = 0x00;
            break;
        } 
    }
    
    switch (opcode)
    {
        case 0xCB:
        {
            IncreaseR();

            if (IsPrefixedInstruction())
            {
                prefixed_cb_opcode_ = true;
                prefixed_cb_value_ = memory_impl_->Read(PC_.GetValue());
                PC_.Increment();
            } 
            else
                IncreaseR();
                    
            opcode = FetchOPCode();

#ifdef GZ80_DISASM
            u16 opcode_address = PC_.GetValue() - 1;
            
            if (!memory_impl_->IsDisassembled(opcode_address))
            {
                if (actual_prefix_ == 0xDD)
                    memory_impl_->Disassemble(opcode_address, kOPCodeDDCBNames[opcode]);
                else if (actual_prefix_ == 0xFD)
                    memory_impl_->Disassemble(opcode_address, kOPCodeFDCBNames[opcode]);
                else
                    memory_impl_->Disassemble(opcode_address, kOPCodeCBNames[opcode]);
            }
#endif

            (this->*opcodes_cb_[opcode])();

            if (IsPrefixedInstruction())
            {
                t_states_ += kOPCodeXYCBTStates[opcode];
                prefixed_cb_opcode_ = false;
            }
            else
                t_states_ += kOPCodeCBTStates[opcode];
            
            break;
        }
        case 0xED:
        {
            IncreaseR();
            IncreaseR();

            actual_prefix_ = 0x00;
            opcode = FetchOPCode();
            
#ifdef GZ80_DISASM
            u16 opcode_address = PC_.GetValue() - 1;
            
            if (!memory_impl_->IsDisassembled(opcode_address))
            {
                memory_impl_->Disassemble(opcode_address, kOPCodeEDNames[opcode]);
            }
#endif

            (this->*opcodes_ed_[opcode])();

            t_states_ += kOPCodeEDTStates[opcode];
            break;
        }
        default:
        {
            IncreaseR();

#ifdef GZ80_DISASM
            u16 opcode_address = PC_.GetValue() - 1;
            
            if (!memory_impl_->IsDisassembled(opcode_address))
            {
                if (actual_prefix_ == 0xDD)
                    memory_impl_->Disassemble(opcode_address, kOPCodeDDNames[opcode]);
                else if (actual_prefix_ == 0xFD)
                    memory_impl_->Disassemble(opcode_address, kOPCodeFDNames[opcode]);
                else
                    memory_impl_->Disassemble(opcode_address, kOPCodeNames[opcode]);
            }
#endif

            (this->*opcodes_[opcode])();

            if (IsPrefixedInstruction())
                t_states_ += kOPCodeXYTStates[opcode];
            else
                t_states_ += kOPCodeTStates[opcode];

            if (branched_)
            {
                branched_ = false;
                t_states_ += kOPCodeTStatesBranched[opcode];
            }
            break;
        }
    }
}

void GZ80::InvalidOPCode()
{
#ifdef DEBUG_GEARSYSTEM
    u16 opcode_address = PC_.GetValue() - 1;
    u16 prefix_address = PC_.GetValue() - 2;
    u8 opcode = memory_impl_->Read(opcode_address);
    u8 prefix = memory_impl_->Read(prefix_address);

    switch (prefix)
    {
        case 0xCB:
        {
            Log("--> ** INVALID CB OP Code (%X) at $%.4X -- %s", opcode, opcode_address, kOPCodeCBNames[opcode]);
            break;
        }
        case 0xED:
        {
            Log("--> ** INVALID ED OP Code (%X) at $%.4X -- %s", opcode, opcode_address, kOPCodeEDNames[opcode]);
            break;
        }
        default:
        {
            Log("--> ** INVALID OP Code (%X) at $%.4X -- %s", opcode, opcode_address, kOPCodeNames[opcode]);
        }
    }
#endif
}

void GZ80::UndocumentedOPCode()
{
#ifdef DEBUG_GEARSYSTEM
    u16 opcode_address = PC_.GetValue() - 1;
    u8 opcode = memory_impl_->Read(opcode_address);

    Log("--> ** UNDOCUMENTED OP Code (%X) at $%.4X -- %s", opcode, opcode_address, kOPCodeNames[opcode]);
#endif
}

void GZ80::InitOPCodeFunctors()
{
    opcodes_[0x00] = &GZ80::OPCode0x00;
    opcodes_[0x01] = &GZ80::OPCode0x01;
    opcodes_[0x02] = &GZ80::OPCode0x02;
    opcodes_[0x03] = &GZ80::OPCode0x03;
    opcodes_[0x04] = &GZ80::OPCode0x04;
    opcodes_[0x05] = &GZ80::OPCode0x05;
    opcodes_[0x06] = &GZ80::OPCode0x06;
    opcodes_[0x07] = &GZ80::OPCode0x07;
    opcodes_[0x08] = &GZ80::OPCode0x08;
    opcodes_[0x09] = &GZ80::OPCode0x09;
    opcodes_[0x0A] = &GZ80::OPCode0x0A;
    opcodes_[0x0B] = &GZ80::OPCode0x0B;
    opcodes_[0x0C] = &GZ80::OPCode0x0C;
    opcodes_[0x0D] = &GZ80::OPCode0x0D;
    opcodes_[0x0E] = &GZ80::OPCode0x0E;
    opcodes_[0x0F] = &GZ80::OPCode0x0F;

    opcodes_[0x10] = &GZ80::OPCode0x10;
    opcodes_[0x11] = &GZ80::OPCode0x11;
    opcodes_[0x12] = &GZ80::OPCode0x12;
    opcodes_[0x13] = &GZ80::OPCode0x13;
    opcodes_[0x14] = &GZ80::OPCode0x14;
    opcodes_[0x15] = &GZ80::OPCode0x15;
    opcodes_[0x16] = &GZ80::OPCode0x16;
    opcodes_[0x17] = &GZ80::OPCode0x17;
    opcodes_[0x18] = &GZ80::OPCode0x18;
    opcodes_[0x19] = &GZ80::OPCode0x19;
    opcodes_[0x1A] = &GZ80::OPCode0x1A;
    opcodes_[0x1B] = &GZ80::OPCode0x1B;
    opcodes_[0x1C] = &GZ80::OPCode0x1C;
    opcodes_[0x1D] = &GZ80::OPCode0x1D;
    opcodes_[0x1E] = &GZ80::OPCode0x1E;
    opcodes_[0x1F] = &GZ80::OPCode0x1F;

    opcodes_[0x20] = &GZ80::OPCode0x20;
    opcodes_[0x21] = &GZ80::OPCode0x21;
    opcodes_[0x22] = &GZ80::OPCode0x22;
    opcodes_[0x23] = &GZ80::OPCode0x23;
    opcodes_[0x24] = &GZ80::OPCode0x24;
    opcodes_[0x25] = &GZ80::OPCode0x25;
    opcodes_[0x26] = &GZ80::OPCode0x26;
    opcodes_[0x27] = &GZ80::OPCode0x27;
    opcodes_[0x28] = &GZ80::OPCode0x28;
    opcodes_[0x29] = &GZ80::OPCode0x29;
    opcodes_[0x2A] = &GZ80::OPCode0x2A;
    opcodes_[0x2B] = &GZ80::OPCode0x2B;
    opcodes_[0x2C] = &GZ80::OPCode0x2C;
    opcodes_[0x2D] = &GZ80::OPCode0x2D;
    opcodes_[0x2E] = &GZ80::OPCode0x2E;
    opcodes_[0x2F] = &GZ80::OPCode0x2F;

    opcodes_[0x30] = &GZ80::OPCode0x30;
    opcodes_[0x31] = &GZ80::OPCode0x31;
    opcodes_[0x32] = &GZ80::OPCode0x32;
    opcodes_[0x33] = &GZ80::OPCode0x33;
    opcodes_[0x34] = &GZ80::OPCode0x34;
    opcodes_[0x35] = &GZ80::OPCode0x35;
    opcodes_[0x36] = &GZ80::OPCode0x36;
    opcodes_[0x37] = &GZ80::OPCode0x37;
    opcodes_[0x38] = &GZ80::OPCode0x38;
    opcodes_[0x39] = &GZ80::OPCode0x39;
    opcodes_[0x3A] = &GZ80::OPCode0x3A;
    opcodes_[0x3B] = &GZ80::OPCode0x3B;
    opcodes_[0x3C] = &GZ80::OPCode0x3C;
    opcodes_[0x3D] = &GZ80::OPCode0x3D;
    opcodes_[0x3E] = &GZ80::OPCode0x3E;
    opcodes_[0x3F] = &GZ80::OPCode0x3F;

    opcodes_[0x40] = &GZ80::OPCode0x40;
    opcodes_[0x41] = &GZ80::OPCode0x41;
    opcodes_[0x42] = &GZ80::OPCode0x42;
    opcodes_[0x43] = &GZ80::OPCode0x43;
    opcodes_[0x44] = &GZ80::OPCode0x44;
    opcodes_[0x45] = &GZ80::OPCode0x45;
    opcodes_[0x46] = &GZ80::OPCode0x46;
    opcodes_[0x47] = &GZ80::OPCode0x47;
    opcodes_[0x48] = &GZ80::OPCode0x48;
    opcodes_[0x49] = &GZ80::OPCode0x49;
    opcodes_[0x4A] = &GZ80::OPCode0x4A;
    opcodes_[0x4B] = &GZ80::OPCode0x4B;
    opcodes_[0x4C] = &GZ80::OPCode0x4C;
    opcodes_[0x4D] = &GZ80::OPCode0x4D;
    opcodes_[0x4E] = &GZ80::OPCode0x4E;
    opcodes_[0x4F] = &GZ80::OPCode0x4F;

    opcodes_[0x50] = &GZ80::OPCode0x50;
    opcodes_[0x51] = &GZ80::OPCode0x51;
    opcodes_[0x52] = &GZ80::OPCode0x52;
    opcodes_[0x53] = &GZ80::OPCode0x53;
    opcodes_[0x54] = &GZ80::OPCode0x54;
    opcodes_[0x55] = &GZ80::OPCode0x55;
    opcodes_[0x56] = &GZ80::OPCode0x56;
    opcodes_[0x57] = &GZ80::OPCode0x57;
    opcodes_[0x58] = &GZ80::OPCode0x58;
    opcodes_[0x59] = &GZ80::OPCode0x59;
    opcodes_[0x5A] = &GZ80::OPCode0x5A;
    opcodes_[0x5B] = &GZ80::OPCode0x5B;
    opcodes_[0x5C] = &GZ80::OPCode0x5C;
    opcodes_[0x5D] = &GZ80::OPCode0x5D;
    opcodes_[0x5E] = &GZ80::OPCode0x5E;
    opcodes_[0x5F] = &GZ80::OPCode0x5F;

    opcodes_[0x60] = &GZ80::OPCode0x60;
    opcodes_[0x61] = &GZ80::OPCode0x61;
    opcodes_[0x62] = &GZ80::OPCode0x62;
    opcodes_[0x63] = &GZ80::OPCode0x63;
    opcodes_[0x64] = &GZ80::OPCode0x64;
    opcodes_[0x65] = &GZ80::OPCode0x65;
    opcodes_[0x66] = &GZ80::OPCode0x66;
    opcodes_[0x67] = &GZ80::OPCode0x67;
    opcodes_[0x68] = &GZ80::OPCode0x68;
    opcodes_[0x69] = &GZ80::OPCode0x69;
    opcodes_[0x6A] = &GZ80::OPCode0x6A;
    opcodes_[0x6B] = &GZ80::OPCode0x6B;
    opcodes_[0x6C] = &GZ80::OPCode0x6C;
    opcodes_[0x6D] = &GZ80::OPCode0x6D;
    opcodes_[0x6E] = &GZ80::OPCode0x6E;
    opcodes_[0x6F] = &GZ80::OPCode0x6F;

    opcodes_[0x70] = &GZ80::OPCode0x70;
    opcodes_[0x71] = &GZ80::OPCode0x71;
    opcodes_[0x72] = &GZ80::OPCode0x72;
    opcodes_[0x73] = &GZ80::OPCode0x73;
    opcodes_[0x74] = &GZ80::OPCode0x74;
    opcodes_[0x75] = &GZ80::OPCode0x75;
    opcodes_[0x76] = &GZ80::OPCode0x76;
    opcodes_[0x77] = &GZ80::OPCode0x77;
    opcodes_[0x78] = &GZ80::OPCode0x78;
    opcodes_[0x79] = &GZ80::OPCode0x79;
    opcodes_[0x7A] = &GZ80::OPCode0x7A;
    opcodes_[0x7B] = &GZ80::OPCode0x7B;
    opcodes_[0x7C] = &GZ80::OPCode0x7C;
    opcodes_[0x7D] = &GZ80::OPCode0x7D;
    opcodes_[0x7E] = &GZ80::OPCode0x7E;
    opcodes_[0x7F] = &GZ80::OPCode0x7F;

    opcodes_[0x80] = &GZ80::OPCode0x80;
    opcodes_[0x81] = &GZ80::OPCode0x81;
    opcodes_[0x82] = &GZ80::OPCode0x82;
    opcodes_[0x83] = &GZ80::OPCode0x83;
    opcodes_[0x84] = &GZ80::OPCode0x84;
    opcodes_[0x85] = &GZ80::OPCode0x85;
    opcodes_[0x86] = &GZ80::OPCode0x86;
    opcodes_[0x87] = &GZ80::OPCode0x87;
    opcodes_[0x88] = &GZ80::OPCode0x88;
    opcodes_[0x89] = &GZ80::OPCode0x89;
    opcodes_[0x8A] = &GZ80::OPCode0x8A;
    opcodes_[0x8B] = &GZ80::OPCode0x8B;
    opcodes_[0x8C] = &GZ80::OPCode0x8C;
    opcodes_[0x8D] = &GZ80::OPCode0x8D;
    opcodes_[0x8E] = &GZ80::OPCode0x8E;
    opcodes_[0x8F] = &GZ80::OPCode0x8F;

    opcodes_[0x90] = &GZ80::OPCode0x90;
    opcodes_[0x91] = &GZ80::OPCode0x91;
    opcodes_[0x92] = &GZ80::OPCode0x92;
    opcodes_[0x93] = &GZ80::OPCode0x93;
    opcodes_[0x94] = &GZ80::OPCode0x94;
    opcodes_[0x95] = &GZ80::OPCode0x95;
    opcodes_[0x96] = &GZ80::OPCode0x96;
    opcodes_[0x97] = &GZ80::OPCode0x97;
    opcodes_[0x98] = &GZ80::OPCode0x98;
    opcodes_[0x99] = &GZ80::OPCode0x99;
    opcodes_[0x9A] = &GZ80::OPCode0x9A;
    opcodes_[0x9B] = &GZ80::OPCode0x9B;
    opcodes_[0x9C] = &GZ80::OPCode0x9C;
    opcodes_[0x9D] = &GZ80::OPCode0x9D;
    opcodes_[0x9E] = &GZ80::OPCode0x9E;
    opcodes_[0x9F] = &GZ80::OPCode0x9F;

    opcodes_[0xA0] = &GZ80::OPCode0xA0;
    opcodes_[0xA1] = &GZ80::OPCode0xA1;
    opcodes_[0xA2] = &GZ80::OPCode0xA2;
    opcodes_[0xA3] = &GZ80::OPCode0xA3;
    opcodes_[0xA4] = &GZ80::OPCode0xA4;
    opcodes_[0xA5] = &GZ80::OPCode0xA5;
    opcodes_[0xA6] = &GZ80::OPCode0xA6;
    opcodes_[0xA7] = &GZ80::OPCode0xA7;
    opcodes_[0xA8] = &GZ80::OPCode0xA8;
    opcodes_[0xA9] = &GZ80::OPCode0xA9;
    opcodes_[0xAA] = &GZ80::OPCode0xAA;
    opcodes_[0xAB] = &GZ80::OPCode0xAB;
    opcodes_[0xAC] = &GZ80::OPCode0xAC;
    opcodes_[0xAD] = &GZ80::OPCode0xAD;
    opcodes_[0xAE] = &GZ80::OPCode0xAE;
    opcodes_[0xAF] = &GZ80::OPCode0xAF;

    opcodes_[0xB0] = &GZ80::OPCode0xB0;
    opcodes_[0xB1] = &GZ80::OPCode0xB1;
    opcodes_[0xB2] = &GZ80::OPCode0xB2;
    opcodes_[0xB3] = &GZ80::OPCode0xB3;
    opcodes_[0xB4] = &GZ80::OPCode0xB4;
    opcodes_[0xB5] = &GZ80::OPCode0xB5;
    opcodes_[0xB6] = &GZ80::OPCode0xB6;
    opcodes_[0xB7] = &GZ80::OPCode0xB7;
    opcodes_[0xB8] = &GZ80::OPCode0xB8;
    opcodes_[0xB9] = &GZ80::OPCode0xB9;
    opcodes_[0xBA] = &GZ80::OPCode0xBA;
    opcodes_[0xBB] = &GZ80::OPCode0xBB;
    opcodes_[0xBC] = &GZ80::OPCode0xBC;
    opcodes_[0xBD] = &GZ80::OPCode0xBD;
    opcodes_[0xBE] = &GZ80::OPCode0xBE;
    opcodes_[0xBF] = &GZ80::OPCode0xBF;

    opcodes_[0xC0] = &GZ80::OPCode0xC0;
    opcodes_[0xC1] = &GZ80::OPCode0xC1;
    opcodes_[0xC2] = &GZ80::OPCode0xC2;
    opcodes_[0xC3] = &GZ80::OPCode0xC3;
    opcodes_[0xC4] = &GZ80::OPCode0xC4;
    opcodes_[0xC5] = &GZ80::OPCode0xC5;
    opcodes_[0xC6] = &GZ80::OPCode0xC6;
    opcodes_[0xC7] = &GZ80::OPCode0xC7;
    opcodes_[0xC8] = &GZ80::OPCode0xC8;
    opcodes_[0xC9] = &GZ80::OPCode0xC9;
    opcodes_[0xCA] = &GZ80::OPCode0xCA;
    opcodes_[0xCB] = &GZ80::OPCode0xCB;
    opcodes_[0xCC] = &GZ80::OPCode0xCC;
    opcodes_[0xCD] = &GZ80::OPCode0xCD;
    opcodes_[0xCE] = &GZ80::OPCode0xCE;
    opcodes_[0xCF] = &GZ80::OPCode0xCF;

    opcodes_[0xD0] = &GZ80::OPCode0xD0;
    opcodes_[0xD1] = &GZ80::OPCode0xD1;
    opcodes_[0xD2] = &GZ80::OPCode0xD2;
    opcodes_[0xD3] = &GZ80::OPCode0xD3;
    opcodes_[0xD4] = &GZ80::OPCode0xD4;
    opcodes_[0xD5] = &GZ80::OPCode0xD5;
    opcodes_[0xD6] = &GZ80::OPCode0xD6;
    opcodes_[0xD7] = &GZ80::OPCode0xD7;
    opcodes_[0xD8] = &GZ80::OPCode0xD8;
    opcodes_[0xD9] = &GZ80::OPCode0xD9;
    opcodes_[0xDA] = &GZ80::OPCode0xDA;
    opcodes_[0xDB] = &GZ80::OPCode0xDB;
    opcodes_[0xDC] = &GZ80::OPCode0xDC;
    opcodes_[0xDD] = &GZ80::OPCode0xDD;
    opcodes_[0xDE] = &GZ80::OPCode0xDE;
    opcodes_[0xDF] = &GZ80::OPCode0xDF;

    opcodes_[0xE0] = &GZ80::OPCode0xE0;
    opcodes_[0xE1] = &GZ80::OPCode0xE1;
    opcodes_[0xE2] = &GZ80::OPCode0xE2;
    opcodes_[0xE3] = &GZ80::OPCode0xE3;
    opcodes_[0xE4] = &GZ80::OPCode0xE4;
    opcodes_[0xE5] = &GZ80::OPCode0xE5;
    opcodes_[0xE6] = &GZ80::OPCode0xE6;
    opcodes_[0xE7] = &GZ80::OPCode0xE7;
    opcodes_[0xE8] = &GZ80::OPCode0xE8;
    opcodes_[0xE9] = &GZ80::OPCode0xE9;
    opcodes_[0xEA] = &GZ80::OPCode0xEA;
    opcodes_[0xEB] = &GZ80::OPCode0xEB;
    opcodes_[0xEC] = &GZ80::OPCode0xEC;
    opcodes_[0xED] = &GZ80::OPCode0xED;
    opcodes_[0xEE] = &GZ80::OPCode0xEE;
    opcodes_[0xEF] = &GZ80::OPCode0xEF;

    opcodes_[0xF0] = &GZ80::OPCode0xF0;
    opcodes_[0xF1] = &GZ80::OPCode0xF1;
    opcodes_[0xF2] = &GZ80::OPCode0xF2;
    opcodes_[0xF3] = &GZ80::OPCode0xF3;
    opcodes_[0xF4] = &GZ80::OPCode0xF4;
    opcodes_[0xF5] = &GZ80::OPCode0xF5;
    opcodes_[0xF6] = &GZ80::OPCode0xF6;
    opcodes_[0xF7] = &GZ80::OPCode0xF7;
    opcodes_[0xF8] = &GZ80::OPCode0xF8;
    opcodes_[0xF9] = &GZ80::OPCode0xF9;
    opcodes_[0xFA] = &GZ80::OPCode0xFA;
    opcodes_[0xFB] = &GZ80::OPCode0xFB;
    opcodes_[0xFC] = &GZ80::OPCode0xFC;
    opcodes_[0xFD] = &GZ80::OPCode0xFD;
    opcodes_[0xFE] = &GZ80::OPCode0xFE;
    opcodes_[0xFF] = &GZ80::OPCode0xFF;


    opcodes_cb_[0x00] = &GZ80::OPCodeCB0x00;
    opcodes_cb_[0x01] = &GZ80::OPCodeCB0x01;
    opcodes_cb_[0x02] = &GZ80::OPCodeCB0x02;
    opcodes_cb_[0x03] = &GZ80::OPCodeCB0x03;
    opcodes_cb_[0x04] = &GZ80::OPCodeCB0x04;
    opcodes_cb_[0x05] = &GZ80::OPCodeCB0x05;
    opcodes_cb_[0x06] = &GZ80::OPCodeCB0x06;
    opcodes_cb_[0x07] = &GZ80::OPCodeCB0x07;
    opcodes_cb_[0x08] = &GZ80::OPCodeCB0x08;
    opcodes_cb_[0x09] = &GZ80::OPCodeCB0x09;
    opcodes_cb_[0x0A] = &GZ80::OPCodeCB0x0A;
    opcodes_cb_[0x0B] = &GZ80::OPCodeCB0x0B;
    opcodes_cb_[0x0C] = &GZ80::OPCodeCB0x0C;
    opcodes_cb_[0x0D] = &GZ80::OPCodeCB0x0D;
    opcodes_cb_[0x0E] = &GZ80::OPCodeCB0x0E;
    opcodes_cb_[0x0F] = &GZ80::OPCodeCB0x0F;

    opcodes_cb_[0x10] = &GZ80::OPCodeCB0x10;
    opcodes_cb_[0x11] = &GZ80::OPCodeCB0x11;
    opcodes_cb_[0x12] = &GZ80::OPCodeCB0x12;
    opcodes_cb_[0x13] = &GZ80::OPCodeCB0x13;
    opcodes_cb_[0x14] = &GZ80::OPCodeCB0x14;
    opcodes_cb_[0x15] = &GZ80::OPCodeCB0x15;
    opcodes_cb_[0x16] = &GZ80::OPCodeCB0x16;
    opcodes_cb_[0x17] = &GZ80::OPCodeCB0x17;
    opcodes_cb_[0x18] = &GZ80::OPCodeCB0x18;
    opcodes_cb_[0x19] = &GZ80::OPCodeCB0x19;
    opcodes_cb_[0x1A] = &GZ80::OPCodeCB0x1A;
    opcodes_cb_[0x1B] = &GZ80::OPCodeCB0x1B;
    opcodes_cb_[0x1C] = &GZ80::OPCodeCB0x1C;
    opcodes_cb_[0x1D] = &GZ80::OPCodeCB0x1D;
    opcodes_cb_[0x1E] = &GZ80::OPCodeCB0x1E;
    opcodes_cb_[0x1F] = &GZ80::OPCodeCB0x1F;

    opcodes_cb_[0x20] = &GZ80::OPCodeCB0x20;
    opcodes_cb_[0x21] = &GZ80::OPCodeCB0x21;
    opcodes_cb_[0x22] = &GZ80::OPCodeCB0x22;
    opcodes_cb_[0x23] = &GZ80::OPCodeCB0x23;
    opcodes_cb_[0x24] = &GZ80::OPCodeCB0x24;
    opcodes_cb_[0x25] = &GZ80::OPCodeCB0x25;
    opcodes_cb_[0x26] = &GZ80::OPCodeCB0x26;
    opcodes_cb_[0x27] = &GZ80::OPCodeCB0x27;
    opcodes_cb_[0x28] = &GZ80::OPCodeCB0x28;
    opcodes_cb_[0x29] = &GZ80::OPCodeCB0x29;
    opcodes_cb_[0x2A] = &GZ80::OPCodeCB0x2A;
    opcodes_cb_[0x2B] = &GZ80::OPCodeCB0x2B;
    opcodes_cb_[0x2C] = &GZ80::OPCodeCB0x2C;
    opcodes_cb_[0x2D] = &GZ80::OPCodeCB0x2D;
    opcodes_cb_[0x2E] = &GZ80::OPCodeCB0x2E;
    opcodes_cb_[0x2F] = &GZ80::OPCodeCB0x2F;

    opcodes_cb_[0x30] = &GZ80::OPCodeCB0x30;
    opcodes_cb_[0x31] = &GZ80::OPCodeCB0x31;
    opcodes_cb_[0x32] = &GZ80::OPCodeCB0x32;
    opcodes_cb_[0x33] = &GZ80::OPCodeCB0x33;
    opcodes_cb_[0x34] = &GZ80::OPCodeCB0x34;
    opcodes_cb_[0x35] = &GZ80::OPCodeCB0x35;
    opcodes_cb_[0x36] = &GZ80::OPCodeCB0x36;
    opcodes_cb_[0x37] = &GZ80::OPCodeCB0x37;
    opcodes_cb_[0x38] = &GZ80::OPCodeCB0x38;
    opcodes_cb_[0x39] = &GZ80::OPCodeCB0x39;
    opcodes_cb_[0x3A] = &GZ80::OPCodeCB0x3A;
    opcodes_cb_[0x3B] = &GZ80::OPCodeCB0x3B;
    opcodes_cb_[0x3C] = &GZ80::OPCodeCB0x3C;
    opcodes_cb_[0x3D] = &GZ80::OPCodeCB0x3D;
    opcodes_cb_[0x3E] = &GZ80::OPCodeCB0x3E;
    opcodes_cb_[0x3F] = &GZ80::OPCodeCB0x3F;

    opcodes_cb_[0x40] = &GZ80::OPCodeCB0x40;
    opcodes_cb_[0x41] = &GZ80::OPCodeCB0x41;
    opcodes_cb_[0x42] = &GZ80::OPCodeCB0x42;
    opcodes_cb_[0x43] = &GZ80::OPCodeCB0x43;
    opcodes_cb_[0x44] = &GZ80::OPCodeCB0x44;
    opcodes_cb_[0x45] = &GZ80::OPCodeCB0x45;
    opcodes_cb_[0x46] = &GZ80::OPCodeCB0x46;
    opcodes_cb_[0x47] = &GZ80::OPCodeCB0x47;
    opcodes_cb_[0x48] = &GZ80::OPCodeCB0x48;
    opcodes_cb_[0x49] = &GZ80::OPCodeCB0x49;
    opcodes_cb_[0x4A] = &GZ80::OPCodeCB0x4A;
    opcodes_cb_[0x4B] = &GZ80::OPCodeCB0x4B;
    opcodes_cb_[0x4C] = &GZ80::OPCodeCB0x4C;
    opcodes_cb_[0x4D] = &GZ80::OPCodeCB0x4D;
    opcodes_cb_[0x4E] = &GZ80::OPCodeCB0x4E;
    opcodes_cb_[0x4F] = &GZ80::OPCodeCB0x4F;

    opcodes_cb_[0x50] = &GZ80::OPCodeCB0x50;
    opcodes_cb_[0x51] = &GZ80::OPCodeCB0x51;
    opcodes_cb_[0x52] = &GZ80::OPCodeCB0x52;
    opcodes_cb_[0x53] = &GZ80::OPCodeCB0x53;
    opcodes_cb_[0x54] = &GZ80::OPCodeCB0x54;
    opcodes_cb_[0x55] = &GZ80::OPCodeCB0x55;
    opcodes_cb_[0x56] = &GZ80::OPCodeCB0x56;
    opcodes_cb_[0x57] = &GZ80::OPCodeCB0x57;
    opcodes_cb_[0x58] = &GZ80::OPCodeCB0x58;
    opcodes_cb_[0x59] = &GZ80::OPCodeCB0x59;
    opcodes_cb_[0x5A] = &GZ80::OPCodeCB0x5A;
    opcodes_cb_[0x5B] = &GZ80::OPCodeCB0x5B;
    opcodes_cb_[0x5C] = &GZ80::OPCodeCB0x5C;
    opcodes_cb_[0x5D] = &GZ80::OPCodeCB0x5D;
    opcodes_cb_[0x5E] = &GZ80::OPCodeCB0x5E;
    opcodes_cb_[0x5F] = &GZ80::OPCodeCB0x5F;

    opcodes_cb_[0x60] = &GZ80::OPCodeCB0x60;
    opcodes_cb_[0x61] = &GZ80::OPCodeCB0x61;
    opcodes_cb_[0x62] = &GZ80::OPCodeCB0x62;
    opcodes_cb_[0x63] = &GZ80::OPCodeCB0x63;
    opcodes_cb_[0x64] = &GZ80::OPCodeCB0x64;
    opcodes_cb_[0x65] = &GZ80::OPCodeCB0x65;
    opcodes_cb_[0x66] = &GZ80::OPCodeCB0x66;
    opcodes_cb_[0x67] = &GZ80::OPCodeCB0x67;
    opcodes_cb_[0x68] = &GZ80::OPCodeCB0x68;
    opcodes_cb_[0x69] = &GZ80::OPCodeCB0x69;
    opcodes_cb_[0x6A] = &GZ80::OPCodeCB0x6A;
    opcodes_cb_[0x6B] = &GZ80::OPCodeCB0x6B;
    opcodes_cb_[0x6C] = &GZ80::OPCodeCB0x6C;
    opcodes_cb_[0x6D] = &GZ80::OPCodeCB0x6D;
    opcodes_cb_[0x6E] = &GZ80::OPCodeCB0x6E;
    opcodes_cb_[0x6F] = &GZ80::OPCodeCB0x6F;

    opcodes_cb_[0x70] = &GZ80::OPCodeCB0x70;
    opcodes_cb_[0x71] = &GZ80::OPCodeCB0x71;
    opcodes_cb_[0x72] = &GZ80::OPCodeCB0x72;
    opcodes_cb_[0x73] = &GZ80::OPCodeCB0x73;
    opcodes_cb_[0x74] = &GZ80::OPCodeCB0x74;
    opcodes_cb_[0x75] = &GZ80::OPCodeCB0x75;
    opcodes_cb_[0x76] = &GZ80::OPCodeCB0x76;
    opcodes_cb_[0x77] = &GZ80::OPCodeCB0x77;
    opcodes_cb_[0x78] = &GZ80::OPCodeCB0x78;
    opcodes_cb_[0x79] = &GZ80::OPCodeCB0x79;
    opcodes_cb_[0x7A] = &GZ80::OPCodeCB0x7A;
    opcodes_cb_[0x7B] = &GZ80::OPCodeCB0x7B;
    opcodes_cb_[0x7C] = &GZ80::OPCodeCB0x7C;
    opcodes_cb_[0x7D] = &GZ80::OPCodeCB0x7D;
    opcodes_cb_[0x7E] = &GZ80::OPCodeCB0x7E;
    opcodes_cb_[0x7F] = &GZ80::OPCodeCB0x7F;

    opcodes_cb_[0x80] = &GZ80::OPCodeCB0x80;
    opcodes_cb_[0x81] = &GZ80::OPCodeCB0x81;
    opcodes_cb_[0x82] = &GZ80::OPCodeCB0x82;
    opcodes_cb_[0x83] = &GZ80::OPCodeCB0x83;
    opcodes_cb_[0x84] = &GZ80::OPCodeCB0x84;
    opcodes_cb_[0x85] = &GZ80::OPCodeCB0x85;
    opcodes_cb_[0x86] = &GZ80::OPCodeCB0x86;
    opcodes_cb_[0x87] = &GZ80::OPCodeCB0x87;
    opcodes_cb_[0x88] = &GZ80::OPCodeCB0x88;
    opcodes_cb_[0x89] = &GZ80::OPCodeCB0x89;
    opcodes_cb_[0x8A] = &GZ80::OPCodeCB0x8A;
    opcodes_cb_[0x8B] = &GZ80::OPCodeCB0x8B;
    opcodes_cb_[0x8C] = &GZ80::OPCodeCB0x8C;
    opcodes_cb_[0x8D] = &GZ80::OPCodeCB0x8D;
    opcodes_cb_[0x8E] = &GZ80::OPCodeCB0x8E;
    opcodes_cb_[0x8F] = &GZ80::OPCodeCB0x8F;

    opcodes_cb_[0x90] = &GZ80::OPCodeCB0x90;
    opcodes_cb_[0x91] = &GZ80::OPCodeCB0x91;
    opcodes_cb_[0x92] = &GZ80::OPCodeCB0x92;
    opcodes_cb_[0x93] = &GZ80::OPCodeCB0x93;
    opcodes_cb_[0x94] = &GZ80::OPCodeCB0x94;
    opcodes_cb_[0x95] = &GZ80::OPCodeCB0x95;
    opcodes_cb_[0x96] = &GZ80::OPCodeCB0x96;
    opcodes_cb_[0x97] = &GZ80::OPCodeCB0x97;
    opcodes_cb_[0x98] = &GZ80::OPCodeCB0x98;
    opcodes_cb_[0x99] = &GZ80::OPCodeCB0x99;
    opcodes_cb_[0x9A] = &GZ80::OPCodeCB0x9A;
    opcodes_cb_[0x9B] = &GZ80::OPCodeCB0x9B;
    opcodes_cb_[0x9C] = &GZ80::OPCodeCB0x9C;
    opcodes_cb_[0x9D] = &GZ80::OPCodeCB0x9D;
    opcodes_cb_[0x9E] = &GZ80::OPCodeCB0x9E;
    opcodes_cb_[0x9F] = &GZ80::OPCodeCB0x9F;

    opcodes_cb_[0xA0] = &GZ80::OPCodeCB0xA0;
    opcodes_cb_[0xA1] = &GZ80::OPCodeCB0xA1;
    opcodes_cb_[0xA2] = &GZ80::OPCodeCB0xA2;
    opcodes_cb_[0xA3] = &GZ80::OPCodeCB0xA3;
    opcodes_cb_[0xA4] = &GZ80::OPCodeCB0xA4;
    opcodes_cb_[0xA5] = &GZ80::OPCodeCB0xA5;
    opcodes_cb_[0xA6] = &GZ80::OPCodeCB0xA6;
    opcodes_cb_[0xA7] = &GZ80::OPCodeCB0xA7;
    opcodes_cb_[0xA8] = &GZ80::OPCodeCB0xA8;
    opcodes_cb_[0xA9] = &GZ80::OPCodeCB0xA9;
    opcodes_cb_[0xAA] = &GZ80::OPCodeCB0xAA;
    opcodes_cb_[0xAB] = &GZ80::OPCodeCB0xAB;
    opcodes_cb_[0xAC] = &GZ80::OPCodeCB0xAC;
    opcodes_cb_[0xAD] = &GZ80::OPCodeCB0xAD;
    opcodes_cb_[0xAE] = &GZ80::OPCodeCB0xAE;
    opcodes_cb_[0xAF] = &GZ80::OPCodeCB0xAF;

    opcodes_cb_[0xB0] = &GZ80::OPCodeCB0xB0;
    opcodes_cb_[0xB1] = &GZ80::OPCodeCB0xB1;
    opcodes_cb_[0xB2] = &GZ80::OPCodeCB0xB2;
    opcodes_cb_[0xB3] = &GZ80::OPCodeCB0xB3;
    opcodes_cb_[0xB4] = &GZ80::OPCodeCB0xB4;
    opcodes_cb_[0xB5] = &GZ80::OPCodeCB0xB5;
    opcodes_cb_[0xB6] = &GZ80::OPCodeCB0xB6;
    opcodes_cb_[0xB7] = &GZ80::OPCodeCB0xB7;
    opcodes_cb_[0xB8] = &GZ80::OPCodeCB0xB8;
    opcodes_cb_[0xB9] = &GZ80::OPCodeCB0xB9;
    opcodes_cb_[0xBA] = &GZ80::OPCodeCB0xBA;
    opcodes_cb_[0xBB] = &GZ80::OPCodeCB0xBB;
    opcodes_cb_[0xBC] = &GZ80::OPCodeCB0xBC;
    opcodes_cb_[0xBD] = &GZ80::OPCodeCB0xBD;
    opcodes_cb_[0xBE] = &GZ80::OPCodeCB0xBE;
    opcodes_cb_[0xBF] = &GZ80::OPCodeCB0xBF;

    opcodes_cb_[0xC0] = &GZ80::OPCodeCB0xC0;
    opcodes_cb_[0xC1] = &GZ80::OPCodeCB0xC1;
    opcodes_cb_[0xC2] = &GZ80::OPCodeCB0xC2;
    opcodes_cb_[0xC3] = &GZ80::OPCodeCB0xC3;
    opcodes_cb_[0xC4] = &GZ80::OPCodeCB0xC4;
    opcodes_cb_[0xC5] = &GZ80::OPCodeCB0xC5;
    opcodes_cb_[0xC6] = &GZ80::OPCodeCB0xC6;
    opcodes_cb_[0xC7] = &GZ80::OPCodeCB0xC7;
    opcodes_cb_[0xC8] = &GZ80::OPCodeCB0xC8;
    opcodes_cb_[0xC9] = &GZ80::OPCodeCB0xC9;
    opcodes_cb_[0xCA] = &GZ80::OPCodeCB0xCA;
    opcodes_cb_[0xCB] = &GZ80::OPCodeCB0xCB;
    opcodes_cb_[0xCC] = &GZ80::OPCodeCB0xCC;
    opcodes_cb_[0xCD] = &GZ80::OPCodeCB0xCD;
    opcodes_cb_[0xCE] = &GZ80::OPCodeCB0xCE;
    opcodes_cb_[0xCF] = &GZ80::OPCodeCB0xCF;

    opcodes_cb_[0xD0] = &GZ80::OPCodeCB0xD0;
    opcodes_cb_[0xD1] = &GZ80::OPCodeCB0xD1;
    opcodes_cb_[0xD2] = &GZ80::OPCodeCB0xD2;
    opcodes_cb_[0xD3] = &GZ80::OPCodeCB0xD3;
    opcodes_cb_[0xD4] = &GZ80::OPCodeCB0xD4;
    opcodes_cb_[0xD5] = &GZ80::OPCodeCB0xD5;
    opcodes_cb_[0xD6] = &GZ80::OPCodeCB0xD6;
    opcodes_cb_[0xD7] = &GZ80::OPCodeCB0xD7;
    opcodes_cb_[0xD8] = &GZ80::OPCodeCB0xD8;
    opcodes_cb_[0xD9] = &GZ80::OPCodeCB0xD9;
    opcodes_cb_[0xDA] = &GZ80::OPCodeCB0xDA;
    opcodes_cb_[0xDB] = &GZ80::OPCodeCB0xDB;
    opcodes_cb_[0xDC] = &GZ80::OPCodeCB0xDC;
    opcodes_cb_[0xDD] = &GZ80::OPCodeCB0xDD;
    opcodes_cb_[0xDE] = &GZ80::OPCodeCB0xDE;
    opcodes_cb_[0xDF] = &GZ80::OPCodeCB0xDF;

    opcodes_cb_[0xE0] = &GZ80::OPCodeCB0xE0;
    opcodes_cb_[0xE1] = &GZ80::OPCodeCB0xE1;
    opcodes_cb_[0xE2] = &GZ80::OPCodeCB0xE2;
    opcodes_cb_[0xE3] = &GZ80::OPCodeCB0xE3;
    opcodes_cb_[0xE4] = &GZ80::OPCodeCB0xE4;
    opcodes_cb_[0xE5] = &GZ80::OPCodeCB0xE5;
    opcodes_cb_[0xE6] = &GZ80::OPCodeCB0xE6;
    opcodes_cb_[0xE7] = &GZ80::OPCodeCB0xE7;
    opcodes_cb_[0xE8] = &GZ80::OPCodeCB0xE8;
    opcodes_cb_[0xE9] = &GZ80::OPCodeCB0xE9;
    opcodes_cb_[0xEA] = &GZ80::OPCodeCB0xEA;
    opcodes_cb_[0xEB] = &GZ80::OPCodeCB0xEB;
    opcodes_cb_[0xEC] = &GZ80::OPCodeCB0xEC;
    opcodes_cb_[0xED] = &GZ80::OPCodeCB0xED;
    opcodes_cb_[0xEE] = &GZ80::OPCodeCB0xEE;
    opcodes_cb_[0xEF] = &GZ80::OPCodeCB0xEF;

    opcodes_cb_[0xF0] = &GZ80::OPCodeCB0xF0;
    opcodes_cb_[0xF1] = &GZ80::OPCodeCB0xF1;
    opcodes_cb_[0xF2] = &GZ80::OPCodeCB0xF2;
    opcodes_cb_[0xF3] = &GZ80::OPCodeCB0xF3;
    opcodes_cb_[0xF4] = &GZ80::OPCodeCB0xF4;
    opcodes_cb_[0xF5] = &GZ80::OPCodeCB0xF5;
    opcodes_cb_[0xF6] = &GZ80::OPCodeCB0xF6;
    opcodes_cb_[0xF7] = &GZ80::OPCodeCB0xF7;
    opcodes_cb_[0xF8] = &GZ80::OPCodeCB0xF8;
    opcodes_cb_[0xF9] = &GZ80::OPCodeCB0xF9;
    opcodes_cb_[0xFA] = &GZ80::OPCodeCB0xFA;
    opcodes_cb_[0xFB] = &GZ80::OPCodeCB0xFB;
    opcodes_cb_[0xFC] = &GZ80::OPCodeCB0xFC;
    opcodes_cb_[0xFD] = &GZ80::OPCodeCB0xFD;
    opcodes_cb_[0xFE] = &GZ80::OPCodeCB0xFE;
    opcodes_cb_[0xFF] = &GZ80::OPCodeCB0xFF;

    for (int i = 0x00; i < 0x40; i++)
    {
        opcodes_ed_[i] = &GZ80::InvalidOPCode;
    }

    opcodes_ed_[0x40] = &GZ80::OPCodeED0x40;
    opcodes_ed_[0x41] = &GZ80::OPCodeED0x41;
    opcodes_ed_[0x42] = &GZ80::OPCodeED0x42;
    opcodes_ed_[0x43] = &GZ80::OPCodeED0x43;
    opcodes_ed_[0x44] = &GZ80::OPCodeED0x44;
    opcodes_ed_[0x45] = &GZ80::OPCodeED0x45;
    opcodes_ed_[0x46] = &GZ80::OPCodeED0x46;
    opcodes_ed_[0x47] = &GZ80::OPCodeED0x47;
    opcodes_ed_[0x48] = &GZ80::OPCodeED0x48;
    opcodes_ed_[0x49] = &GZ80::OPCodeED0x49;
    opcodes_ed_[0x4A] = &GZ80::OPCodeED0x4A;
    opcodes_ed_[0x4B] = &GZ80::OPCodeED0x4B;
    opcodes_ed_[0x4C] = &GZ80::OPCodeED0x4C;
    opcodes_ed_[0x4D] = &GZ80::OPCodeED0x4D;
    opcodes_ed_[0x4E] = &GZ80::OPCodeED0x4E;
    opcodes_ed_[0x4F] = &GZ80::OPCodeED0x4F;

    opcodes_ed_[0x50] = &GZ80::OPCodeED0x50;
    opcodes_ed_[0x51] = &GZ80::OPCodeED0x51;
    opcodes_ed_[0x52] = &GZ80::OPCodeED0x52;
    opcodes_ed_[0x53] = &GZ80::OPCodeED0x53;
    opcodes_ed_[0x54] = &GZ80::OPCodeED0x54;
    opcodes_ed_[0x55] = &GZ80::OPCodeED0x55;
    opcodes_ed_[0x56] = &GZ80::OPCodeED0x56;
    opcodes_ed_[0x57] = &GZ80::OPCodeED0x57;
    opcodes_ed_[0x58] = &GZ80::OPCodeED0x58;
    opcodes_ed_[0x59] = &GZ80::OPCodeED0x59;
    opcodes_ed_[0x5A] = &GZ80::OPCodeED0x5A;
    opcodes_ed_[0x5B] = &GZ80::OPCodeED0x5B;
    opcodes_ed_[0x5C] = &GZ80::OPCodeED0x5C;
    opcodes_ed_[0x5D] = &GZ80::OPCodeED0x5D;
    opcodes_ed_[0x5E] = &GZ80::OPCodeED0x5E;
    opcodes_ed_[0x5F] = &GZ80::OPCodeED0x5F;

    opcodes_ed_[0x60] = &GZ80::OPCodeED0x60;
    opcodes_ed_[0x61] = &GZ80::OPCodeED0x61;
    opcodes_ed_[0x62] = &GZ80::OPCodeED0x62;
    opcodes_ed_[0x63] = &GZ80::OPCodeED0x63;
    opcodes_ed_[0x64] = &GZ80::OPCodeED0x64;
    opcodes_ed_[0x65] = &GZ80::OPCodeED0x65;
    opcodes_ed_[0x66] = &GZ80::OPCodeED0x66;
    opcodes_ed_[0x67] = &GZ80::OPCodeED0x67;
    opcodes_ed_[0x68] = &GZ80::OPCodeED0x68;
    opcodes_ed_[0x69] = &GZ80::OPCodeED0x69;
    opcodes_ed_[0x6A] = &GZ80::OPCodeED0x6A;
    opcodes_ed_[0x6B] = &GZ80::OPCodeED0x6B;
    opcodes_ed_[0x6C] = &GZ80::OPCodeED0x6C;
    opcodes_ed_[0x6D] = &GZ80::OPCodeED0x6D;
    opcodes_ed_[0x6E] = &GZ80::OPCodeED0x6E;
    opcodes_ed_[0x6F] = &GZ80::OPCodeED0x6F;

    opcodes_ed_[0x70] = &GZ80::OPCodeED0x70;
    opcodes_ed_[0x71] = &GZ80::OPCodeED0x71;
    opcodes_ed_[0x72] = &GZ80::OPCodeED0x72;
    opcodes_ed_[0x73] = &GZ80::OPCodeED0x73;
    opcodes_ed_[0x74] = &GZ80::OPCodeED0x74;
    opcodes_ed_[0x75] = &GZ80::OPCodeED0x75;
    opcodes_ed_[0x76] = &GZ80::OPCodeED0x76;
    opcodes_ed_[0x77] = &GZ80::InvalidOPCode;
    opcodes_ed_[0x78] = &GZ80::OPCodeED0x78;
    opcodes_ed_[0x79] = &GZ80::OPCodeED0x79;
    opcodes_ed_[0x7A] = &GZ80::OPCodeED0x7A;
    opcodes_ed_[0x7B] = &GZ80::OPCodeED0x7B;
    opcodes_ed_[0x7C] = &GZ80::OPCodeED0x7C;
    opcodes_ed_[0x7D] = &GZ80::OPCodeED0x7D;
    opcodes_ed_[0x7E] = &GZ80::OPCodeED0x7E;

    for (int i = 0x7F; i < 0xA0; i++)
    {
        opcodes_ed_[i] = &GZ80::InvalidOPCode;
    }

    opcodes_ed_[0xA0] = &GZ80::OPCodeED0xA0;
    opcodes_ed_[0xA1] = &GZ80::OPCodeED0xA1;
    opcodes_ed_[0xA2] = &GZ80::OPCodeED0xA2;
    opcodes_ed_[0xA3] = &GZ80::OPCodeED0xA3;
    opcodes_ed_[0xA4] = &GZ80::InvalidOPCode;
    opcodes_ed_[0xA5] = &GZ80::InvalidOPCode;
    opcodes_ed_[0xA6] = &GZ80::InvalidOPCode;
    opcodes_ed_[0xA7] = &GZ80::InvalidOPCode;
    opcodes_ed_[0xA8] = &GZ80::OPCodeED0xA8;
    opcodes_ed_[0xA9] = &GZ80::OPCodeED0xA9;
    opcodes_ed_[0xAA] = &GZ80::OPCodeED0xAA;
    opcodes_ed_[0xAB] = &GZ80::OPCodeED0xAB;
    opcodes_ed_[0xAC] = &GZ80::InvalidOPCode;
    opcodes_ed_[0xAD] = &GZ80::InvalidOPCode;
    opcodes_ed_[0xAE] = &GZ80::InvalidOPCode;
    opcodes_ed_[0xAF] = &GZ80::InvalidOPCode;

    opcodes_ed_[0xB0] = &GZ80::OPCodeED0xB0;
    opcodes_ed_[0xB1] = &GZ80::OPCodeED0xB1;
    opcodes_ed_[0xB2] = &GZ80::OPCodeED0xB2;
    opcodes_ed_[0xB3] = &GZ80::OPCodeED0xB3;
    opcodes_ed_[0xB4] = &GZ80::InvalidOPCode;
    opcodes_ed_[0xB5] = &GZ80::InvalidOPCode;
    opcodes_ed_[0xB6] = &GZ80::InvalidOPCode;
    opcodes_ed_[0xB7] = &GZ80::InvalidOPCode;
    opcodes_ed_[0xB8] = &GZ80::OPCodeED0xB8;
    opcodes_ed_[0xB9] = &GZ80::OPCodeED0xB9;
    opcodes_ed_[0xBA] = &GZ80::OPCodeED0xBA;
    opcodes_ed_[0xBB] = &GZ80::OPCodeED0xBB;

    for (int i = 0xBC; i <= 0xFF; i++)
    {
        opcodes_ed_[i] = &GZ80::InvalidOPCode;
    }
}

} // namespace gz80
