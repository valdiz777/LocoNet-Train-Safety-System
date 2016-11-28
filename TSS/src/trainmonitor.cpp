//////////////////////////////////////////////////////////////
/// \brief TrainMonitor.cpp
/// Description: This class performs operations associated
///     with monitoring the train track. When receiving
///     occupancy data, this class will determine if there
///     will be a crash scenario or not. If one is found, it
///     is reported. Otherwise, the software will sit idle
///     until more occupancy data is received.
//////////////////////////////////////////////////////////////

#include "trainmonitor.h"
#include <QFile>
#include <QTextStream>
#include <QThread>

//////////////////////////////////////////////////////////////
/// \brief TrainMonitor::TrainMonitor
/// Description: Constructor
//////////////////////////////////////////////////////////////
TrainMonitor::TrainMonitor()
{

}

//////////////////////////////////////////////////////////////
/// \brief TrainMonitor::TrainMonitor
/// Description: Destructor
//////////////////////////////////////////////////////////////
TrainMonitor::~TrainMonitor()
{


}

//////////////////////////////////////////////////////////////
/// \brief TrainMonitor::do_run
/// Description: Interface to the generateSectionList. Will
///     be called during initialization to set up the list
///     Sections based on Section.txt
//////////////////////////////////////////////////////////////
void TrainMonitor::do_run() {
	qDebug() << "do_run()";
    generateSectionList();
}

//////////////////////////////////////////////////////////////
/// \brief TrainMonitor::do_handleOccupancy
/// \param section
/// \param state
/// Description: Interface to the Monitor method. The Monitor
///     function only needs to operate on occupancy data with
///     a state of true (a train entering the section) and
///     duplicate information must be ignored to prevent the
///     pairs from getting messed up
//////////////////////////////////////////////////////////////
void TrainMonitor::do_handleOccupancy(QString section, bool state)
{
	qDebug() << "do_handleOccupancy entered";
	qDebug() << "New Occupancy Data at Monitor:" << section << " State:" << state;
    QList<Section> monitoredSections = retrieveSections(section);

    for(auto it : monitoredSections)
    {
        it.setOccupancy(state);
    }

    int found = false;

    for(auto pair:sectionPairs)
    {
        if(pair.second == section)
        {
            found = true;
        }
    }

    if (state)
    {
        if(!found)
        {
            Monitor(section);
        }
        else
        {
            qDebug() << "Prevented a problem with occupancy data at section " << section << " from interferring with the algorithm.";
        }
    }
}

