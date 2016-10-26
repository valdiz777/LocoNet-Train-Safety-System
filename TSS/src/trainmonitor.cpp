#include "trainmonitor.h"
#include "enginepath.h"
#include <QFile>
#include <QDir>
#include <QTextStream>

TrainMonitor::TrainMonitor()
{
#pragma message("[MKJ] Clean up when everything is done")
#pragma message("[MKJ] Look for things that would be useful in a log")

}

TrainMonitor::~TrainMonitor()
{


}

void TrainMonitor::do_run() {
	generateSectionList();
}


/*TrainMonitor TrainMonitor::getInstance()
{
	static TrainMonitor ret;
	return ret;
}*/

// For sure, we will need an engine path and a section to perform the
// safety algorithm. We may need to parse a string to get both of these
// pieces of information or we may be provided it.
void TrainMonitor::addEngineSectionToQueue(EnginePath Path, Section section)
{
#pragma message("[MKJ] Get with Valentine and Richard to determine what we will need to do at this point")
	engineSection es;
	es.path = Path;
	es.sec = section;

	m_engineSectionQueue.enqueue(es);
}

void TrainMonitor::do_handleOccupancy(QString section, bool state)
{
	qDebug() << "New Occupancy Data at Monitor:" << section << " State:" << state;
	Section monitoredSection = retrieveSections(section);
	monitoredSection.setOccupancy(state);
	if (state) Monitor(monitoredSection);
}

void TrainMonitor::Monitor(Section sec)
{
	//while (!m_engineSectionQueue.empty())
	//{
		//engineSection es = m_engineSectionQueue.front();
		//m_engineSectionQueue.dequeue();
		//updateEnginePath(es.path.getEngine());

#pragma message("[MKJ] Will nextSection1 be enough padding for a crash scenario?")
		//Section monitorSection = es.path.getNextSection1();

		//switch (monitorSection.getNumOfConns())
		//{
		//case 1:		// Endpoint case
		//{
			//endpointMonitor(monitorSection);
			//break;
		//}
		//case 2:		// Straight case
		//{
			//straightMonitor(monitorSection, es);
	if (sec.getNumOfConns() == 1) {
		endpointMonitor(sec);
	}
	else {
		straightMonitor(sec);
	}
	//break;
//}
//case 3:		// Switch case
//{
	//switchMonitor(monitorSection, es);
	//break;
//}
//case 4:		// Crossover case
//{
	//crossoverMonitor(monitorSection, es);
	//break;
//}
//default:
//{
	//qFatal("Invalid track type");
	//break;
//}
//}
//}
}

void TrainMonitor::endpointMonitor(Section sec)
{
	// Shutdown the section since the means the train is headed
	// towards the end of the track
	//qDebug() << "SECTION:" << sec.getNode() << "BYE BYE";

	emit trackOff();
}

void TrainMonitor::straightMonitor(Section sec)
{
	Section oneWay = retrieveSections(sec.getConn1());
	Section theOtherWay = retrieveSections(sec.getConn2());
	qDebug() << "SECTION:" << sec.getNode() << " CONN1:" << oneWay.getNode() << ", CONN2:" << theOtherWay.getNode();

	/*if (sec.getConn2() == "4-13") {

		emit trackOff();
	 }*/

	 //Section check1;
	 //Section check2;

	 //if (sec.getConn1() == "1-11" || sec.getConn2() == "1-11") emit trackOff();

	 /*if(oneWay.getConn1() == sec.getNode())
	 {
		 check1 = retrieveSections(oneWay.getConn2());

		 //emit trackOff();
	 }
	 else
	 {
		 check1 = retrieveSections(oneWay.getConn1());
		 //emit trackOff();
	 }

	 if(theOtherWay.getConn1() == sec.getNode())
	 {
		 check2 = retrieveSections(theOtherWay.getConn2());
		// emit trackOff();
	 }
	 else
	 {
		 check2 = retrieveSections(theOtherWay.getConn1());
		 //emit trackOff();
	 }*/

	 //qDebug() << "CHECK1_Occ:" << check1.getOccupancy() << " Check2_OCC:" << check2.getOccupancy();

	 /*if( check1.getOccupancy() == true)
	 {

		 qDebug() << "EMITTING TRACK OFF. SHUT DOWN NOW";
		 emit trackOff();
	 }

	 if( check2.getOccupancy() == true)
	 {

		 qDebug() << "EMITTING TRACK OFF. SHUT DOWN NOW";
		 emit trackOff();
	 }*/

	if (oneWay.getOccupancy() == true || (retrieveSections(oneWay.getConn2()).getOccupancy() == true))
	{

		qDebug() << "EMITTING TRACK OFF. SHUT DOWN NOW";
		emit trackOff();
	}

	if (theOtherWay.getOccupancy() == true || (retrieveSections(theOtherWay.getConn2()).getOccupancy() == true))
	{

		qDebug() << "EMITTING TRACK OFF. SHUT DOWN NOW";
		emit trackOff();
	}


	/*// Determine which track section is the one that must be checked
	// for vacancy
	QString sectionInQuestion;
	if (sec.getConn1() == es.sec.getNode())
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
	if (engineOnSection(sectionInQuestion).getEngine() != "")
	{
		// Check to see if the two trains are heading towards each other
		// in a crash situation
		if (es.path.getDirection() != engineOnSection(
			sectionInQuestion).getDirection())
		{
			// Shutdown the piece of track that would see the crash (sec)
			emit (trackOff());
		}
	}*/
}

