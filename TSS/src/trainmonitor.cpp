#include "trainmonitor.h"
#include <QFile>
#include <QTextStream>
#include <QThread>

TrainMonitor::TrainMonitor()
{

}

TrainMonitor::~TrainMonitor()
{


}

void TrainMonitor::do_run() {
	qDebug() << "do_run()";
    //Uncomment below to test emit collisionEvt();
    //emit collisionEvt(QStringList() << "1-1");
    generateSectionList();
}

void TrainMonitor::do_handleOccupancy(QString section, bool state)
{
	qDebug() << "do_handleOccupancy entered";
	qDebug() << "New Occupancy Data at Monitor:" << section << " State:" << state;
    QList<Section> monitoredSections = retrieveSections(section);

    for(auto it : monitoredSections)
    {
        it.setOccupancy(state);
    }

	//if (state) Monitor(monitoredSection);
	if (state) Monitor(section);
}

void TrainMonitor::Monitor(QString section)
{
	qDebug() << "Monitor()";
	qDebug() << endl << endl << "Mike's debug";

	for (auto section : sectionPairs)
	{
		qDebug() << "last section is: " << section.first << " current section is:" << section.second << endl;
	}

	qDebug() << endl << endl;
#pragma message("[MKJ] Will nextSection1 be enough padding for a crash scenario?")
	//Section monitorSection = es.path.getNextSection1();

    int totalConnections = 0;
    QList<Section> currentSections = retrieveSections(section);
	qDebug() << "Starting monitor ....." << endl;

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

    qDebug() << "totalConnections: " << totalConnections;
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
                             << currentSection.getNode());

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
                    qDebug() << "Section "
                             << currentSection.getNode()
                             << " needs to be shutdown (crossover)";
                    qDebug() << "Section "
                             << currentSection.getConn1()
                             << " needs to be shutdown (crossover)";
                    qDebug() << "Section "
                             << currentSection.getConn2()
                             << " needs to be shutdown (crossover)";
                    qDebug() << "Section "
                             << currentSection.getConn3()
                             << " needs to be shutdown (crossover)";
                    qDebug() << "Section "
                             << currentSection.getConn4()
                             << " needs to be shutdown (crossover)";
                    emit collisionEvt(QStringList()
                             << currentSection.getNode()
                             << currentSection.getConn1()
                             << currentSection.getConn2()
                             << currentSection.getConn3()
                             << currentSection.getConn4());

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
        qDebug() << "Attempting to update/create pair";
        qDebug() << endl << endl << "Mike's debug 2.0";

        int i = 0;
        if(m_lastSectionCurrent != "NA")
        {
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

        sectionPairs.push_back(m_nextPair);
        qDebug() << endl << endl << "Mike's debug 3.0";

        for (auto section : sectionPairs)
        {
            qDebug() << "last section is: " << section.first << " current section is:" << section.second << endl;
        }
    }
}

/*void TrainMonitor::endpointMonitor(Section sec)
{
	qDebug() << "endpointMonitor()";
	// Shutdown the section since the means the train is headed
	// towards the end of the track
	for (auto section : sectionPairs)
	{
		if (sec.getNode() == section.second)
		{
			if (section.first != "")
			{
				qDebug() << "Shutting off section " << section.second;
				emit collisionEvt(QStringList() << section.second);
			}
		}
	}
}*/

/*void TrainMonitor::straightMonitor(Section sec)
{
	qDebug() << "straightMonitor()" << sec.getNode();
	QString nextSection;
	bool found = false;

	for (auto section : sectionPairs)
	{

		qDebug() << sec.getNode() << " sec.second is:" << section.second << endl;
		if (sec.getNode() == section.second)
		{
			nextSection = getNextStraightSection(
				retrieveSections(section.first), retrieveSections(section.second));
			found = true;
		}
	}


	/* if(!found)
	 {
		 qDebug() << "Shit didn't work homie!!!";
	 }*/

    /*for (auto section : sectionPairs)
	{
		qDebug() << "nextSection: " << nextSection << " section.second:" << section.second << endl;
		if (nextSection == section.second)
		{

			qDebug() << "Shutting off section " << section.second;
			emit collisionEvt(QStringList() << section.second);
		}
	}
}*/

