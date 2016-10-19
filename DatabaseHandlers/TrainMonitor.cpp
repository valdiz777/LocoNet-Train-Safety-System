#include "TrainMonitor.h"
#include "EnginePath.h"

TrainMonitor::TrainMonitor()
{
	#pragma message "[MKJ] Clean up when everything is done"
	#pragma message "[MKJ] Look for things that would be useful in a log"
	startMonitor();
}

TrainMonitor::~TrainMonitor()
{
	stopMonitor();
}

void TrainMonitor::startMonitor()
{
	running = true;
	while(running)
	{
		Monitor();
	}
}

void TrainMonitor::stopMonitor()
{
	running = false;
}

/*TrainMonitor TrainMonitor::getInstance()
{
	static TrainMonitor ret;
	return ret;
}*/

// For sure, we will need an engine path and a section to perform the
// safety algorithm. We may need to parse a string to get both of these
// pieces of information or we may be provided it.
void TrainMonitor::addEngineSectionToQueue( EnginePath Path, Section section )
{
	#pragma message "[MKJ] Get with Valentine and Richard to determine what we will need to do at this point"
	engineSection es;
	es.path = Path;
	es.sec = section;
	
	m_engineSectionQueue.push(es);
}

void TrainMonitor::Monitor()
{
	while(!m_engineSectionQueue.empty())
	{
		engineSection es = m_engineSectionQueue.front();
		m_engineSectionQueue.pop();
		es.path.updateEnginePath( es.path.getEngine() );
		
		#pragma message "[MKJ] Will nextSection1 be enough padding for a crash scenario?"
		Section monitorSection = es.path.getNextSection1();
			
		switch(monitorSection.getNumOfConns())
		{
			case 1:		// Endpoint case
			{
				endpointMonitor(monitorSection);
				break;
			}
			case 2:		// Straight case
			{
				straightMonitor(monitorSection, es);
				break;
			}
			case 3:		// Switch case
			{
				switchMonitor(monitorSection, es);
				break;
			}
			case 4:		// Crossover case
			{
				crossoverMonitor(monitorSection, es);
				break;
			}
			default:
			{
				cout << "Invalid track type";
				break;
			}
		}
	}
}

void TrainMonitor::endpointMonitor(Section sec)
{
	// Shutdown the section since the means the train is headed
	// towards the end of the track
}

void TrainMonitor::straightMonitor(Section sec, engineSection es)
{
	// Determine which track section is the one that must be checked
	// for vacancy
	string sectionInQuestion;
	if(sec.getConn1() == es.sec.getNode())
	{
		sectionInQuestion = sec.getConn2();
	}
	else
	{
		sectionInQuestion = sec.getConn1();
	}
	
	// Determine which train is on the section in question. If there is 
	// none, a default EnginePath will be returned.
	EnginePath ep;
	
	// Check to see if there was an engine in the section that we need to 
	// be monitoring for a potential crash.
	if(ep.engineOnSection(sectionInQuestion).getEngine() != "")
	{
		// Check to see if the two trains are heading towards each other
		// in a crash situation
		if(es.path.getDirection() != ep.engineOnSection(
			sectionInQuestion).getDirection())
		{
			// Shutdown the piece of track that would see the crash (sec)
		}
	}
}

void TrainMonitor::switchMonitor(Section sec, engineSection es)
{
	
}

void TrainMonitor::crossoverMonitor(Section sec, engineSection es)
{
	
}

