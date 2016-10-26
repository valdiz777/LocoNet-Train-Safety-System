
#ifndef SECTION_H
#define SECTION_H

#include <QString>
#include <QVector>
#include <QList>
#include <iostream>
#include <list>
#include <vector>


namespace std {
	class Section;
}

// namespace TrackType
// {
// 	enum TrackType
// 	{
// 		DEFAULT,
// 		ENDPOINT,
// 		STRAIGHT,
// 		SWITCH,
// 		CROSSOVER
// 	};
// }

namespace SwitchInfo{
    enum class TURNOUT_STATE{not_set = 0, thrown = 1, closed = 2};
}

class Section
{

public:
	Section();
    Section(int boardNum, int section, SwitchInfo::TURNOUT_STATE switchDirection, QString node, int numOfConns, QString conn1,
		QString conn2, QString conn3, QString conn4);
	~Section();

    void setBoardNum( int boardNum );
    int getBoardNum();

    void setSection( int section );
    int getSection();

	//void setNode( QString node );
	QString getNode();

	//void setNumOfConns( int numConn );
	int getNumOfConns();

	//void setConn1( QString conn1 );
	QString getConn1();

	//void setConn2( QString conn1 );
	QString getConn2();

	//void setConn3( QString conn1 );
	QString getConn3();

	//void setConn4( QString conn1 );
	QString getConn4();

    SwitchInfo::TURNOUT_STATE getSwitchDirection();

	bool ConnectToMySQL();

    QString Section::getSwitchDirectionString();

	void GenerateSectionList();
	void toggleSwitchDirectionLeft();
	void toggleSwitchDirectionRight();
	void setOccupancy(bool state);
	bool getOccupancy();
    QStringList TURNOUT_STATE_STRING = {"not_set", "thrown", "closed"};
private:
    int m_boardNum;
    int m_section;
	QString m_node;
	int m_numOfConns;
	QString m_conn1;
	QString m_conn2;
	QString m_conn3;
	QString m_conn4;
    //bool m_switchDirection;		// true for left; false for right
	bool m_occupancy;
    SwitchInfo::TURNOUT_STATE m_switchDirection;
};

#endif
