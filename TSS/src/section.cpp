#include "section.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>

Section::Section()
{
#pragma message("[MKJ] Clean up when everything is done")
#pragma message("[MKJ] Look for things that would be useful in a log")
    m_boardNum = -1;
    m_section = -1;
	m_node = "";
	m_numOfConns = -1;
	m_conn1 = "";
	m_conn2 = "";
	m_conn3 = "";
	m_conn4 = "";
    m_thrown = true;		// true for left; false for right
	m_occupancy = false;
}

Section::Section(int boardNum, int section, QString Node, int NumOfConns,
    QString Conn1, QString Conn2, QString Conn3, QString Conn4,
    bool LTNum)
{
    m_boardNum = boardNum;
    m_section = section;
	//type = Type;
	m_node = Node;
	m_numOfConns = NumOfConns;
	m_conn1 = Conn1;
	m_conn2 = Conn2;
	m_conn3 = Conn3;
	m_conn4 = Conn4;
    m_thrown = true;
	m_occupancy = false;
    m_LTNum = LTNum;
}

Section::~Section()
{

}

// CRUD for boardNum
/*void Section::setBoardNum( int boardNum )
{

}*/

int Section::getBoardNum()
{
    return m_boardNum;
}

// CRUD for section
/*void Section::setSection( int section )
{

}*/

int Section::getSection()
{
    return m_section;
}

// CRUD for Node
/*void Section::setNode( QString node )
{

}*/

QString Section::getNode()
{
	return m_node;
}

// CRUD for NumOfConns
/*void Section::setNumOfConns( int numConn )
{

}*/

int Section::getNumOfConns()
{
	return m_numOfConns;
}

// CRUD for Conn1
/*void Section::setConn1( QString conn1 )
{

}*/

QString Section::getConn1()
{
	return m_conn1;
}

// CRUD for Conn2
/*void Section::setConn2( QString conn2 )
{

}*/

QString Section::getConn2()
{
	return m_conn2;
}

// CRUD for Conn3
/*void Section::setConn3( QString conn3 )
{

}*/

QString Section::getConn3()
{
	return m_conn3;
}

// CRUD for Conn4
/*void Section::setConn4( QString conn3 )
{

}*/

QString Section::getConn4()
{
	return m_conn4;
}

// CRUD for TrackInfoID
/*void Section::setTrackInfoID( TrackInfo track )
{

}*/


bool getSwitchDirection()
{
    return false;
}

bool Section::getThrown()
{
    return m_thrown;
}

bool Section::getThrownLeft()
{
    return m_thrownLeft;
}

void Section::setThrown(bool thrown)
{
    m_thrown = thrown;
}

bool Section::ConnectToMySQL()
{
	//driver = sql::mysql::get_mysql_driver_instance();
	//con = driver->connect("http://git.arl.uah.edu:3306", "root", "cstrapwi");

	return false;
}

void Section::toggleSwitchDirectionLeft()
{
#pragma message("[MKJ] Look into updating all engine paths")
	// When we do this, we must update all engine paths that contain this
	// switch
    m_thrown = true;
}

void Section::toggleSwitchDirectionRight()
{
#pragma message("[MKJ] Look into updating all engine paths")
	// When we do this, we must update all engine paths that contain this
    // switch
    m_thrown = false;
}

void Section::setOccupancy(bool state)
{
	m_occupancy = state;
}

bool Section::getOccupancy()
{
	return m_occupancy;
}

bool Section::getLTNum()
{
    return m_LTNum;
}

