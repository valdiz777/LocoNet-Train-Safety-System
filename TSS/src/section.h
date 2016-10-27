
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


namespace switchinfo{
    enum class TURNOUT_STATE{not_set = 0, thrown = 1, closed = 2};
}

class Section
{

public:
	Section();
    Section(int boardNum, int section, switchinfo::TURNOUT_STATE switchDirection, QString node, int numOfConns, QString conn1,
		QString conn2, QString conn3, QString conn4);
	~Section();

	//void setX( int X );
    int getBoardNum();

	//void setY( int Y );
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

    switchinfo::TURNOUT_STATE getSwitchDirection();

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
	int m_x;
	int m_y;
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
    switchinfo::TURNOUT_STATE m_switchDirection;
};

#endif