/*void TrainMonitor::switchMonitor(Section sec)
{
	qDebug() << "switchMonitor()";
	QString monitorSection;
	QString nextSection;
	for (auto section : sectionPairs)
	{
		if (sec.getNode() == section.second)
		{
			nextSection = getNextStraightSection(
				retrieveSections(section.first), retrieveSections(section.second));
		}

		/*if(nextSection != "")
		{
			switch (retrieveSections(nextSection).getNumOfConns())
			{
				case 1: {
					qDebug() << "Shutting off section " << section.second;
					Section shutdownSection = retrieveSections(nextSection);
					QStringList shutdownList = shutdownSection.getNode().split("-");
					emit sectionOff(shutdownList[0].toInt(),shutdownList[1].toInt());

					break;
				} case 2: {
					monitorSection = getNextStraightSection(
								retrieveSections(section.first), retrieveSections(nextSection));
					break;
				} case 3: {
					monitorSection = getNextStraightSection(
								retrieveSections(section.second), retrieveSections(nextSection));
					break;
				} case 4:
					// TODO
					break;
			}
		}*/
    /*}

	for (auto section : sectionPairs)
	{
		//        if(section.second == monitorSection)
		if (section.second == nextSection)
		{
			qDebug() << "Shutting off section " << section.second;
			emit  collisionEvt(QStringList() << section.second);
		}
	}
}*/

//void TrainMonitor::crossoverMonitor(Section sec, engineSection es)
//{

//}

QList<Section> TrainMonitor::retrieveSections(QString section)
{
    QList<Section> ret;
    qDebug() << "retrieveSections() " << section;
	//int NotFoundIndex;

	bool found = false;
	for (Section it : m_sectionList)
	{
		if (it.getNode() == section)
		{
            ret.push_back(it);
			found = true;
			break;
		}
	}

	if (!found)
	{
		qDebug() << "Invalid section:" << section << ", Are you sure the section exists?" << section;
	}

    return ret;
}

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
	printHeader("Done loading " + QString::number(lines) + "sections");
	infile.close();
}

void TrainMonitor::printHeader(QString message)
{
    qDebug() << "-------------------" + message + "-------------------" << endl;
}

void TrainMonitor::clearSectionPairs()
{
    this->sectionPairs.clear();
}

void TrainMonitor::handle_serialOpened()
{
	qDebug() << "handle_serialOpened()";
	int switches = 0;
	for (Section sec : m_sectionList)
	{
		if (sec.getNumOfConns() == 3)
		{
			switches++;
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

/*Section TrainMonitor::findNextSection(Section previousSection,
	Section currentSection)
{
	qDebug() << "findNextSection()";
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
}*/

/// Updates to an endpoint.....fix comment later
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
			qDebug() << "Found a pair to update (endpoint)";
			//            sec.first = sec.second;
			//            sec.second = current.getNode();

			QString past = current.getConn1();
//            std::pair<QString, QString> secPair =
            m_nextPair =
                std::make_pair<QString, QString>((QString)past, current.getNode());
            m_lastSectionCurrent = past;
//            sectionPairs.push_back(secPair);
//            sectionPairs.removeAt(i);

            return 2;   // If an endpoint is already in the list, it is heading towards
                        // the endpoint resulting in a collision event.
            found = true;
        }
		++i;
	}

	if (!found)
	{
		qDebug() << "Section " << current.getNode() << " not found, adding to list" << endl;
		/// TODO
//		std::pair<QString, QString> secPair =
        m_nextPair =
			std::make_pair<QString, QString>("", current.getNode());
        m_lastSectionCurrent = "NA";
//        sectionPairs.push_back(secPair);
        ret = 0;
	}

	return ret;
}