//////////////////////////////////////////////////////////////
/// \brief TrainMonitor::Monitor
/// \param section
/// Description: Main function of the TrainMonitor class.
///     This class will attempt to update the pairs of
///     sections that are used to represent the previous and
///     current sections of a train (one to one correlation
///     between pairs and trains on the track). If the pairs
///     cannot be updated, a collision event is reported.
///     If no collision event is reported, the section goes
///     through a short section collision check
//////////////////////////////////////////////////////////////
void TrainMonitor::Monitor(QString section)
{
	qDebug() << "Monitor()";

    int totalConnections = 0;
    QList<Section> currentSections = retrieveSections(section);

    for(auto currentSection : currentSections)
    {
        switch (currentSection.getNumOfConns())
        {
            case 1:		// Endpoint case
            {
                totalConnections += updateSingleConnList(currentSection);
                break;
            }
            case 2:		// Straight case
            {
                totalConnections += updateDoubleConnList(currentSection);
                break;
            }
            case 3:		// Switch case
            {
                totalConnections += updateTripleConnList(currentSection);
                break;
            }
            case 4:		// Crossover case
            {
                totalConnections += updateQuadConnList(currentSection);
                break;
            }
            default:
            {
                qFatal("Invalid track type");
                break;
            }
        }
    }

    if(totalConnections > 1)
    {
        for(auto currentSection : currentSections)
        {
            switch (currentSection.getNumOfConns())
            {
                case 1:		// Endpoint case
                {
                    qDebug() << "Section "
                             << currentSection.getNode()
                             << " needs to be shutdown (endpoint)";
                    emit collisionEvt(QStringList()
                             << "Endpoint Section Collision Detected" << currentSection.getNode());

                    int i = 0;
                    for(auto section:sectionPairs)
                    {
                        if(section.second == currentSection.getNode())
                        {
                            sectionPairs.removeAt(i);
                        }
                        else
                        {
                            ++i;
                        }
                    }

                    break;
                }
                case 2:		// Straight case
                {
                    qDebug() << "Section "
                             << currentSection.getNode()
                             << " needs to be shutdown (straight)";
                    qDebug() << "Section "
                             << currentSection.getConn1()
                             << " needs to be shutdown (straight)";
                    qDebug() << "Section "
                             << currentSection.getConn2()
                             << " needs to be shutdown (straight)";
                    emit collisionEvt(QStringList()
                             << "Straight Section Collision Detected"
                             << currentSection.getNode()
                             << currentSection.getConn1()
                             << currentSection.getConn2());

                    int i = 0;
                    for(auto section:sectionPairs)
                    {
                        if(section.second == currentSection.getNode() ||
                                section.second == currentSection.getConn1() ||
                                section.second == currentSection.getConn2())
                        {
                            sectionPairs.removeAt(i);
                        }
                        else
                        {
                            ++i;
                        }
                    }

                    break;
                }
                case 3:		// Switch case
                {
                    qDebug() << "Section "
                             << currentSection.getNode()
                             << " needs to be shutdown (switch)";
                    qDebug() << "Section "
                             << currentSection.getConn1()
                             << " needs to be shutdown (switch)";
                    qDebug() << "Section "
                             << currentSection.getConn2()
                             << " needs to be shutdown (switch)";
                    qDebug() << "Section "
                             << currentSection.getConn3()
                             << " needs to be shutdown (switch)";
                    emit collisionEvt(QStringList()
                             << "Switch Section Collision Detected"
                             << currentSection.getNode()
                             << currentSection.getConn1()
                             << currentSection.getConn2()
                             << currentSection.getConn3());

                    int i = 0;
                    for(auto section:sectionPairs)
                    {
                        if(section.second == currentSection.getNode() ||
                                section.second == currentSection.getConn1() ||
                                section.second == currentSection.getConn2() ||
                                section.second == currentSection.getConn3())
                        {
                            sectionPairs.removeAt(i);
                        }
                        else
                        {
                            ++i;
                        }
                    }

                    break;
                }
                case 4:		// Crossover case
                {
                    QStringList shutdownList;
                    shutdownList.append("Crossover Section Collision Detected");

                    if(!currentSection.getConn1().isEmpty())
                    {
                        shutdownList.append(currentSection.getConn1());
                    }

                    if(!currentSection.getConn2().isEmpty())
                    {
                        shutdownList.append(currentSection.getConn2());
                    }

                    if(!currentSection.getConn3().isEmpty())
                    {
                        shutdownList.append(currentSection.getConn3());
                    }

                    if(!currentSection.getConn4().isEmpty())
                    {
                        shutdownList.append(currentSection.getConn4());
                    }

                    for(auto sd : shutdownList)
                    {
                        qDebug() << "Section " << sd << " needs to be shutdown (crossover)";
                    }

                    emit collisionEvt(shutdownList);

                    int i = 0;
                    for(auto section:sectionPairs)
                    {
                        if(section.second == currentSection.getNode() ||
                                section.second == currentSection.getConn1() ||
                                section.second == currentSection.getConn2() ||
                                section.second == currentSection.getConn3() ||
                                section.second == currentSection.getConn4())
                        {
                            sectionPairs.removeAt(i);
                        }
                        else
                        {
                            ++i;
                        }
                    }


                    break;
                }
                default:
                {
                    qFatal("Invalid track type");
                    break;
                }
            }
        }

        // Since there is no way to update the list of pairs when a collision occurs,
        // the only thing that we can do is to inform the user of the collision,
        // remove the pair within the list of pairs, and rely on the user to move
        // the trains to a safe location (trains must have at least one section
        // them at startup and during this situation).
        int i = 0;
        for (auto section : sectionPairs)
        {
            qDebug() << "last section is: " << section.first << " current section is:" << section.second << endl;
            if(section.second == m_lastSectionCurrent)
            {
                sectionPairs.removeAt(i);
            }

            ++i;
        }
    }
    else
    {
        qDebug() << "Updating/creating pair " << section;

        int i = 0;
        if(m_lastSectionCurrent != "NA")
        {
            for (auto section : sectionPairs)
            {
                if(section.second == m_lastSectionCurrent)
                {
                    sectionPairs.removeAt(i);
                }
                ++i;
            }
        }

        sectionPairs.push_back(m_nextPair);

        qDebug() << endl << endl << "======= List of Current Pairs =======";

        for (auto section : sectionPairs)
        {
            qDebug() << "last section is: " << section.first << " current section is:" << section.second << endl;
        }

        // The section that was received through occupancy data has been successfully updated.
        // The remaining task is to make sure, if the next section is a short track, that
        // there won't be a collision event.
        QString nextSection = getNextSection(currentSections,section);
        QList<Section> pss = retrieveSections(nextSection);

        // Now that the nextSection has been determined, we need to see if it's a short section
        for(auto ss : pss)
        {
            if(ss.getNumOfConns() < 4 && ss.getNumOfConns() > 0 && ss.getShortSection())
            {
                QString potentialCollision = getNextSectionWithoutPairs(section,nextSection);
                for(auto pair : sectionPairs)
                {
                    if (pair.second == potentialCollision)
                    {
                        // Shut down which ever ones we need to shut down in this case
                        qDebug() << "Section "
                                 << section
                                 << " needs to be shutdown (short section)";
                        qDebug() << "Section "
                                 << nextSection
                                 << " needs to be shutdown (short section)";
                        qDebug() << "Section "
                                 << potentialCollision
                                 << " needs to be shutdown (short section)";
                        emit collisionEvt(QStringList()
                                 << "Short Section Collision Detected"
                                 << section
                                 << nextSection
                                 << potentialCollision);

                        int i = 0;
                        for(auto pair:sectionPairs)
                        {
                            if(pair.second == section ||
                                    pair.second == nextSection ||
                                    pair.second == potentialCollision)
                            {
                                sectionPairs.removeAt(i);
                            }
                            else
                            {
                                ++i;
                            }
                        }
                    }
                }
            }
        }
    }
}

