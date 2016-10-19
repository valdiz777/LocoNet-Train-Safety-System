#include <iostream>
#include "Section.h"

using namespace std;

// Tests for X 
/*bool setXTest()
{
	bool ret = false;
	Section sec;
	sec.setX(37);
	int test;// Read the database to determine if the value is correct
	
	if( 37 == test )
		ret = true;
	
	return ret;
}*/

bool getXTest()
{
	bool ret = false;
	// Set the X to a known value
	Section sec;
	sec.GenerateSectionList();
	
	Section test = sec.RetrieveSections( "4-12" );
	
	if( test.getX() == -1 )
	{
		ret = true;
	}
	
	return ret;
}

// Tests for Y 
/*bool setYTest()
{
	bool ret = false;
	Section sec;
	sec.setY(42);
	int test;// Read the database to determine if the value is correct
	
	if( 42 == test )
		ret = true;
	
	return ret;
}*/

bool getYTest()
{
	bool ret = false;
	// Set the Y to a known value
	Section sec;
	sec.GenerateSectionList();
	
	Section test = sec.RetrieveSections( "4-12" );
	
	if( test.getY() == -1 )
	{
		ret = true;
	}
	
	return ret;
}

// Tests for node 
/*bool setNodeTest()
{
	bool ret = false;
	Section sec;
	sec.setNode("nodeTest");
	string test;// Read the database to determine if the value is correct
	
	if( "nodeTest" == test )
		ret = true;
	
	return ret;
}*/

bool getNodeTest()
{
	bool ret = false;
	// Set the node to a known value
	Section sec;
	sec.GenerateSectionList();
	
	Section test = sec.RetrieveSections( "4-12" );
	
	if( test.getNode() == "4-12" )
	{
		ret = true;
	}
	
	return ret;
}

// Tests for numOfConns 
/*bool setNumOfConnsTest()
{
	bool ret = false;
	Section sec;
	sec.setNumOfConns(88);
	int test;// Read the database to determine if the value is correct
	
	if( 88 == test )
		ret = true;
	
	return ret;
}*/

bool getNumOfConnsTest()
{
	bool ret = false;
	// Set the numOfConns to a known value
	Section sec;
	sec.GenerateSectionList();
	
	Section test = sec.RetrieveSections( "4-12" );
	
	if( test.getNumOfConns() == 2 )
	{
		ret = true;
	}
	
	return ret;
}

// Tests for conn1 
/*bool setConn1Test()
{
	bool ret = false;
	Section sec;
	sec.setConn1("conn1Test");
	string test;// Read the database to determine if the value is correct
	
	if( "conn1Test" == test )
		ret = true;
	
	return ret;
}*/

bool getConn1Test()
{
	bool ret = false;
	// Set the conn1 to a known value
	Section sec;
	sec.GenerateSectionList();
	
	Section test = sec.RetrieveSections( "4-12" );
	
	if( test.getConn1() == "5-2" )
	{
		ret = true;
	}
	
	return ret;
}

// Tests for conn2 
/*bool setConn2Test()
{
	bool ret = false;
	Section sec;
	sec.setConn2("conn2Test");
	string test;// Read the database to determine if the value is correct
	
	if( "conn2Test" == test )
		ret = true;
	
	return ret;
}*/

bool getConn2Test()
{
	bool ret = false;
	// Set the conn2 to a known value
	Section sec;
	sec.GenerateSectionList();
	
	Section test = sec.RetrieveSections( "4-12" );
	
	if( test.getConn2() == "3-5" )
	{
		ret = true;
	}
	
	return ret;
}

// Tests for conn3 
/*bool setConn3Test()
{
	bool ret = false;
	Section sec;
	sec.setConn3("conn3Test");
	string test;// Read the database to determine if the value is correct
	
	if( "conn3Test" == test )
		ret = true;
	
	return ret;
}*/

bool getConn3Test()
{
	bool ret = false;
	// Set the conn3 to a known value
	Section sec;
	sec.GenerateSectionList();
	
	Section test = sec.RetrieveSections( "4-12" );
	
	if( test.getConn3() == "" )
	{
		ret = true;
	}
	
	return ret;
}

