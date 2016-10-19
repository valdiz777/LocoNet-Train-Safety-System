#include <iostream>
#include "EnginePath.h"

using namespace std;

// Tests for Engine 
bool engineTest()
{
	bool ret = false;
	EnginePath path;
	path.setEngine("testEngine");
	
	if(path.getEngine() == "testEngine")
		ret = true;
	
	return ret;
}

/*bool getEngineTest()
{
	bool ret = false;
	EnginePath path;
	// Set the engine to a known value
	string test = path.getEngine(  );
	
	if( falseknown value == test )
		ret = true;
	
	return ret;
}*/

// Tests for startSection 
bool startSectionTest()
{
	bool ret = false;
	EnginePath path;
	path.setStartSection("startTest");
	
	if( path.getStartSection() == "startTest" )
		ret = true;
	
	return ret;
}

/*bool getStartSectionTest()
{
	bool ret = false;
	// Set the startSection to a known valuei
	EnginePath path;
	string test = path.getStartSection(  );
	
	if( falseknown value == test )
		ret = true;
	
	return ret;
}*/

// Tests for nextSection1 
bool nextSection1Test()
{
	bool ret = false;
	EnginePath path;
	path.setNextSection1("next1Test");
	
	if( path.getNextSection1() == "next1Test" )
		ret = true;
	
	return ret;
}

/*bool getNextSection1Test()
{
	bool ret = false;
	// Set the nextSection1 to a known value
	EnginePath path;
	string test = path.getNextSection1(  );
	
	if( falseknown value == test )
		ret = true;
	
	return ret;
}*/

// Tests for nextSection2 
bool nextSection2Test()
{
	bool ret = false;
	EnginePath path;
	path.setNextSection2("next2Test");
	
	if( path.getNextSection2() == "next2Test" )
		ret = true;
	
	return ret;
}

/*bool getNextSection2Test()
{
	bool ret = false;
	// Set the nextSection2 to a known value
	EnginePath path;
	string test = path.getNextSection2(  );
	
	if( falseknown value == test )
		ret = true;
	
	return ret;
}*/

// Tests for nextSection3 
bool nextSection3Test()
{
	bool ret = false;
	EnginePath path;
	path.setNextSection3("next3Test");
	
	if( path.getNextSection3() == "next3Test" )
		ret = true;
	
	return ret;
}

/*bool getNextSection3Test()
{
	bool ret = false;
	// Set the nextSection1 to a known value
	EnginePath path;
	string test = path.getNextSection3(  );
	
	if( falseknown value == test )
		ret = true;
	
	return ret;
}*/

// Tests for nextSection4 
bool nextSection4Test()
{
	bool ret = false;
	EnginePath path;
	path.setNextSection4("next4Test");
	
	if( path.getNextSection4() == "next4Test" )
		ret = true;
	
	return ret;
}

/*bool getNextSection4Test()
{
	bool ret = false;
	// Set the nextSection4 to a known value
	EnginePath path;
	string test = path.getNextSection4(  );
	
	if( falseknown value == test )
		ret = true;
	
	return ret;
}*/

// Tests for nextSection5 
bool nextSection5Test()
{
	bool ret = false;
	EnginePath path;
	path.setNextSection5("next5Test");
	
	if( path.getNextSection5() == "next5Test" )
		ret = true;
	
	return ret;
}

/*bool getNextSection5Test()
{
	bool ret = false;
	// Set the nextSection5 to a known value
	EnginePath path;
	string test = path.getNextSection5(  );
	
	if( falseknown value == test )
		ret = true;
	
	return ret;
}*/

// Tests for nextSection6 
bool nextSection6Test()
{
	bool ret = false;
	EnginePath path;
	path.setNextSection6("next6Test");
	
	if( path.getNextSection6() == "next6Test" )
		ret = true;
	
	return ret;
}

/*bool getNextSection6Test()
{
	bool ret = false;
	// Set the nextSection6 to a known value
	EnginePath path;
	string test = path.getNextSection6(  );
	
	if( falseknown value == test )
		ret = true;
	
	return ret;
}*/

// Tests for nextSection7
bool nextSection7Test()
{
	bool ret = false;
	EnginePath path;
	path.setNextSection7("next7Test");
	
	if( path.getNextSection7() == "next7Test" )
		ret = true;
	
	return ret;
}

