
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

class Section
{

public:
	Section();
	Section(QString node, int numOfConns, QString conn1, QString conn2, QString conn3, QString conn4, int ltNum, bool thrownLeft);
	~Section();

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

	void setOccupancy(bool state);
	bool getOccupancy();

	bool getThrown();
	void setThrown(bool thrown);
	bool getThrownLeft();
	int getLtNum();

	int getBoardNum() const;
	void setBoardNum(int boardNum);

	int getSection() const;
	void setSection(int section);

	QString getConn4() const;
	void setConn4(const QString &conn4);

private:
	int m_boardNum;
	int m_section;
	QString m_node;
	int m_numOfConns;
	QString m_conn1;
	QString m_conn2;
	QString m_conn3;
	QString m_conn4;
	int m_ltnum;
	bool m_thrownLeft;
	bool m_thrown;		// true for left; false for right
	bool m_occupancy;
};

#endif
