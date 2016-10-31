#ifndef TRAINMONITOR_H
#define TRAINMONITOR_H

#include <QString>
#include <QList>
#include <QQueue>
#include <queue>
#include "section.h"
#include "locopacket.h"
#include <queue>
#include "enginepath.h"

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
    bool updateDoubleConnList(Section current);
    bool updateTripleConnList(Section current);
    void updateQuadConnList(Section current);
    QString getNextStraightSection(Section previous, Section current);
    QString getNextSwitchSection(Section previous, Section current);
    QString getNextEndpointSection(Section previous, Section current);
    void generateSectionList();
    void printHeader(QString message);

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
    void nodeOn(QString node);
    void nodeOff(QString node);
    void closeTurnout(int locoTurnout);
    void throwTurnout(int locoTurnout);
    void clearSectionOnCount();
    void clearSectionOffCount();
    void printSectionsOn();
    void printSectionsOff();
    void collisionEvt(QStringList collisionSections);

protected:

private:
	void endpointMonitor(Section sec);
    void straightMonitor(Section sec);
    void switchMonitor(Section sec);
    void crossoverMonitor(Section sec);
    QList<std::pair<QString, QString>> sectionPairs; // First QString = lastSection, Second = currentSection
    QList<EnginePath> enginePaths; // may take place of sectionPairs.......

    bool running;
	Section retrieveSections(QString section);
	QList<Section> m_sectionList;
	Section findNextSection(Section previousSection, Section currentSection);
	void toggleSwitchDirectionLeft(QString section);
	void toggleSwitchDirectionRight(QString section);
};
#endif
