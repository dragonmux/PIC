#include "Common.h"

ROM::ROM(const char *program, uint32_t programLength) : maxAddress(programLength)
{
	Program = new char[maxAddress];
	memcpy(Program, program, maxAddress);
}

ROM::~ROM()
{
	delete Program;
}

uint16_t ROM::operator [](uint32_t ref) const
{
	if (ref > maxAddress - 2)
		throw new ROMException("ROM address out of range!");
	return *((uint16_t *)(Program[ref]));
}
