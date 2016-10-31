#include "trainmonitor.h"
#include <QFile>
#include <QTextStream>
#include <QThread>

TrainMonitor::TrainMonitor()
{
#pragma message("[MKJ] Clean up when everything is done")
#pragma message("[MKJ] Look for things that would be useful in a log")

}

TrainMonitor::~TrainMonitor()
{


}

void TrainMonitor::do_run() {
    qDebug() << "do_run()";
    generateSectionList();
}

void TrainMonitor::do_handleOccupancy(QString section, bool state)
{
    qDebug() << "do_handleOccupancy entered";
    qDebug() << "New Occupancy Data at Monitor:" << section << " State:" << state;
	Section monitoredSection = retrieveSections(section);
	monitoredSection.setOccupancy(state);
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

    Section currentSection = retrieveSections(section);
    qDebug() << "Starting monitor ....." << endl;
    switch (currentSection.getNumOfConns())
    {
        case 1:		// Endpoint case
        {
            updateSingleConnList(currentSection);
            qDebug() << "entered endpoint monitor" << endl;
            endpointMonitor(currentSection);
            break;
        }
        case 2:		// Straight case
        {
            if(!updateDoubleConnList(currentSection))
            {
                qDebug() << "Section " << currentSection.getNode() << " needs to be shutdown (straight)";
                qDebug() << "Section " << currentSection.getConn1() << " needs to be shutdown (straight)";
                qDebug() << "Section " << currentSection.getConn2() << " needs to be shutdown (straight)";
                emit collisionEvt(QStringList() << currentSection.getNode() << currentSection.getConn1() << currentSection.getConn2());
            }
            //qDebug() << "entered straight monitor" << endl;
            //straightMonitor(currentSection);
            break;
        }
        case 3:		// Switch case
        {
            if(!updateTripleConnList(currentSection))
            {
                qDebug() << "Section " << currentSection.getNode() << " needs to be shutdown (switch)";
                qDebug() << "Section " << currentSection.getConn1() << " needs to be shutdown (switch)";
                qDebug() << "Section " << currentSection.getConn2() << " needs to be shutdown (switch)";
                qDebug() << "Section " << currentSection.getConn3() << " needs to be shutdown (switch)";
            }
//            qDebug() << "entered switch monitor" << endl;
//            switchMonitor(currentSection);
            break;
        }
        case 4:		// Crossover case
        {
            updateQuadConnList(currentSection);
            qDebug() << "entered crossover monitor" << endl;
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
    qDebug() << "endpointMonitor()";
	// Shutdown the section since the means the train is headed
    // towards the end of the track
    for (auto section : sectionPairs)
    {
        if (sec.getNode() == section.second)
        {
            if(section.first != "")
            {
                qDebug() << "Shutting off section " << section.second;
                emit collisionEvt(QStringList() << section.second);
            }
        }
    }
}

void TrainMonitor::straightMonitor(Section sec)
{
    qDebug() << "straightMonitor()" << sec.getNode();
    QString nextSection;
    bool found = false;

    for (auto section : sectionPairs)
    {

        qDebug() << sec.getNode() << " sec.second is:" << section.second << endl;
        if(sec.getNode() == section.second)
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

    for(auto section : sectionPairs)
    {
        qDebug() << "nextSection: " << nextSection << " section.second:" << section.second << endl;
        if(nextSection == section.second)
        {

            qDebug() << "Shutting off section " << section.second;
            emit collisionEvt(QStringList() << section.second);
        }
    }
}

void TrainMonitor::switchMonitor(Section sec)
{
    qDebug() << "switchMonitor()";
    QString monitorSection;
    QString nextSection;
    for (auto section : sectionPairs)
    {
        if(sec.getNode() == section.second)
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
    }

    for (auto section : sectionPairs)
    {
//        if(section.second == monitorSection)
        if(section.second == nextSection)
        {
            qDebug() << "Shutting off section " << section.second;
            emit  collisionEvt(QStringList() << section.second);
        }
    }
}

//void TrainMonitor::crossoverMonitor(Section sec, engineSection es)
//{

//}

Section TrainMonitor::retrieveSections(QString section)
{
    qDebug() << "retrieveSections() " << section;
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
        qDebug() << "Invalid section:" << section << ", Are you sure the section exists?" << section;
	}

	return newSection;
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
    int ltNum; bool thrownLeft;
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
            thrownLeft = (splitString[7] == "false")? false : true;

            Section sec = Section(node, numOfConns, conn1, conn2,
                conn3, conn4, ltNum, thrownLeft);
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
qDebug() << "-------------------"+ message + "-------------------" << endl;
}

void TrainMonitor::handle_serialOpened()
{
    qDebug() << "handle_serialOpened()";
    int switches = 0;
    for (Section  sec : m_sectionList)
    {
        if (sec.getNumOfConns() ==  3)
        {
            switches++;
            emit throwTurnout(sec.getLtNum());
        }
        emit sectionOn(sec.getBoardNum(),sec.getSection());
        QThread::msleep(100);
    }

    qDebug() << switches << " switches have be initialized";
    emit printSectionsOn();
}

Section TrainMonitor::findNextSection(Section previousSection,
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
}

/// Updates to an endpoint.....fix comment later
void TrainMonitor::updateSingleConnList(Section current)
{
    qDebug() << "updateSingleConnList()";
    bool found = false;
    int i = 0;
    for(auto sec : sectionPairs)
    {
        if (sec.second == current.getConn1())
        {
            qDebug() << "Found a pair to update (endpoint)";
//            sec.first = sec.second;
//            sec.second = current.getNode();

            QString past = current.getConn1();
            std::pair<QString, QString> secPair=
                    std::make_pair<QString, QString>((QString)past, current.getNode());
            sectionPairs.push_back(secPair);
            sectionPairs.removeAt(i);

            found = true;
        }
        ++i;
    }

    if(!found)
    {
        qDebug() << "Section " << current.getNode() << " not found, adding to list" << endl;
        /// TODO
        std::pair<QString, QString> secPair=
                std::make_pair<QString, QString>("", current.getNode());
        sectionPairs.push_back(secPair);
    }
}

/// Updates strait aways.....
bool TrainMonitor::updateDoubleConnList(Section current)
{
    bool ret = false;
    qDebug() << "updateDoubleConnList()";
    bool found = false;
    int i = 0;
    int numOccurrences = 0;

    for(auto sec : sectionPairs)
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
            if(sec.second == current.getConn1())
            {
                past = current.getConn1();
                ++numOccurrences;
            }

            if(sec.second == current.getConn1())
            {
                past = current.getConn2();
                ++numOccurrences;
            }

            std::pair<QString, QString> secPair=
                    std::make_pair<QString, QString>((QString)past, current.getNode());
            sectionPairs.push_back(secPair);
            sectionPairs.removeAt(i);
            found = true;

        }
        ++i;
    }

    if(!found)
    {
        qDebug() << "Section " << current.getNode() << " not found, adding to list" << endl;
        /// TODO
//        if(sectionPairs.count() == 0)
//        {
            std::pair<QString, QString> secPair=
                    std::make_pair<QString, QString>("", current.getNode());
            sectionPairs.push_back(secPair);
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

    if(numOccurrences < 2)
    {
        ret = true;
    }

    return ret;
}

/// Updates switches.....
bool TrainMonitor::updateTripleConnList(Section current)
{
    qDebug() << "updateTripleConnList()";
    bool found = false;
    int i = 0;
    int numOccurrences = 0;
    bool ret = false;
    for(auto sec : sectionPairs)
    {
        if (sec.second == current.getConn1() ||
                sec.second == current.getConn2() ||
                sec.second == current.getConn3())
        {
            qDebug() << "Found a pair to update (switch)";
//            sec.first = sec.second;
//            sec.second = current.getNode();

            QString past;
            if(sec.second == current.getConn1())
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

            std::pair<QString, QString> secPair=
                    std::make_pair<QString, QString>((QString)past, current.getNode());
            sectionPairs.push_back(secPair);
            sectionPairs.removeAt(i);

            found = true;
        }
        ++i;
    }

    if(!found)
    {
        qDebug() << "Section " << current.getNode() << " not found, adding to list" << endl;
        /// TODO
        std::pair<QString, QString> secPair =
            std::make_pair<QString, QString>("", current.getNode());
        sectionPairs.push_back(secPair);
    }

    if(numOccurrences < 2)
    {
        ret = true;
    }

    return ret;

}

/// Updates Crossovers....
void TrainMonitor::updateQuadConnList(Section current)
{
    qDebug() << "updateQuadConnList()";
    // TODO
}

QString TrainMonitor::getNextStraightSection(Section previous, Section current)
{
    qDebug() << "getNextStraightSection()";
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
    qDebug() << "getNextSwitchSection()";
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

void TrainMonitor::do_handleSwitch(int LT, bool closed)
{
    qDebug() << "do_handleSwitch() entered";

    for(auto section : m_sectionList)
    {
        if(section.getLtNum() == LT)
        {
            if(closed)
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
