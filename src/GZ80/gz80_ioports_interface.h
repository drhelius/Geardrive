#ifndef GD_Z80_IOPORTS_INTERFACE_H_
#define	GD_Z80_IOPORTS_INTERFACE_H_

#include "gz80_definitions.h"

namespace gz80
{
    
class IOPortsInterface
{
public:
    IOPortsInterface() {};
    virtual ~IOPortsInterface();
    virtual u8 Input(u8 port) = 0;
    virtual void Output(u8 port, u8 value) = 0;
};

} // namespace gz80

#endif // GD_Z80_IOPORTS_INTERFACE_H_