/// Updates strait aways.....
int TrainMonitor::updateDoubleConnList(Section current)
{
	qDebug() << "updateDoubleConnList()";
	bool found = false;
	int i = 0;
	int numOccurrences = 0;

	for (auto sec : sectionPairs)
	{
		qDebug() << "current.conn1 = '" << current.getConn1() << "'\tcurrent.conn2 = '" << current.getConn2() << "'";
		if (sec.second == current.getConn1() ||
			sec.second == current.getConn2())
		{
			qDebug() << "Found a pair to update (straight)";
			//            sec.first = sec.second;
			//            sec.second = current.getNode();
			//            qDebug() << "current.conn1(AU) = '" << current.getConn1() << "'\tcurrent.conn2(AU) = '" << current.getConn2() << "'";

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

//            std::pair<QString, QString> secPair =
            m_nextPair =
                std::make_pair<QString, QString>((QString)past, current.getNode());
//            sectionPairs.push_back(secPair);
//            sectionPairs.removeAt(i);
            m_lastSectionCurrent = past;
            found = true;

		}
		++i;
	}

	if (!found)
	{
		qDebug() << "Section " << current.getNode() << " not found, adding to list" << endl;
		/// TODO
//        if(sectionPairs.count() == 0)
//        {
//		std::pair<QString, QString> secPair =
        m_nextPair =
			std::make_pair<QString, QString>("", current.getNode());
        m_lastSectionCurrent = "NA";
//        sectionPairs.push_back(secPair);
		//        }
		//        else if (current.getConn1() == "")
		//        {
		//            std::pair<QString, QString> secPair=
		//                    std::make_pair<QString, QString>(current.getConn2(), current.getNode());
		//            sectionPairs.push_back(secPair);
		//        }
		//        else
		//        {
		//            std::pair<QString, QString> secPair=
		//                    std::make_pair<QString, QString>(current.getConn1(), current.getNode());
		//            sectionPairs.push_back(secPair);
		//        }
	}

    return numOccurrences;
}

/// Updates switches.....
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
			qDebug() << "Found a pair to update (switch)";
			//            sec.first = sec.second;
			//            sec.second = current.getNode();

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

//            std::pair<QString, QString> secPair =
            m_nextPair =
                std::make_pair<QString, QString>((QString)past, current.getNode());
            m_lastSectionCurrent = past;
//            sectionPairs.push_back(secPair);
//            sectionPairs.removeAt(i);

			found = true;
		}
		++i;
	}

	if (!found)
	{
		qDebug() << "Section " << current.getNode() << " not found, adding to list" << endl;
		/// TODO
//        std::pair<QString, QString> secPair =
        m_nextPair =
            std::make_pair<QString, QString>("", current.getNode());
        m_lastSectionCurrent = "NA";
//        sectionPairs.push_back(secPair);
    }

    return numOccurrences;

}

/// Updates Crossovers....
int TrainMonitor::updateQuadConnList(Section current)
{
	qDebug() << "updateQuadConnList()";
	bool found = false;
	int i = 0;
	int numOccurrences = 0;
	bool ret = false;
	for (auto sec : sectionPairs)
	{
        qDebug() << "Found a pair to update (crossover)";
        //            sec.first = sec.second;
        //            sec.second = current.getNode();

        QString past;
        if (current.getConn1() != "" && sec.second == current.getConn1())
        {
            past = current.getConn1();
            ++numOccurrences;
        }

        if (current.getConn2() != "" && sec.second == current.getConn2())
        {
            past = current.getConn2();
            ++numOccurrences;
        }

        if (current.getConn3() != "" && sec.second == current.getConn3())
        {
            past = current.getConn3();
            ++numOccurrences;
        }


        if (current.getConn4() != "" && sec.second == current.getConn4())
        {
            past = current.getConn4();
            ++numOccurrences;
        }

//        if (sec.second == current.getNode())
//        {
//            past = current.getNode();
//            numOccurrences += 2;
//        }


//            std::pair<QString, QString> secPair =
        m_nextPair =
            std::make_pair<QString, QString>((QString)past, current.getNode());
        m_lastSectionCurrent = past;
//            sectionPairs.push_back(secPair);
//            sectionPairs.removeAt(i);

        found = true;
		++i;
	}

	if (!found)
	{
		qDebug() << "Section " << current.getNode() << " not found, adding to list" << endl;
		/// TODO
//		std::pair<QString, QString> secPair =
        m_nextPair =
			std::make_pair<QString, QString>("", current.getNode());
        m_lastSectionCurrent = "NA";
//		sectionPairs.push_back(secPair);
	}

    return numOccurrences;
}

QString TrainMonitor::getNextStraightSection(Section previous, Section current)
{
	qDebug() << "getNextStraightSection()";
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

void TrainMonitor::do_handleSwitch(int LT, bool closed)
{
	qDebug() << "do_handleSwitch() entered";

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
