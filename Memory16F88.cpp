#include "Common.h"
#include "Memory.h"
#include "Memory16F88.h"

#define mapLocation(location) \
	memoryMap[location] = new MemoryLocationMapped(new Register<>());

#define mapRange(location, end) \
	for (uint16_t i = location; i <= end; i++) \
		mapLocation(location + i);

#define unmapLocation(location) \
	memoryMap[location] = new MemoryLocationUnmapped(&memory[location]);

#define unmapRange(location, end) \
	for (uint16_t i = location; i <= end; i++) \
		unmapLocation(location + i);

#define aliasLocation(from, to) \
	memoryMap[to] = memoryMap[from];

#define aliasRange(fromBase, toBase, number) \
	for (uint16_t i = 0; i < number; i++) \
		aliasLocation(fromBase + i, toBase + i);

Memory16F88::Memory16F88() : Memory()
{
	mapRange(0x0000, 0x0006);
	unmapRange(0x0007, 0x0009);
	mapRange(0x000A, 0x001A);
	unmapRange(0x001B, 0x001D);
	mapRange(0x001E, 0x001F);
	unmapRange(0x0020, 0x007F);
	aliasLocation(0x0000, 0x0080);
	mapLocation(0x0081);
	aliasRange(0x0002, 0x0082, 3);
	mapRange(0x0085, 0x0086);
	unmapRange(0x0087, 0x0089);
	aliasRange(0x000A, 0x008A, 2);
	mapRange(0x008C, 0x0090);
	unmapLocation(0x0091);
	mapRange(0x0092, 0x0094);
	unmapRange(0x0095, 0x0097);
	mapRange(0x0098, 0x0099);
	unmapLocation(0x009A);
	mapRange(0x009B, 0x009F);
	unmapRange(0x00A0, 0x00EF);
	aliasRange(0x0070, 0x00F0, 16);
	aliasRange(0x0000, 0x0100, 5);
	mapLocation(0x0105);
	aliasLocation(0x0006, 0x0106);
	unmapRange(0x0107, 0x0109);
	aliasRange(0x000A, 0x010A, 2);
	mapRange(0x010C, 0x010F);
	unmapRange(0x0110, 96);
	aliasRange(0x0070, 0x0170, 16);
	aliasRange(0x0180, 0x0080, 5);
	unmapLocation(0x0185);
	aliasLocation(0x0086, 0x0186);
	unmapRange(0x0187, 0x0189);
	aliasRange(0x000A, 0x018A, 2);
	mapRange(0x018C, 0x018F);
	unmapRange(0x018E, 0x01EF);
	aliasRange(0x0070, 0x01F0, 16);
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