/*bool getNextSection7Test()
{
	bool ret = false;
	// Set the nextSection7 to a known value
	EnginePath path;
	string test = path.getNextSection7(  );
	
	if( falseknown value == test )
		ret = true;
	
	return ret;
}*/

// Tests for nextSection8 
bool nextSection8Test()
{
	bool ret = false;
	EnginePath path;
	path.setNextSection8("next8Test");
	
	if( path.getNextSection8() == "next8Test" )
		ret = true;
	
	return ret;
}

/*bool getNextSection8Test()
{
	bool ret = false;
	// Set the nextSection8 to a known value
	EnginePath path;
	string test = path.getNextSection8(  );
	
	if( falseknown value == test )
		ret = true;
	
	return ret;
}*/

// Tests for nextSection9 
bool nextSection9Test()
{
	bool ret = false;
	EnginePath path;
	path.setNextSection9("next9Test");
	
	if( path.getNextSection9() == "next9Test" )
		ret = true;
	
	return ret;
}

/*bool getNextSection9Test()
{
	bool ret = false;
	// Set the nextSection9 to a known value
	EnginePath path;
	string test = path.getNextSection9(  );
	
	if( falseknown value == test )
		ret = true;
	
	return ret;
}*/

// Tests for nextSection10
bool nextSection10Test()
{
	bool ret = false;
	EnginePath path;
	path.setNextSection10("next10Test");
	
	if( path.getNextSection10() == "next10Test" )
		ret = true;
	
	return ret;
}

/*bool getNextSection10Test()
{
	bool ret = false;
	// Set the nextSection10 to a known value
	EnginePath path;
	string test = path.getNextSection10(  );
	
	if( falseknown value == test )
		ret = true;
	
	return ret;
}*/

// Tests for throttle
bool throttleTest()
{
	bool ret = false;
	EnginePath path;
	path.setThrottle(65);
	
	if( path.getThrottle() == 65 )
		ret = true;
	
	return ret;
}

/*bool getThrottleTest()
{
	bool ret = false;
	// Set the throttle to a known value
	EnginePath path;
	int test = path.getThrottle(  );
	
	if( falseknown value == test )
		ret = true;
	
	return ret;
}*/

// Tests for direction
bool directionTest()
{
	bool ret = false;
	EnginePath path;
	path.setDirection(32);
	
	if( path.getDirection() == 32 )
		ret = true;
	
	return ret;
}

/*bool getDirectionTest()
{
	bool ret = false;
	// Set the direction to a known value
	EnginePath path;
	int test = path.getDirection(  );
	
	if( falseknown value == test )
		ret = true;
	
	return ret;
}*/

// Tests for RetrieveEnginePath
/*bool RetrieveEnginePathTest()
{
	bool ret = false;
	string engine;
	// Set every aspect of this to a known value
	EnginePath path = EnginePath( "test", "0", "1", "2", "3", "4", "5", 
		"6", "7", "8", "9", "10", 32, 88 );

	EnginePath test = path.RetrieveEnginePath( "test" );
	
	if( falseevery aspect == test.(<every aspect> )
		ret = true;
	
	return ret;
}*/

// Tests for updateEnginePath
// This test would take a lot of time and effort to test.
/*bool updateEnginePathTest()
{
	bool ret = false;
	string engine;
	
	// Set up a known situation so it's possible to test it
	EnginePath path;	
	if( path.updateEnginePath( engine ) )
	{
		if( falseevery aspect == test.(<every aspect> )
			ret = true;
	}
	else
	{
		cout << "Update method failed... Unable to test\n";
	}
		
	return ret;
}*/


// Tests for insertEnginePath
/*bool insertEnginePathTest()
{
	bool ret = false;
	string engine;
	
	EnginePath path;
	if( path.insertEnginePath( engine ) )
	{
		if( falseevery aspect == test.(<every aspect> )
			ret = true;

	}
	else
	{
		cout << "Insert method failed... Unable to test\n";
	}
	
	
	return ret;
}*/

