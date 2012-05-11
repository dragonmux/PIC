#include "Common.h"
#include "Memory.h"
#include "Memory16F88.h"

#define mapLocation(location) \
	memoryMap[location] = new MemoryLocationMapped(new Register<>());

#define unmapLocation(location) \
	memoryMap[location] = new MemoryLocationUnmapped(&memory[location]);

Memory16F88::Memory16F88() : Memory()
{
	mapLocation(0x0000);
	mapLocation(0x0001);
	mapLocation(0x0002);
	mapLocation(0x0003);
	mapLocation(0x0004);
	mapLocation(0x0005);
	mapLocation(0x0006);
	unmapLocation(0x0007);
	unmapLocation(0x0008);
	unmapLocation(0x0009);
	mapLocation(0x000A);
	mapLocation(0x000B);
	mapLocation(0x000C);
	mapLocation(0x000D);
	mapLocation(0x000E);
	mapLocation(0x000F);
	mapLocation(0x0010);
	mapLocation(0x0011);
	mapLocation(0x0012);
	mapLocation(0x0013);
	mapLocation(0x0014);
	mapLocation(0x0015);
	mapLocation(0x0016);
	mapLocation(0x0017);
	mapLocation(0x0018);
	mapLocation(0x0019);
	mapLocation(0x0020);
}

uint8_t Memory16F88::Dereference(uint8_t bank, uint8_t partialAddress)
{
	uint16_t address = (bank << 7) | (partialAddress & 0x7F);
	return *(*memoryMap[address]);
}

uint8_t *Memory16F88::Reference(uint8_t bank, uint8_t partialAddress)
{
	uint16_t address = (bank << 7) | (partialAddress & 0x7F);
	return *memoryMap[address];
}

uint8_t *Memory16F88::operator [](uint32_t ref)
{
	if (ref > 511)
		throw new MemoryException("Memory reference out of range!");
	return *memoryMap[ref];
}