//////////////////////////////////////////////////////////////
/// \brief TrainMonitor::retrieveSections
/// \param section
/// \return
/// Description: Uses the passed-in QString to find all
///     corresponding sections within the list of sections.
///     Due to the fact that there can be multiple entries
///     (regular section type and zero or more crossover
///     entries), a list is returned.
//////////////////////////////////////////////////////////////
QList<Section> TrainMonitor::retrieveSections(QString section)
{
    QList<Section> ret;
    qDebug() << "retrieveSections() " << section;

	bool found = false;
	for (Section it : m_sectionList)
	{
		if (it.getNode() == section)
		{
            ret.push_back(it);
			found = true;
		}
	}

	if (!found)
	{
		qDebug() << "Invalid section:" << section << ", Are you sure the section exists?" << section;
	}

    return ret;
}

//////////////////////////////////////////////////////////////
/// \brief TrainMonitor::generateSectionList
/// Description: Uses the Sections.txt configuration file to
///     populate the list of sections.
//////////////////////////////////////////////////////////////
void TrainMonitor::generateSectionList()
{
	qDebug() << "generateSectionList";
	printHeader("Loading Section List");

	QString filename = ":/config/Sections";
	QFile infile(filename);
	if (!infile.open(QIODevice::ReadOnly | QIODevice::Text)) {

		qFatal("Cannot find input file: %s", filename.toLatin1().constData());
		return;
	}

	QTextStream in(&infile);
	QString node;
	int numOfConns;
	QString conn1, conn2, conn3, conn4;
    int ltNum; bool thrownLeft, shortSection;
	int lines = 0;
	while (!in.atEnd())
	{

		if (lines == 0)
		{
			printHeader(in.readLine());
		}
		else
		{
			QStringList splitString = in.readLine().split(",");
			node = splitString[0];
			numOfConns = splitString[1].toInt();
			conn1 = splitString[2];
			conn2 = splitString[3];
			conn3 = splitString[4];
			conn4 = splitString[5];
			ltNum = splitString[6].toInt();
			thrownLeft = (splitString[7] == "false") ? false : true;
            shortSection = (splitString[8] == "false") ? false : true;

			Section sec = Section(node, numOfConns, conn1, conn2,
                conn3, conn4, ltNum, thrownLeft, shortSection);
			QStringList shutdownList = node.split("-");
			sec.setBoardNum(shutdownList[0].toInt());
			sec.setSection(shutdownList[1].toInt());
			qDebug() << "TRACK SECTION:" << QString::number(sec.getBoardNum()) << "-" << QString::number(sec.getSection()) << " CONN1:" << conn1 << ", CONN2:" << conn2 << ", CONN3:" << conn3 << ", CONN4:" << conn4;;
			m_sectionList.append(sec);
		}
		lines++;
	}
    printHeader("Done loading " + QString::number(lines) + " sections");
	infile.close();
}

