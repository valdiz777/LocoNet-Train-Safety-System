#include <iostream>
#include "TrackInfo.h"
#include <list>
#include <vector>

using namespace std;

#ifndef SECTION_H
#define SECTION_H

// namespace TrackType
// {
// 	enum TrackType
// 	{
// 		DEFAULT,
// 		ENDPOINT,
// 		STRAIGHT,
// 		SWITCH,
// 		CROSSOVER
// 	};
// }

class Section
{

	public:
		Section();
		Section(int x, int y, string node, int numOfConns, string conn1,
			string conn2, string conn3, string conn4);
		~Section();

		//void setX( int X );
		int getX(  );

		//void setY( int Y );
		int getY(  );

		//void setNode( std::string node );
		std::string getNode(  );

		//void setNumOfConns( int numConn );
		int getNumOfConns(  );

		//void setConn1( std::string conn1 );
		std::string getConn1(  );

		//void setConn2( std::string conn1 );
		std::string getConn2(  );

		//void setConn3( std::string conn1 );
		std::string getConn3(  );
		
		//void setConn4( std::string conn1 );
		std::string getConn4(  );

		//void setTrackInfoID( TrackInfo track );
		TrackInfo getTrackInfoID(  );
		
		bool getSwitchDirection();

		bool ConnectToMySQL();

		Section RetrieveSections(string section);

		void GenerateSectionList();

		void split(const string &s, char delim, vector<string> &elems);

		vector<string> split(const string &s, char delim);
		
		void toggleSwitchDirectionLeft(string section);
		void toggleSwitchDirectionRight(string section);
		Section findNextSection(Section previousSection, 
			Section currentSection);


	private:
			int m_x;
			int m_y;
			//TrackType::TrackType type;
			string m_node;
			int m_numOfConns;
			string m_conn1;
			string m_conn2;
			string m_conn3;
			string m_conn4;
			bool m_switchDirection;		// true for left; false for right

			list<Section> m_sectionList;
};

#endif
