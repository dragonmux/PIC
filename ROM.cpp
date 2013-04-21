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