//////////////////////////////////////////////////////////////
/// \brief TrainMonitor::printHeader
/// \param message
/// Description: Logs a header message
//////////////////////////////////////////////////////////////
void TrainMonitor::printHeader(QString message)
{
    qDebug() << "-------------------" + message + "-------------------" << endl;
}

//////////////////////////////////////////////////////////////
/// \brief TrainMonitor::clearSectionPairs
/// Description: Clears all section pairs
//////////////////////////////////////////////////////////////
void TrainMonitor::clearSectionPairs()
{
    this->sectionPairs.clear();
}

//////////////////////////////////////////////////////////////
/// \brief TrainMonitor::handle_serialOpened
/// Description: Initialization method that powers on all
///     sections and throws all switches
//////////////////////////////////////////////////////////////
void TrainMonitor::handle_serialOpened()
{
	qDebug() << "handle_serialOpened()";
	int switches = 0;
	for (Section sec : m_sectionList)
	{
        if (sec.getLtNum() > -1)
		{
			switches++;
			qDebug()<<"Throwing "<< sec.getLtNum();
			emit throwTurnout(sec.getLtNum());
		}
        emit sectionOn(sec.getBoardNum(), sec.getSection());
		QThread::msleep(100);
	}

	qDebug() << switches << " switches have be initialized";
    emit printSectionsOn();
    emit clearSectionOnCount();
    emit systemReady();
}

//////////////////////////////////////////////////////////////
/// \brief TrainMonitor::updateSingleConnList
/// \param current
/// \return
/// Description: Called when an enpoint is entered to
///     attempt to update the list of pairs. Prepares a new
///     pair if unable to find an entry to update.
//////////////////////////////////////////////////////////////
int TrainMonitor::updateSingleConnList(Section current)
{
	qDebug() << "updateSingleConnList()";
    bool ret = 0;
	bool found = false;
	int i = 0;
	for (auto sec : sectionPairs)
	{
		if (sec.second == current.getConn1())
		{
			QString past = current.getConn1();
            m_nextPair =
                std::make_pair<QString, QString>((QString)past, current.getNode());
            m_lastSectionCurrent = past;

            return 2;   // If an endpoint is already in the list, it is heading towards
                        // the endpoint resulting in a collision event.
            found = true;
        }
		++i;
	}

	if (!found)
	{
		qDebug() << "Section " << current.getNode() << " not found, adding to list" << endl;
        m_nextPair =
			std::make_pair<QString, QString>("", current.getNode());
        m_lastSectionCurrent = "NA";

        ret = 0;
	}

	return ret;
}

//////////////////////////////////////////////////////////////
/// \brief TrainMonitor::updateDoubleConnList
/// \param current
/// \return
/// Description: Called when a straight section is entered to
///     attempt to update the list of pairs. Prepares a new
///     pair if unable to find an entry to update.
//////////////////////////////////////////////////////////////
int TrainMonitor::updateDoubleConnList(Section current)
{
	qDebug() << "updateDoubleConnList()";
	bool found = false;
	int i = 0;
	int numOccurrences = 0;

	for (auto sec : sectionPairs)
	{
		if (sec.second == current.getConn1() ||
			sec.second == current.getConn2())
		{
			QString past;
			if (sec.second == current.getConn1())
			{
				past = current.getConn1();
				++numOccurrences;
			}

            if (sec.second == current.getConn2())
			{
				past = current.getConn2();
				++numOccurrences;
			}

            m_nextPair =
                std::make_pair<QString, QString>((QString)past, current.getNode());
            m_lastSectionCurrent = past;
            found = true;

		}
		++i;
	}

	if (!found)
	{
		qDebug() << "Section " << current.getNode() << " not found, adding to list" << endl;
        m_nextPair =
			std::make_pair<QString, QString>("", current.getNode());
        m_lastSectionCurrent = "NA";
	}

    return numOccurrences;
}

