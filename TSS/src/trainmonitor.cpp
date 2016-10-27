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
    //if (state) Monitor(monitoredSection);
    if (state) Monitor(section);
}

void TrainMonitor::Monitor(QString section)
{
	//while (!m_engineSectionQueue.empty())
	//{
		//engineSection es = m_engineSectionQueue.front();
		//m_engineSectionQueue.dequeue();
		//updateEnginePath(es.path.getEngine());

#pragma message("[MKJ] Will nextSection1 be enough padding for a crash scenario?")
        //Section monitorSection = es.path.getNextSection1();

    Section currentSection = retrieveSections(section);

    switch (currentSection.getNumOfConns())
    {
        case 1:		// Endpoint case
        {
            updateSingleConnList(currentSection);
            endpointMonitor(currentSection);
            break;
        }
        case 2:		// Straight case
        {
            updateDoubleConnList(currentSection);
            straightMonitor(currentSection);
            break;
        }
        case 3:		// Switch case
        {
            updateTripleConnList(currentSection);
            switchMonitor(currentSection);
            break;
        }
        case 4:		// Crossover case
        {
            updateQuadConnList(currentSection);
            //crossoverMonitor(currentSection);
            break;
        }
        default:
        {
            qFatal("Invalid track type");
            break;
        }
    }
}

void TrainMonitor::endpointMonitor(Section sec)
{
	// Shutdown the section since the means the train is headed
    // towards the end of the track
    for (auto section : sectionPairs)
    {
        if (sec.getNode() == section.second)
        {
            if(section.first != "")
            {
                emit trackOff();
            }
        }
    }
}

void TrainMonitor::straightMonitor(Section sec)
{
    QString nextSection;
    bool found = false;

    for (auto section : sectionPairs)
    {
        if(sec.getNode() == section.second)
        {
            nextSection = getNextStraightSection(
                        retrieveSections(section.first), retrieveSections(section.second));

            found = true;
        }
    }


    if(!found)
    {
        //QFatal << "Shit didn't work homie!!!";
    }

    for(auto section : sectionPairs)
    {
        if(nextSection == section.second)
        {
            emit trackOff();
        }
    }
}

void TrainMonitor::switchMonitor(Section sec)
{
    QString monitorSection;
    QString nextSection;
    for (auto section : sectionPairs)
    {
        if(sec.getNode() == section.second)
        {
            nextSection = getNextStraightSection(
                        retrieveSections(section.first), retrieveSections(section.second));
        }

        if(nextSection != "")
        {
            switch (retrieveSections(nextSection).getNumOfConns())
            {
                case 1:
                    emit trackOff();
                    break;
                case 2:
                    monitorSection = getNextStraightSection(
                                retrieveSections(section.first), retrieveSections(nextSection));
                    break;
                case 3:
                    monitorSection = getNextStraightSection(
                                retrieveSections(section.second), retrieveSections(nextSection));
                    break;
                case 4:
                    // TODO
                    break;
            }
        }
    }

    for (auto section : sectionPairs)
    {
        if(section.second == monitorSection)
        {
            emit trackOff();
        }
    }
}

//void TrainMonitor::crossoverMonitor(Section sec, engineSection es)
//{

//}

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
	int id, x, y, trackType;
	QString node;
	int numOfConns;
	QString conn1, conn2, conn3, conn4;
	int trackInfoId;

	while (!in.atEnd())
	{

		QStringList splitString = in.readLine().split(",");
		id = splitString[0].toInt();
		x = splitString[1].toInt();
		y = splitString[2].toInt();
		trackType = splitString[3].toInt();
		node = splitString[4];
		numOfConns = splitString[5].toInt();
		conn1 = splitString[6];
		conn2 = splitString[7];
		conn3 = splitString[8];
		conn4 = splitString[9];
		trackInfoId = splitString[10].toInt();

		Section sec = Section(x, y, node, numOfConns, conn1, conn2,
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
                if (currentSection.getThrownLeft())	// configured left
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

	for (auto it : m_engineList)
	{
		if (it.getCurrentSection().getNode() == section)
		{
			engine = it;
			continue;
		}
	}

	return engine;
}

bool TrainMonitor::updateEnginePath(QString engine)
{
#pragma message("[MKJ] This is the nominal updateEnginePath. We need to add a reverseUpdateEnginePath for when the direction is reversed")
	EnginePath path;

	bool found = false;
	for (auto it : m_engineList)
	{
		if (it.getEngine() == engine)
		{
			it.setCurrentSection(it.getNextSection1());
			it.setNextSection1(it.getNextSection2());
			it.setNextSection2(it.getNextSection3());
			it.setNextSection3(it.getNextSection4());
			it.setNextSection4(findNextSection(it.getNextSection3(),
				it.getNextSection4()));

			found = true;
			continue;
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

/// Updates to an endpoint.....fix comment later
void TrainMonitor::updateSingleConnList(Section current)
{
    bool found = false;
    for(auto sec : sectionPairs)
    {
        if (sec.second == current.getConn1())
        {
            sec.first = sec.second;
            sec.second = current.getNode();
            found = true;
        }
    }

    if(!found)
    {
        /// TODO
        std::pair<QString, QString> secPair=
                new std::pair<QString, QString>("", current.getNode());
        sectionPairs.push_back(secPair);
    }
}

/// Updates strait aways.....
void TrainMonitor::updateDoubleConnList(Section current)
{
    bool found = false;
    for(auto sec : sectionPairs)
    {
        if (sec.second == current.getConn1() ||
                sec.second == current.getConn2())
        {
            sec.first = sec.second;
            sec.second = current.getNode();
            found = true;
        }
    }

    if(!found)
    {
        /// TODO
        std::pair<QString, QString> secPair=
                new std::pair<QString, QString>("", current.getNode());
        sectionPairs.push_back(secPair);
    }
}

/// Updates switches.....
void TrainMonitor::updateTripleConnList(Section current)
{
    bool found = false;
    for(auto sec : sectionPairs)
    {
        if (sec.second == current.getConn1() ||
                sec.second == current.getConn2() ||
                sec.second == current.getConn3())
        {
            sec.first = sec.second;
            sec.second = current.getNode();
            found = true;
        }
    }

    if(!found)
        if(!found)
        {
            /// TODO
            std::pair<QString, QString> secPair=
                    new std::pair<QString, QString>("", current.getNode());
            sectionPairs.push_back(secPair);
        }
}

/// Updates Crossovers....
void TrainMonitor::updateQuadConnList(Section current)
{
    // TODO
}

QString TrainMonitor::getNextStraightSection(Section previous, Section current)
{
    QString nextSection;
    if(previous.getNode() == current.getConn1())
    {
        nextSection = current.getConn2();
    }
    else
    {
        nextSection = current.getConn1();
    }

    return nextSection;
}

QString TrainMonitor::getNextSwitchSection(Section previous, Section current)
{
    QString nextSection;

    if(previous.getNode() == current.getConn2() ||
            previous.getNode() == current.getConn3())
    {
        nextSection = current.getConn1();
    }
    else
    {
        if(!current.getThrownLeft())
        {
            if(current.getThrown())
            {
                nextSection = current.getConn3();
            }
            else
            {
                nextSection = current.getConn2();
            }
        }
        else
        {
            if(current.getThrown())
            {
                nextSection = current.getConn2();
            }
            else
            {
                nextSection = current.getConn3();
            }
        }
    }

    return nextSection;
}




