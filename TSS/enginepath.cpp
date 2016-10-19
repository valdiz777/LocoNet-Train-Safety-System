#include "enginepath.h"
/* Class: EnginePath
*
* [Insert Description]
* 
* 
*
* [insert developer(s)] [$insert email(s)]
* Train Safety Team
* [Insert additional comments]
*/

// Constructor
EnginePath::EnginePath()
{
#pragma message("[MKJ] Do we want to change the sections to Section types?")
#pragma message("[MKJ] Since we are keeping up with which direction the switches are going, can we reduce the number of nextSections to around 4?")
#pragma message("[MKJ] Clean up when everything is done")
#pragma message("[MKJ] Look for things that would be useful in a log")
	m_engine = "";
	Section m_currentSection;
	Section m_nextSection1;
	Section m_nextSection2;
	Section m_nextSection3;
	Section m_nextSection4;
	m_throttle = -1;
	m_direction = -1;
}

EnginePath::EnginePath(QString Engine, Section CurrentSection,
	Section NextSection1, Section NextSection2, Section NextSection3,
	Section NextSection4, int Throttle, int Direction)
{
	m_engine = Engine;
	m_currentSection = CurrentSection;
	m_nextSection1 = NextSection1;
	m_nextSection2 = NextSection2;
	m_nextSection3 = NextSection3;
	m_nextSection4 = NextSection4;
	m_throttle = Throttle;
	m_direction = Direction;

}

// Destructor....because C++ is a terrible terrible language
EnginePath::~EnginePath()
{

}

// more than likely not needed
/*int EnginePath::getId(  )
{
	return 1;
}*/

void EnginePath::setEngine(QString engine)
{
	m_engine = engine;
}

QString EnginePath::getEngine()
{
	return m_engine;
}

// CRUD for currentSection
void EnginePath::setCurrentSection(Section section)
{
	m_currentSection = section;
}

Section EnginePath::getCurrentSection()
{
	return m_currentSection;
}

// CRUD for nextSection1
void EnginePath::setNextSection1(Section section)
{
	m_nextSection1 = section;
}

Section EnginePath::getNextSection1()
{
	return m_nextSection1;
}

// CRUD for nextSection2
void EnginePath::setNextSection2(Section section)
{
	m_nextSection2 = section;
}

Section EnginePath::getNextSection2()
{
	return m_nextSection2;
}


// CRUD for nextSection3
void EnginePath::setNextSection3(Section section)
{
	m_nextSection3 = section;
}

Section EnginePath::getNextSection3()
{
	return m_nextSection3;
}

// CRUD for nextSection4
void EnginePath::setNextSection4(Section section)
{
	m_nextSection4 = section;
}

Section EnginePath::getNextSection4()
{
	return m_nextSection4;
}

// CRUD for throttle
void EnginePath::setThrottle(int throttle)
{
	m_throttle = throttle;
}

int EnginePath::getThrottle()
{
	return m_throttle;
}

// CRUD for direction
void EnginePath::setDirection(int direction)
{
	m_direction = direction;
}

int EnginePath::getDirection()
{
	return m_direction;
}