void TrainMonitor::switchMonitor(Section sec, engineSection es)
{

}

void TrainMonitor::crossoverMonitor(Section sec, engineSection es)
{

}

Section TrainMonitor::retrieveSections(QString section)
{
	Section newSection;
	//int NotFoundIndex;

	bool found = false;
	for (Section it : m_sectionList)
	{
		if (it.getNode() == section)
		{
			newSection = it;
			found = true;
			break;
		}
	}

	if (!found)
	{
		qWarning("Invalid section:%s. Are you sure the section exists?", section.toLatin1().constData());
	}

	return newSection;
}

void TrainMonitor::generateSectionList()
{
#pragma message("[MKJ] Do we want to move this to a central location?")
	qDebug() << "Testing file input";
	QString filename = QDir::currentPath().append("/Sections.txt");
	QFile infile(filename);
	if (!infile.open(QIODevice::ReadOnly | QIODevice::Text)) {

		qFatal("Cannot find input file: %s", filename.toLatin1().constData());
		return;
	}

	QTextStream in(&infile);
    int id, boardNum, section;
    SwitchInfo::TURNOUT_STATE switchState;
	QString node;
	int numOfConns;
	QString conn1, conn2, conn3, conn4;
	int trackInfoId;

	while (!in.atEnd())
	{

		QStringList splitString = in.readLine().split(",");
		id = splitString[0].toInt();
        boardNum = splitString[1].toInt();
        section = splitString[2].toInt();
        switchState = (splitString[3].toInt() == 1)? SwitchInfo::TURNOUT_STATE::thrown : (splitString[3].toInt() == 2)? SwitchInfo::TURNOUT_STATE::closed:SwitchInfo::TURNOUT_STATE::not_set;
		node = splitString[4];
		numOfConns = splitString[5].toInt();
		conn1 = splitString[6];
		conn2 = splitString[7];
		conn3 = splitString[8];
		conn4 = splitString[9];
		trackInfoId = splitString[10].toInt();

        Section sec = Section(boardNum, section, switchState, node, numOfConns, conn1, conn2,
			conn3, conn4);
		qDebug() << "SECTION:" << sec.getNode() << " CONN1:" << conn1 << ", CONN2:" << conn2;

		m_sectionList.append(sec);
	}
	infile.close();
}

Section TrainMonitor::findNextSection(Section previousSection,
	Section currentSection)
{
	Section nextSection;
	Section temp;

	if (previousSection.getNode() != "" && currentSection.getNode() != "")
	{
		switch (currentSection.getNumOfConns())
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
			if (currentSection.getConn1() == previousSection.getNode())
			{
				nextSection = retrieveSections(
					currentSection.getConn2());
			}
			else
			{
				nextSection = retrieveSections(
					currentSection.getConn1());
			}
			break;
		}
		case 3:
		{
			if (currentSection.getConn1() == previousSection.getNode())
			{
				if (currentSection.getSwitchDirection())	// configured left
				{
					nextSection = retrieveSections(
						currentSection.getConn2());
				}
				else
				{
					nextSection = retrieveSections(
						currentSection.getConn3());
				}
			}
			else
			{
				nextSection = retrieveSections(
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


EnginePath TrainMonitor::engineOnSection(QString section)
{
	EnginePath engine;

	for (EnginePath itr : m_engineList)
	{
		if (itr.getCurrentSection().getNode() == section)
		{
			engine = itr;
			break;
		}
	}

	return engine;
}

bool TrainMonitor::updateEnginePath(QString engine)
{
#pragma message("[MKJ] This is the nominal updateEnginePath. We need to add a reverseUpdateEnginePath for when the direction is reversed")
	EnginePath path;

	bool found = false;
	for (EnginePath itr : m_engineList)
	{
		if (itr.getEngine() == engine)
		{
			itr.setCurrentSection(itr.getNextSection1());
			itr.setNextSection1(itr.getNextSection2());
			itr.setNextSection2(itr.getNextSection3());
			itr.setNextSection3(itr.getNextSection4());
			itr.setNextSection4(findNextSection(itr.getNextSection3(),
				itr.getNextSection4()));

			found = true;
			break;
		}
	}

	if (!found)
	{
		qWarning("Invalid engine. Are you sure the engine exists?");
	}


	return found;
}

void TrainMonitor::toggleSwitchDirectionLeft(QString section)
{
	// When we do this, we must update all engine paths that contain this
	// switch
	Section sec = retrieveSections(section);
	sec.toggleSwitchDirectionLeft();
}

void TrainMonitor::toggleSwitchDirectionRight(QString section)
{
	// When we do this, we must update all engine paths that contain this
	// switch
	Section sec = retrieveSections(section);
	sec.toggleSwitchDirectionRight();
}