//////////////////////////////////////////////////////////////
/// \brief TrainMonitor::updateTripleConnList
/// \param current
/// \return
/// Description: Called when a switch section is entered to
///     attempt to update the list of pairs. Prepares a new
///     pair if unable to find an entry to update.
//////////////////////////////////////////////////////////////
int TrainMonitor::updateTripleConnList(Section current)
{
	qDebug() << "updateTripleConnList()";
	bool found = false;
	int i = 0;
	int numOccurrences = 0;

    for (auto sec : sectionPairs)
	{
		if (sec.second == current.getConn1() ||
			sec.second == current.getConn2() ||
			sec.second == current.getConn3())
		{
			QString past;
			if (sec.second == current.getConn1())
			{
				past = current.getConn1();
				++numOccurrences;
			}

			if (sec.second == current.getConn2())
			{
				past = current.getConn2();
				++numOccurrences;
			}

			if (sec.second == current.getConn3())
			{
				past = current.getConn3();
				++numOccurrences;
			}

            m_nextPair =
                std::make_pair<QString, QString>((QString)past, current.getNode());
            m_lastSectionCurrent = past;

			found = true;
		}
		++i;
	}

	if (!found)
	{
		qDebug() << "Section " << current.getNode() << " not found, adding to list" << endl;
		/// TODO
        m_nextPair =
            std::make_pair<QString, QString>("", current.getNode());
        m_lastSectionCurrent = "NA";
    }

    return numOccurrences;

}

//////////////////////////////////////////////////////////////
/// \brief TrainMonitor::updateSingleConnList
/// \param current
/// \return
/// Description: Called when a crossover is entered to
///     determine if corresponding sections are within the
///     list of pairs. Returns the number of occurrences
///     within the pairs.
//////////////////////////////////////////////////////////////
int TrainMonitor::updateQuadConnList(Section current)
{
	qDebug() << "updateQuadConnList()";
	bool found = false;
	int i = 0;
	int numOccurrences = 0;

    for (auto sec : sectionPairs)
	{
        if (current.getConn1() != "" && sec.second == current.getConn1())
        {
            ++numOccurrences;
        }

        if (current.getConn2() != "" && sec.second == current.getConn2())
        {
            ++numOccurrences;
        }

        if (current.getConn3() != "" && sec.second == current.getConn3())
        {
            ++numOccurrences;
        }


        if (current.getConn4() != "" && sec.second == current.getConn4())
        {
            ++numOccurrences;
        }

        found = true;
		++i;
	}

	if (!found)
	{
		qDebug() << "Section " << current.getNode() << " not found, adding to list" << endl;
        m_nextPair =
			std::make_pair<QString, QString>("", current.getNode());
        m_lastSectionCurrent = "NA";
	}

    return numOccurrences;
}

//////////////////////////////////////////////////////////////
/// \brief TrainMonitor::getNextSection
/// \param currentSections
/// \param section
/// \return
/// Description: Uses the list of currentSections and the
///     QString representation of the current section to
///     determine the next section on the track.
//////////////////////////////////////////////////////////////
QString TrainMonitor::getNextSection(QList<Section> currentSections, QString section)
{
    qDebug() << "getNextSection()";
    QString nextSection;
    for (auto pair : sectionPairs)
    {
        if(pair.second == section)
        {
            if(pair.first == "")
            {
                return "";
            }

            for(auto currentSection : currentSections)
            {
                switch (currentSection.getNumOfConns())
                {
                    case 1:		// Endpoint case
                    {
                        nextSection = currentSection.getConn1();
                        break;
                    }
                    case 2:     // Straight case
                    {
                        QList<Section> currentPair = retrieveSections(pair.second);
                        QList<Section> pastPair = retrieveSections(pair.first);

                        Section here;
                        Section there;

                        for(auto cp : currentPair )
                        {
                            if(cp.getNumOfConns() == 2)
                            {
                                here = cp;
                            }
                        }

                        for(auto pp : pastPair)
                        {
                            if(pp.getNumOfConns() < 4 && pp.getNumOfConns() > 0)
                            {
                                there = pp;
                            }
                        }

                        nextSection = getNextStraightSection(there,here);
                        break;
                    }
                    case 3:     // Switch case
                    {
                        QList<Section> currentPair = retrieveSections(pair.second);
                        QList<Section> pastPair = retrieveSections(pair.first);

                        Section here;
                        Section there;

                        for(auto cp : currentPair )
                        {
                            if(cp.getNumOfConns() == 3)
                            {
                                here = cp;
                            }
                        }

                        for(auto pp : pastPair)
                        {
                            if(pp.getNumOfConns() < 4 && pp.getNumOfConns() > 0)
                            {
                                there = pp;
                            }
                        }

                        nextSection = getNextSwitchSection(there, here);
                        break;
                    }
                    case 4:
                    {
                        // This case will never be used for the next section
                        break;
                    }
                    default:
                    {
                        qDebug() << "Invalid numOfConns during nextSection lookup";
                    }
                }
            }
        }
    }

    return nextSection;
}

