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

#ifndef __MEMORY_H__
#define __MEMORY_H__

class MemoryLocation
{
protected:
	MemoryLocation();
	virtual ~MemoryLocation();

public:
	virtual operator uint8_t *() const = 0;
};

class MemoryLocationUnmapped : public MemoryLocation
{
private:
	uint8_t *valuePtr;

public:
	MemoryLocationUnmapped(uint8_t *address) : MemoryLocation()
	{
		valuePtr = address;
	}

	~MemoryLocationUnmapped()
	{
		valuePtr = NULL;
	}

	operator uint8_t *() const
	{
		return valuePtr;
	}
};

class MemoryLocationMapped : public MemoryLocation
{
private:
	Register<> *valuePtr;

public:
	MemoryLocationMapped(Register<> *address) : MemoryLocation()
	{
		valuePtr = address;
	}

	~MemoryLocationMapped()
	{
		valuePtr = NULL;
	}

	operator uint8_t *() const
	{
		return *valuePtr;
	}
};

#endif /*__MEMORY_H__*/
