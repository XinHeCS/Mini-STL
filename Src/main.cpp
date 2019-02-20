#include <iostream>
#include <vector>

#include <Config/Config.h>
#include "Test/TestUtility.h"

extern void testVector();

int main()
{
	// Enable memory usage reporting
	_REPORT_MEM();

	// Unit Test Example
	testVector();

	// Print Unit Test results
	MSTD::TestCounter::getInstance().reportResult();

	return 0;
}