//////////////////////////////////////////////////////////////
/// \brief TrainMonitor::getNextSectionWithoutPairs
/// \param current
/// \param future
/// \return
/// Description: Used when trying to find the section
///     following a short section. Since the short section
///     won't be in the pairs, the pairs cannot be used to
///     find the section.
//////////////////////////////////////////////////////////////
QString TrainMonitor::getNextSectionWithoutPairs(QString current, QString future)
{
    qDebug() << "getNextSectionWithoutPairs()";
    QString nextSection;
    for (auto pair : sectionPairs)
    {
        if(pair.second == current)
        {
            QList<Section> currentList = retrieveSections(current);
            QList<Section> futureList = retrieveSections(future);
            Section currentSection;
            Section futureSection;

            for(auto cs: currentList)
            {
                if(cs.getNumOfConns() != 4)
                {
                    currentSection = cs;
                }
            }

            for(auto fs: futureList)
            {
                if(fs.getNumOfConns() != 4)
                {
                    futureSection = fs;
                }
            }

            switch (futureSection.getNumOfConns())
            {
                case 1:		// Endpoint case
                {
                    // This will never get to this point without causing a collision event
                }
                case 2:     // Straight case
                {
                    nextSection = getNextStraightSection(currentSection,futureSection);
                    break;
                }
                case 3:     // Switch case
                {
                    nextSection = getNextSwitchSection(currentSection, futureSection);
                    break;
                }
                case 4:
                {
                    // This case will never be used for the next section
                    break;
                }
                default:
                {
                    qDebug() << "Invalid numOfConns during nextSection lookup";
                }
            }
        }
    }

    return nextSection;
}

//////////////////////////////////////////////////////////////
/// \brief TrainMonitor::getNextStraightSection
/// \param previous
/// \param current
/// \return
/// Description: Used to find the next section when currently
///     within a straight section.
//////////////////////////////////////////////////////////////
QString TrainMonitor::getNextStraightSection(Section previous, Section current)
{
	qDebug() << "getNextStraightSection()";
    qDebug() << "previous = " << previous.getNode() << "\tcurrent = " << current.getNode();
	QString nextSection;
	if (previous.getNode() == current.getConn1())
	{
		nextSection = current.getConn2();
	}
	else
	{
		nextSection = current.getConn1();
	}

	return nextSection;
}

//////////////////////////////////////////////////////////////
/// \brief TrainMonitor::getNextSwitchSection
/// \param previous
/// \param current
/// \return
/// Description: Used to find the next section when currently
///     within a switch section.
/// //////////////////////////////////////////////////////////////
QString TrainMonitor::getNextSwitchSection(Section previous, Section current)
{
	qDebug() << "getNextSwitchSection()";
	QString nextSection;

	if (previous.getNode() == current.getConn2() ||
		previous.getNode() == current.getConn3())
	{
		nextSection = current.getConn1();
	}
	else
	{
		if (!current.getThrownLeft())
		{
			if (current.getThrown())
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
			if (current.getThrown())
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

//////////////////////////////////////////////////////////////
/// \brief TrainMonitor::do_handleSwitch
/// \param LT
/// \param closed
/// Description: Interface class for switch configuration
///     changes.
//////////////////////////////////////////////////////////////
void TrainMonitor::do_handleSwitch(int LT, bool closed)
{
	qDebug() << "do_handleSwitch() entered";
    qDebug() << "New Switch Data at Monitor for Turnout:" << LT << " Closed_State:" << closed;

	for (auto section : m_sectionList)
	{
		if (section.getLtNum() == LT)
		{
			if (closed)
			{
				section.setThrown(false);
			}
			else
			{
				section.setThrown(true);
			}
		}
	}
}
