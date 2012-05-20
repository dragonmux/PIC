#include <stdio.h>
#include <string>
#include "Common.h"
#include "IntelHex.h"

IntelHex::IntelHex(FILE *file) : fileIHex(file)
{
	if (file == NULL)
		throw new IntelHexException("Cannot open the requested file");
}

IntelHex::~IntelHex()
{
	if (fileIHex != NULL)
		fclose(fileIHex);
}

// Returns true on error
bool IntelHex::Hex2Num(char c, uint8_t &num)
{
	if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
	{
		num <<= 4;
		c -= '0';
		if (c < 10)
			num += c;
		else if (c < 22)
			num += c - 7;
		else
			num += c - 39;
		return false;
	}
	return true;
}

bool IntelHex::readHexPair(uint8_t *number, uint8_t &sum)
{
	char numChar;
	uint8_t num = 0;
	if (fread(&numChar, 1, 1, fileIHex) != 1 || feof(fileIHex) != 0 || ferror(fileIHex) != 0)
		return true;
	if (Hex2Num(numChar, num))
		return true;
	if (fread(&numChar, 1, 1, fileIHex) != 1 || ferror(fileIHex) != 0)
		return true;
	if (Hex2Num(numChar, num))
		return true;
	*number = num;
	sum += num;
	return true;
}

// Returns true on error.
bool IntelHex::verifyChecksum(uint8_t calcSum)
{
	uint8_t sum, _ = 0;
	if (readHexPair(&sum, _) == false)
		return true;
	printf("Verifying line checksum %hhu == %hhu\n", sum, -calcSum);
	return sum != (uint8_t)(~calcSum + 1);
}

// This function VERY strictly enforces the format - mistakes are not tollerated.
bool IntelHex::readLine(HexLine &line)
{
	std::string data;
	uint16_t address;
	uint8_t curr, length, recordType, i, sum = 0;

	if (fread(&curr, 1, 1, fileIHex) != 1 || curr != ':')
		return false;
	if (readHexPair(&length, sum) == false)
		return false;

	if (readHexPair(&curr, sum) == false)
		return false;
	address = ((uint16_t)curr) << 8;
	if (readHexPair(&curr, sum) == false)
		return false;
	address |= curr;

	if (readHexPair(&recordType, sum) == false)
		return false;

	for (i = 0; i < length; i++)
	{
		if (readHexPair(&curr, sum) == false)
			return false;
		data += curr;
	}
	//if (verifyChecksum(sum))
	//	return false;
	verifyChecksum(sum);

	if (feof(fileIHex) == 0 && ferror(fileIHex) == 0)
	{
		fread(&curr, 1, 1, fileIHex);
		if (curr == '\r')
		{
			curr = (uint8_t)fgetc(fileIHex);
			if (curr != '\n')
				ungetc(curr, fileIHex);
		}
		else if (curr!= '\n')
			return false;
	}

	line.address = address;
	line.recordType = recordType;
	line.length = length;
	line.data = strdup(data.c_str());
	return true;
}

void IntelHex::verifyIntegrity()
{
	uint32_t i;
	for (i = 0; i < lines.size(); i++)
	{
		HexLine &line = lines[i];
		if (line.recordType == 0)
			continue;
		else if (line.recordType == 1)
		{
			if (i == lines.size() - 1 && line.length == 0)
				continue;
		}
		else if (line.recordType == 2)
		{
		}
		else if (line.recordType == 4)
		{
		}
		else if (line.recordType == 3 || line.recordType == 5)
			throw new IntelHexException("Unsupported record types found in file");
		throw new IntelHexException("Hex file corrupt");
	}
}

IntelHex *IntelHex::Open(const char *fileName)
{
	IntelHex *ret;
	FILE *file = fopen(fileName, "rb");
	ret = new IntelHex(file);
	while (!feof(file))
	{
		HexLine line = {0};
		if (ret->readLine(line) == false)
		{
			delete ret;
			throw new IntelHexException("Requested file was not an Intel HEX file");
		}
		ret->lines.push_back(line);
		ret->verifyIntegrity();
	}
	return ret;
}

ROM *IntelHex::GenerateROM()
{
	return NULL;
}

IntelHexException::IntelHexException(const char *errorMsg) : error(errorMsg)
{
}

const char *IntelHexException::what() const
{
	return error;
}
