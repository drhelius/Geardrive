#ifndef GZ80_MEMORYINTERFACE_H_
#define	GZ80_MEMORYINTERFACE_H_

#include "gz80_definitions.h"

namespace gz80
{
    
class MemoryIterface
{
public:
    MemoryIterface() {};
    virtual ~MemoryIterface();
    virtual u8 Read(u16 address) = 0;
    virtual void Write(u16 address, u8 value) = 0;
    virtual void Disassemble(u16 address, const char* disassembled_string) = 0;
    virtual bool IsDisassembled(u16 address) = 0;
};

} // namespace gz80

#endif // GZ80_MEMORYINTERFACE_H_

