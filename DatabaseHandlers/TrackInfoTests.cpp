#include <iostream>
#include "TrackInfo.h"

// Tests for status 
bool setStatusTest()
{
	bool ret = false;
	TrackInfo info;
	info.setStatus(true);
	bool test;// Read the database to determine if the value is correct
	
	if( false/*true == test*/ )
		ret = true;
	
	return ret;
}

bool getStatusTest()
{
	bool ret = false;
	// Set the status to a known value
	TrackInfo info;
	int test = info.getStatus(  );
	
	if( false/*known value == test*/ )
		ret = true;
	
	return ret;
}

// Tests for position 
bool setPositionTest()
{
	bool ret = false;
	TrackInfo info;
	info.setPosition("positionTest");
	std::string test;// Read the database to determine if the value is correct
	
	if( "positionTest" == test )
		ret = true;
	
	return ret;
}

bool getPositionTest()
{
	bool ret = false;
	// Set the status to a known value
	TrackInfo info;
	std::string test = info.getPosition(  );
	
	if( false/*known value == test*/ )
		ret = true;
	
	return ret;
}

int main()
{
	int numErrors = 0;

	if(!setStatusTest(  ))
		std::cout << "setStatusTest Failed.\n"; ++numErrors;

	if(!getStatusTest(  ))
		std::cout << "getStatusTest Failed.\n"; ++numErrors;

	if(!setPositionTest(  ))
		std::cout << "setPositionTest Failed.\n"; ++numErrors;

	if(!getPositionTest(  ))
		std::cout << "getPositionTest Failed.\n"; ++numErrors;

	
	if(numErrors == 0)
		std::cout << "\n\nSUCCESS: All tests completed successfully...\n\n";
	else
		std::cout << "\n\nFAILED: " << numErrors << " tests completed unsuccessfully\n\n";

	return 0;
}
