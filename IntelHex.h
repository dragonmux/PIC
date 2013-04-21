/*
 * This file is part of PIC
 * Copyright © 2012 Rachel Mant (dx-mon@users.sourceforge.net)
 *
 * PIC is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PIC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