int main()
{
	int numErrors = 0;
		
	/*if(!getIdTest(  ))
	{
		cout << "getIDTest Failed.\n"; 
		++numErrors;
	}*/

	if(!engineTest(  ))
	{
		cout << "getEngineTest Failed.\n"; 
		++numErrors;
	}
		
	if(!startSectionTest(  ))
	{
		cout << "setStartSectionTest Failed.\n"; 
		++numErrors;
	}

	/*if(!getStartSectionTest(  ))
		cout << "getStartSectionTest Failed.\n"; ++numErrors;*/

	if(!nextSection1Test(  ))
	{
		cout << "setNextSectionTest1 Failed.\n"; 
		++numErrors;
	}

	/*if(!getNextSection1Test(  ))
		cout << "getNextSectionTest1 Failed.\n"; ++numErrors;*/

	if(!nextSection2Test(  ))
	{
		cout << "setNextSectionTest2 Failed.\n"; 
		++numErrors;
	}

	/*if(!getNextSection2Test(  ))
		cout << "getNextSectionTest2 Failed.\n"; ++numErrors;*/

	if(!nextSection3Test(  ))
	{
		cout << "setNextSectionTest3 Failed.\n"; 
		++numErrors;
	}

	/*if(!getNextSection3Test(  ))
		cout << "getNextSectionTest3 Failed.\n"; ++numErrors;*/

	if(!nextSection4Test(  ))
	{
		cout << "setNextSectionTest4 Failed.\n"; 
		++numErrors;
	}

	/*if(!getNextSection4Test(  ))
		cout << "getNextSectionTest4 Failed.\n"; ++numErrors;*/

	if(!nextSection5Test(  ))
	{
		cout << "setNextSectionTest5 Failed.\n"; 
		++numErrors;
	}

	/*if(!getNextSection5Test(  ))
		cout << "getNextSectionTest5 Failed.\n"; ++numErrors;*/

	if(!nextSection6Test(  ))
	{
		cout << "setNextSectionTest6 Failed.\n"; 
		++numErrors;
	}

	/*if(!getNextSection6Test(  ))
		cout << "getNextSectionTest6 Failed.\n"; ++numErrors;*/

	if(!nextSection7Test(  ))
	{
		cout << "setNextSectionTest7 Failed.\n"; 
		++numErrors;
	}

	/*if(!getNextSection7Test(  ))
		cout << "getNextSectionTest7 Failed.\n"; ++numErrors;*/

	if(!nextSection8Test(  ))
	{
		cout << "setNextSectionTest8 Failed.\n"; 
		++numErrors;
	}

	/*if(!getNextSection8Test(  ))
		cout << "getNextSectionTest8 Failed.\n"; ++numErrors;*/

	if(!nextSection9Test(  ))
	{
		cout << "setNextSectionTest9 Failed.\n"; 
		++numErrors;
	}

	/*if(!getNextSection9Test(  ))
		cout << "getNextSectionTest9 Failed.\n"; ++numErrors;*/

	if(!nextSection10Test(  ))
	{
		cout << "setNextSectionTest10 Failed.\n"; 
		++numErrors;
	}

	/*if(!getNextSection10Test(  ))
		cout << "getNextSectionTest10 Failed.\n"; ++numErrors;*/

	if(!throttleTest(  ))
	{
		cout << "setThrottleTest Failed.\n"; 
		++numErrors;
	}

	/*if(!getThrottleTest(  ))
		cout << "getThrottleTest Failed.\n"; ++numErrors;*/

	if(!directionTest(  ))
	{
		cout << "setDirectionTest Failed.\n"; 
		++numErrors;
	}

	/*if(!getDirectionTest(  ))
		cout << "getDirectionTest Failed.\n"; ++numErrors;*/

	/*if(!RetrieveEnginePathTest(  ))
		cout << "RetrieveEnginePathTest Failed.\n"; ++numErrors;*/

	/*if(!updateEnginePathTest(  ))
		cout << "updateEnginePathTest Failed.\n"; ++numErrors;*/

	/*if(!insertEnginePathTest(  ))
		cout << "insertEnginePathTest Failed.\n"; ++numErrors;*/

	if(numErrors == 0)
		cout << "\n\nSUCCESS: All tests completed successfully...\n\n";
	else
		cout << "\n\nFAILED: " << numErrors << " tests completed unsuccessfully\n\n";

	return 0;
}
