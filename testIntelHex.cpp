#include <libTest.h>
#include "IntelHex.h"

void testCreate()
{
	IntelHex *hexFile;
	try
	{
		hexFile = IntelHex::Open("LEDBallast.hex");
	}
	catch (IntelHexException *e)
	{
		fprintf(stderr, "%s\n", e->what());
		// fail(e->what());
		assertTrue(FALSE);
	}
	delete hexFile;
}

BEGIN_REGISTER_TESTS()
	TEST(testCreate)
END_REGISTER_TESTS()
