#include <iostream>
#include "TrackInfo.h"
#include "Section.h"
#include "EnginePath.h"
#include "Initialization.h"
#include "TrainMonitor.h"

using namespace std;

int main()
{
	// Start off the main executable by loading in the pre-
	// determined track configuration from a text file. 
	Section section;
	section.GenerateSectionList();	
	
	// Initialize the train track in order to know the initial
	// state of every section (powered on) and of every switch
	// (TBD ==> left or right). 
	Initialization init;
	init.InitSections();
	init.InitSwitches();
	
	// Start the TrainMonitor
	TrainMonitor mon;
		
	//TrackInfo info;
	//EnginePath path;

	//section = section.RetrieveSections("2-9");
	//cout << "Test connection: " << section.getConn2() << endl;

	return 0;
}
