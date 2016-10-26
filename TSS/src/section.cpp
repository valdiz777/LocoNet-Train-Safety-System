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
    m_switchDirection = SwitchInfo::TURNOUT_STATE::not_set;		// true for left; false for right
	m_occupancy = false;
}

Section::Section(int boardNum, int section, SwitchInfo::TURNOUT_STATE switchDirection, QString Node, int NumOfConns,
	QString Conn1, QString Conn2, QString Conn3, QString Conn4)
{
    m_boardNum = boardNum;
    m_section = section;
	m_node = Node;
	m_numOfConns = NumOfConns;
	m_conn1 = Conn1;
	m_conn2 = Conn2;
	m_conn3 = Conn3;
	m_conn4 = Conn4;
    m_switchDirection = switchDirection;
	m_occupancy = false;
}

Section::~Section()
{

}

void Section::setBoardNum( int boardNum )
{
    m_boardNum = boardNum;
}

int Section::getBoardNum()
{
    return m_boardNum;
}


void Section::setSection( int section )
{
    m_section = section;
}

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

SwitchInfo::TURNOUT_STATE Section::getSwitchDirection()
{
	return m_switchDirection;
}

QString Section::getSwitchDirectionString()
{
    return TURNOUT_STATE_STRING[(int)m_switchDirection];
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
    m_switchDirection = SwitchInfo::TURNOUT_STATE::thrown;
     // emit signal to toggle turnout [vcn]
}

void Section::toggleSwitchDirectionRight()
{
#pragma message("[MKJ] Look into updating all engine paths")
	// When we do this, we must update all engine paths that contain this
	// switch
    m_switchDirection = SwitchInfo::TURNOUT_STATE::closed;
    // emit signal to toggle turnout [vcn]
}

void Section::setOccupancy(bool state)
{
	m_occupancy = state;
}

bool Section::getOccupancy()
{
	return m_occupancy;
}

