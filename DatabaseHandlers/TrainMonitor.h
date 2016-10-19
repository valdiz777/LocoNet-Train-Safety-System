#include <iostream>
#include <queue>
#include "Section.h"
#include "EnginePath.h"

#ifndef TRAINMONITOR_H
#define TRAINMONITOR_H

struct engineSection
{
	EnginePath path;
	Section sec;
};

class TrainMonitor
{
	public:
		/*struct engineSection
		{
			EnginePath path;
			Section sec;
		};*/

		
		//static TrainMonitor *getInstance;
		TrainMonitor();
		~TrainMonitor();
		void startMonitor();
		void stopMonitor();
		void Monitor();
		void addEngineSectionToQueue( EnginePath Path, Section section );
	private:
		//TrainMonitor();
		//~TrainMonitor();
		void endpointMonitor(Section sec);
		void straightMonitor(Section sec, engineSection es);
		void switchMonitor(Section sec, engineSection es);
		void crossoverMonitor(Section sec, engineSection es);

		bool running;
		std::queue<engineSection> m_engineSectionQueue;
};
#endif
