#include "section.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>

Section::Section()
{
#pragma message("[MKJ] Clean up when everything is done")
#pragma message("[MKJ] Look for things that would be useful in a log")
	m_node = "";
	m_numOfConns = -1;
	m_conn1 = "";
	m_conn2 = "";
	m_conn3 = "";
    m_ltnum = -1;
    m_thrown = true;		// true for left; false for right
	m_occupancy = false;
    m_thrownLeft = true;
}

Section::Section(QString Node, int NumOfConns,
    QString Conn1, QString Conn2, QString Conn3, int ltNum, bool thrownLeft)
{
	m_node = Node;
	m_numOfConns = NumOfConns;
	m_conn1 = Conn1;
	m_conn2 = Conn2;
	m_conn3 = Conn3;
    m_thrown = true;
	m_occupancy = false;
    m_ltnum = ltNum;
    m_thrownLeft = thrownLeft;
}

Section::~Section()
{

}

QString Section::getNode()
{
	return m_node;
}

int Section::getNumOfConns()
{
	return m_numOfConns;
}

QString Section::getConn1()
{
	return m_conn1;
}

QString Section::getConn2()
{
	return m_conn2;
}

QString Section::getConn3()
{
	return m_conn3;
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

void Section::setOccupancy(bool state)
{
	m_occupancy = state;
}

bool Section::getOccupancy()
{
	return m_occupancy;
}

int Section::getLtNum()
{
    return m_ltnum;
}

