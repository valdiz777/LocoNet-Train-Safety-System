
#ifndef SECTION_H
#define SECTION_H

#include <QString>
#include <QVector>
#include <QList>
#include <iostream>
#include <list>
#include <vector>
#include <utility>

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

namespace switchinfo {
    enum TURNOUT_STATE{NOT_SET, THROWN, CLOSED};
}

class Section
{

public:
	Section();
    Section(int boardNum, int section, QString node, int numOfConns, QString conn1,

		QString conn2, QString conn3, QString conn4);
	~Section();

    //void setBoardNum( int boardNum );
    int getBoardNum();

    //void setSection( int section );
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

	bool getSwitchDirection();

	bool ConnectToMySQL();

	void GenerateSectionList();
	void toggleSwitchDirectionLeft();
	void toggleSwitchDirectionRight();
	void setOccupancy(bool state);
	bool getOccupancy();

    bool getThrown();
    void setThrown(bool thrown);
    bool getThrownLeft();

private:
    int m_boardNum;
    int m_section;
	//TrackType::TrackType type;
	QString m_node;
	int m_numOfConns;
	QString m_conn1;
	QString m_conn2;
	QString m_conn3;
	QString m_conn4;
    bool m_thrown;		// true for left; false for right
    bool m_thrownLeft;
	bool m_occupancy;
};

#endif
