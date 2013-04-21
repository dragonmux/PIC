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

#include <libTest.h>
#include "IntelHex.h"

void testCreate()
{
	IntelHex *hexFile;
	try
	{
		hexFile = IntelHex::Open("" /* TODO: build a suitable test .hex file */);
	}
	catch (IntelHexException *e)
	{
		fail(e->what());
		assertTrue(FALSE);
	}
	delete hexFile;
}

BEGIN_REGISTER_TESTS()
	TEST(testCreate)
END_REGISTER_TESTS()
