#ifndef TRAINMONITOR_H
#define TRAINMONITOR_H

#include <QString>
#include <QList>
#include <QQueue>
#include <queue>
#include "section.h"
#include "enginepath.h"
#include "locopacket.h"
#include <queue>

namespace std {
	class TrainMonitor;
}

struct engineSection
{
	EnginePath path;
	Section sec;
};

class TrainMonitor : public QObject
{
	Q_OBJECT

public:
	/*struct engineSection
	{
		EnginePath path;
		Section sec;
	};*/


	//static TrainMonitor *getInstance;
	TrainMonitor();
	~TrainMonitor();
	void startMonitor();
	void stopMonitor();
    void Monitor(QString monitorSection);
	void addEngineSectionToQueue(EnginePath Path, Section section);
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

signals:
	void slotScan(LocoByte _slot);
	void slotDispatch(LocoByte _slot);
	void slotClear(LocoByte _slot);
	void slotReq(LocoByte _slot);
	void slotUse(LocoByte _slot);
	void trackReset();
	void trackOn();
	void trackOff();

protected:

private:
	//TrainMonitor();
	//~TrainMonitor();
	void endpointMonitor(Section sec);
	//void straightMonitor(Section sec, engineSection es);
    void straightMonitor(Section sec);
    void switchMonitor(Section sec);
    void crossoverMonitor(Section sec);
    QList<std::pair<QString, QString>> sectionPairs; // First QString = lastSection, Second = currentSection

    bool running;
	QQueue<engineSection> m_engineSectionQueue;
	void generateSectionList();
	Section retrieveSections(QString section);
	bool updateEnginePath(QString engine);
	QList<Section> m_sectionList;
	QList<EnginePath> m_engineList;
	Section findNextSection(Section previousSection, Section currentSection);
	void toggleSwitchDirectionLeft(QString section);
	void toggleSwitchDirectionRight(QString section);
	EnginePath engineOnSection(QString section);
};
#endif
