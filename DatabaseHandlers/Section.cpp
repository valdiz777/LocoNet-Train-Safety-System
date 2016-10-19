#include "Section.h"
#include <list>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

Section::Section()
{
	#pragma message "[MKJ] Clean up when everything is done"
	#pragma message "[MKJ] Look for things that would be useful in a log"
	m_x = -1;
	m_y = -1;
	m_node = "";
	m_numOfConns = -1;
	m_conn1 = "";
	m_conn2 = "";
	m_conn3 = "";
	m_conn4 = "";
	m_switchDirection = true;		// true for left; false for right
}

Section::Section( int X, int Y, string Node, int NumOfConns, 
	string Conn1, string Conn2, string Conn3, string Conn4)
{
	m_x = X;
	m_y = Y;
	//type = Type;
	m_node = Node;
	m_numOfConns = NumOfConns;
	m_conn1 = Conn1;
	m_conn2 = Conn2;
	m_conn3 = Conn3;
	m_conn4 = Conn4;
	m_switchDirection = true;
}

Section::~Section()
{

}

// CRUD for X
/*void Section::setX( int X )
{

}*/

int Section::getX(  )
{
	return m_x;
}

// CRUD for Y
/*void Section::setY( int Y )
{

}*/

int Section::getY(  )
{
	return m_y;
}

// CRUD for Node
/*void Section::setNode( std::string node )
{

}*/

std::string Section::getNode(  )
{
	return m_node;
}

// CRUD for NumOfConns
/*void Section::setNumOfConns( int numConn )
{

}*/

int Section::getNumOfConns(  )
{
	return m_numOfConns;
}

// CRUD for Conn1
/*void Section::setConn1( std::string conn1 )
{

}*/

std::string Section::getConn1(  )
{
	return m_conn1;
}

// CRUD for Conn2
/*void Section::setConn2( std::string conn2 )
{

}*/

std::string Section::getConn2(  )
{
	return m_conn2;
}

// CRUD for Conn3
/*void Section::setConn3( std::string conn3 )
{

}*/

std::string Section::getConn3(  )
{
	return m_conn3;
}

// CRUD for Conn4
/*void Section::setConn4( std::string conn3 )
{

}*/

std::string Section::getConn4(  )
{
	return m_conn4;
}

// CRUD for TrackInfoID
/*void Section::setTrackInfoID( TrackInfo track )
{

}*/

TrackInfo Section::getTrackInfoID	()
{
	TrackInfo info;

	return info;
}

bool Section::getSwitchDirection()
{
	return m_switchDirection;
}


bool Section::ConnectToMySQL()
{
	//driver = sql::mysql::get_mysql_driver_instance();
	//con = driver->connect("http://git.arl.uah.edu:3306", "root", "cstrapwi");

	return false;
}

Section Section::RetrieveSections(string section)
{
	Section newSection;

	bool found = false;
	for (auto it : m_sectionList)
	{
		if(it.m_node == section)
		{
			newSection = it;
			found = true;
			continue;
		}	
	}
  
	if(!found)
	{
		cout << "Invalid section. Are you sure the section exists?\n";
	}
	
	return newSection;
}

void Section::GenerateSectionList()
{
	#pragma message "[MKJ] Do we want to move this to a central location?"
	ifstream infile(
		"/home/student/aac0019/CPE656/SafetyTeam_noMySQL/Sections.txt");

	int id, x, y, trackType;
	string node;
	int numOfConns;
	string conn1, conn2, conn3, conn4;
	int trackInfoId;

	string line;


	while(getline(infile,line))
	{	
		stringstream linestream(line);

		vector<string> splitString = split(line, ',');
		
		id = stoi(splitString[0]);
		x = stoi(splitString[1]);
		y = stoi(splitString[2]);
		trackType = stoi(splitString[3]);
		node = splitString[4];
		numOfConns = stoi(splitString[5]);
		conn1 = splitString[6];
		conn2 = splitString[7];
		conn3 = splitString[8];
		conn4 = splitString[9];
		trackInfoId = stoi(splitString[10]);

		Section sec = Section( x, y, node, numOfConns, conn1, conn2, 
			conn3, conn4 );
		m_sectionList.push_back( sec );
	}
}

void Section::split(const std::string &s, char delim, std::vector<string> &elems) 
{
	stringstream ss;
	ss.str(s);
	string item;
	while (getline(ss, item, delim)) 
	{
		elems.push_back(item);
	}
}


vector<string> Section::split(const string &s, char delim) 
{    
	vector<string> elems;
	split(s, delim, elems);
	return elems;
}

void Section::toggleSwitchDirectionLeft(string section)
{
	#pragma message "[MKJ] Look into updating all engine paths"
	// When we do this, we must update all engine paths that contain this
	// switch 
	Section sec = RetrieveSections(section);
	sec.m_switchDirection = true;
}

void Section::toggleSwitchDirectionRight(string section)
{
	#pragma message "[MKJ] Look into updating all engine paths"
	// When we do this, we must update all engine paths that contain this
	// switch 
	Section sec = RetrieveSections(section);
	sec.m_switchDirection = false;
}

Section Section::findNextSection(Section previousSection, 
	Section currentSection)
{
	Section nextSection;
	Section temp;

	if(previousSection.getNode() != "" && currentSection.getNode() != "")
	{
		switch(currentSection.getNumOfConns())
		{
			case 1:
			{
				// The next section will be a null string
				break;
			}
			case 2:
			case 4:
			{
				// The next section will be the only other choice other
				// than the previous section (straight section) or conn1
				// or conn2 (crossover)
				if(currentSection.getConn1() == previousSection.getNode())
				{
					nextSection = RetrieveSections(
						currentSection.getConn2());
				}
				else
				{
					nextSection = RetrieveSections(
						currentSection.getConn1());
				}
				break;
			}
			case 3:
			{
				if(currentSection.getConn1() == previousSection.getNode())
				{
					if(currentSection.getSwitchDirection())	// configured left
					{
						nextSection = RetrieveSections(
							currentSection.getConn2());
					}
					else
					{
						nextSection = RetrieveSections(
							currentSection.getConn3());
					}
				}
				else
				{
					nextSection = RetrieveSections(
						currentSection.getConn1());
				}
			}
		}
	}
	else		
	{
		// One or both of the track sections are null. Therefore, the next
		// section will be null too
	}
	
	return nextSection;
}


