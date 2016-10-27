#ifndef ENGINEPATH_H
#define ENGINEPATH_H

#include <QString>
#include <QList>
#include <list>
#include "section.h"

namespace std {
	class EnginePath;
}


class EnginePath
{
public:
	EnginePath();
	EnginePath(QString Engine, Section CurrentSection,
		Section NextSection1, Section NextSection2,
		Section NextSection3, Section NextSection4, int Throttle,
		int Direction);
	~EnginePath();

	//int getId(  );

	void setEngine(QString engine);
	QString getEngine();

	void setCurrentSection(Section section);
	Section getCurrentSection();

	void setNextSection1(Section section);
	Section getNextSection1();

	void setNextSection2(Section section);
	Section getNextSection2();

	void setNextSection3(Section section);
	Section getNextSection3();

	void setNextSection4(Section section);
	Section getNextSection4();

	// void setNextSection5( QString section );
	// QString getNextSection5(  );

	// void setNextSection6( QString section );
	// QString getNextSection6(  );

	// void setNextSection7( QString section );
	// QString getNextSection7(  );

	// void setNextSection8( QString section );
	// QString getNextSection8(  );

	// void setNextSection9( QString section );
	// QString getNextSection9(  );

	// void setNextSection10( QString section );
	// QString getNextSection10(  );

	void setThrottle(int throttle);
	int getThrottle();

	void setDirection(int direction);
	int getDirection();

	//EnginePath RetrieveEnginePath(QString engine);
	//bool CreateEnginePath( QString engine );

	//bool ConnectToMySQL();

private:
	QString m_engine;
	Section m_currentSection;
	Section m_nextSection1;
	Section m_nextSection2;
	Section m_nextSection3;
	Section m_nextSection4;
	//QString m_nextSection5;
	//QString m_nextSection6;
	//QString m_nextSection7;
	//QString m_nextSection8;
	//QString m_nextSection9;
	//QString m_nextSection10;
	int m_throttle;
	int m_direction;


};
#endif
