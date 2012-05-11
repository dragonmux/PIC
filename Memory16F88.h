#include "Memory.h"

class Memory16F88 : public Memory
{
private:
	uint8_t memory[512];
	std::map<uint32_t, MemoryLocation *> memoryMap;

protected:
	Memory16F88();

public:
	uint8_t Dereference(uint8_t bank, uint8_t partialAddress);
	uint8_t *Reference(uint8_t bank, uint8_t partialAddress);
	uint8_t *operator [](uint32_t ref);
};
