#include <iostream>
#include <list>
#include "Section.h"

using namespace std;

#ifndef ENGINEPATH_H
#define ENGINEPATH_H

class EnginePath
{
	public:
		EnginePath(  );
		EnginePath( string Engine, Section CurrentSection, 
			Section NextSection1, Section NextSection2, 
			Section NextSection3, Section NextSection4, int Throttle, 
			int Direction );
		~EnginePath(  );

		//int getId(  );

		void setEngine( string engine );
		string getEngine(  );

		void setCurrentSection( Section section );
		Section getCurrentSection(  );

		void setNextSection1( Section section );
		Section getNextSection1(  );

		void setNextSection2( Section section );
		Section getNextSection2(  );

		void setNextSection3( Section section );
		Section getNextSection3(  );

		void setNextSection4( Section section );
		Section getNextSection4(  );

		// void setNextSection5( std::string section );
		// std::string getNextSection5(  );

		// void setNextSection6( std::string section );
		// std::string getNextSection6(  );

		// void setNextSection7( std::string section );
		// std::string getNextSection7(  );

		// void setNextSection8( std::string section );
		// std::string getNextSection8(  );

		// void setNextSection9( std::string section );
		// std::string getNextSection9(  );

		// void setNextSection10( std::string section );
		// std::string getNextSection10(  );

		void setThrottle( int throttle );
		int getThrottle(  );

		void setDirection( int direction );
		int getDirection(  );

		//EnginePath RetrieveEnginePath(string engine);
		bool updateEnginePath( string engine );
		//bool CreateEnginePath( string engine );

		//bool ConnectToMySQL();
		
		EnginePath engineOnSection(string section);

	private:
		string m_engine;
		Section m_currentSection;
		Section m_nextSection1;
		Section m_nextSection2;
		Section m_nextSection3;
		Section m_nextSection4;
		//string m_nextSection5;
		//string m_nextSection6;
		//string m_nextSection7;
		//string m_nextSection8;
		//string m_nextSection9;
		//string m_nextSection10;
		int m_throttle;
		int m_direction;
		
		list<EnginePath> m_engineList;


};
#endif
