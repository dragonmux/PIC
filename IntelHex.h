#ifndef __IntelHex_H__
#define __IntelHex_H__

#include "Common.h"
#include <vector>

class IntelHexException
{
private:
	const char *error;

public:
	IntelHexException(const char *errorMsg);
	const char *what() const;
};

typedef struct _HexLine
{
	uint8_t length;
	uint16_t address;
	uint8_t recordType;
	char *data;
} HexLine;

class IntelHex
{
private:
	FILE *fileIHex;
	std::vector<HexLine> lines;

	bool Hex2Num(char c, uint8_t &num);
	bool readHexPair(uint8_t *number, uint8_t &sum);
	bool readLine(HexLine &line);
	bool verifyChecksum(uint8_t calcSum);
	void verifyIntegrity();

protected:
	IntelHex(FILE *file);

public:
	~IntelHex();
	static IntelHex *Open(const char *fileName);
	ROM *GenerateROM();
};

#endif /*__IntelHex_H__*/
