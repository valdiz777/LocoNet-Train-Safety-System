#ifndef TRAINMONITOR_H
#define TRAINMONITOR_H

#include <QString>
#include <QList>
#include <QQueue>
#include <queue>
#include "section.h"
#include "locopacket.h"
#include <queue>

namespace std {
	class TrainMonitor;
}

class TrainMonitor : public QObject
{
	Q_OBJECT

public:

	TrainMonitor();
	~TrainMonitor();
	void startMonitor();
	void stopMonitor();
    void Monitor(QString monitorSection);
    void updateSingleConnList(Section current);
    void updateDoubleConnList(Section current);
    void updateTripleConnList(Section current);
    void updateQuadConnList(Section current);
    //void straightMonitor(Section sec);
    QString getNextStraightSection(Section previous, Section current);
    QString getNextSwitchSection(Section previous, Section current);
    QString getNextEndpointSection(Section previous, Section current);

public slots:
	void do_run();
	void do_handleOccupancy(QString sec, bool state);
    void do_handleSwitch(int LT, bool closed);
    void handle_serialOpened();

signals:
	void slotScan(LocoByte _slot);
	void slotDispatch(LocoByte _slot);
	void slotClear(LocoByte _slot);
	void slotReq(LocoByte _slot);
	void slotUse(LocoByte _slot);
	void trackReset();
	void trackOn();
	void trackOff();
    void sectionOff(int boardNum, int section);
    void sectionOn(int boardNum, int section);
    void closeTurnout(int locoTurnout);
    void throwTurnout(int locoTurnout);
    void clearSectionOnCount();
    void clearSectionOffCount();
    void printSectionsOn();
    void printSectionsOff();

protected:

private:
	void endpointMonitor(Section sec);
	//void straightMonitor(Section sec, engineSection es);
    void straightMonitor(Section sec);
    void switchMonitor(Section sec);
    void crossoverMonitor(Section sec);
    QList<std::pair<QString, QString>> sectionPairs; // First QString = lastSection, Second = currentSection

    bool running;
	void generateSectionList();
	Section retrieveSections(QString section);
	QList<Section> m_sectionList;
	Section findNextSection(Section previousSection, Section currentSection);
	void toggleSwitchDirectionLeft(QString section);
	void toggleSwitchDirectionRight(QString section);
};
#endif