// Tests for conn4 
/*bool setConn4Test()
{
	bool ret = false;
	Section sec;
	sec.setConn1("conn4Test");
	string test;// Read the database to determine if the value is correct
	
	if( "conn4Test" == test )
		ret = true;
	
	return ret;
}*/

bool getConn4Test()
{
	bool ret = false;
	// Set the conn4 to a known value
	Section sec;
	sec.GenerateSectionList();
	
	Section test = sec.RetrieveSections( "4-12" );
	
	if( test.getConn4() == "" )
	{
		ret = true;
	}
	
	return ret;
}

// Tests for TrackInfoID 
/*bool setTrackInfoIDTest()
{
	bool ret = false;
	TrackInfo info; // set all values to a known value to test
	Section sec;
	sec.setTrackInfoID(info);
	TrackInfo test;// Read the database to determine if the value is correct
	
	if( falseall known values == test.<all known values> )
		ret = true;
	
	return ret;
}*/

bool getTrackInfoIDTest()
{
	bool ret = false;
	// Set all values of the trackInfoID to a known value to test
	Section sec;
	sec.GenerateSectionList();
	
	Section test = sec.RetrieveSections( "4-12" );
	
	/*if( test.getTrackInfoID() == -1 )
	{
		ret = true;
	}*/
	
	return ret;
}

// Tests for RetrieveSections
bool RetrieveSectionsTest()
{
	bool ret = false;
	string section;
	// Set every aspect of this to a known value
	Section sec;
	sec.GenerateSectionList();
	
	Section test = sec.RetrieveSections( "4-12" );
	
	if( test.getX() == -1 && test.getY() == -1 && test.getNode() == "4-12" 
		&& test.getNumOfConns() == 2 && test.getConn1() == "5-2" 
		&& test.getConn2() == "3-5" && test.getConn3() == "" 
		&& test.getConn4() == "" /*&& test.getTrackInfoID() == -1*/ )
	{
		ret = true;
	}
		
	return ret;
}

int main()
{
	int numErrors = 0;

	/*if(!setXTest(  ))
		cout << "setXTest Failed.\n"; ++numErrors;*/

	if(!getXTest(  ))
	{
		cout << "getXTest Failed.\n"; 
		++numErrors;
	}

	/*if(!setYTest(  ))
		cout << "setYTest Failed.\n"; ++numErrors;*/

	if(!getYTest(  ))
	{
		cout << "getYTest Failed.\n"; 
		++numErrors;
	}

	/*if(!setNodeTest(  ))
		cout << "setNodeTest Failed.\n"; ++numErrors;*/

	if(!getNodeTest(  ))
	{
		cout << "getNodeTest Failed.\n"; 
		++numErrors;
	}

	/*if(!setNumOfConnsTest(  ))
		cout << "setNumOfConnsTest Failed.\n"; ++numErrors;*/

	if(!getNumOfConnsTest(  ))
	{
		cout << "getNumOfConnsTest Failed.\n"; 
		++numErrors;
	}

	/*if(!setConn1Test(  ))
		cout << "setConn1Test Failed.\n"; ++numErrors;*/

	if(!getConn1Test(  ))
	{
		cout << "getConn1Test Failed.\n"; 
		++numErrors;
	}

	/*if(!setConn2Test(  ))
		cout << "setConn2Test Failed.\n"; ++numErrors;*/

	if(!getConn2Test(  ))
	{
		cout << "getConn2Test Failed.\n"; 
		++numErrors;
	}

	/*if(!setConn3Test(  ))
		cout << "setConn3Test Failed.\n"; ++numErrors;*/

	if(!getConn3Test(  ))
	{
		cout << "getConn3Test Failed.\n"; 
		++numErrors;
	}

	/*if(!setTrackInfoIDTest(  ))
		cout << "setTrackInfoIDTest Failed.\n"; ++numErrors;*/

	/*if(!getTrackInfoIDTest(  ))
	{
		cout << "getTrackInfoIDTest Failed.\n"; 
		++numErrors;
	}*/

	if(!RetrieveSectionsTest(  ))
	{
		cout << "RetrieveSectionsTest Failed.\n"; 
		++numErrors;
	}

	if(numErrors == 0)
		cout << "\n\nSUCCESS: All tests completed successfully...\n\n";
	else
		cout << "\n\nFAILED: " << numErrors << " tests completed unsuccessfully\n\n";
	return 0;
}
