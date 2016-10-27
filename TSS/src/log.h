#ifndef LOG_H
#define LOG_H

#include <iomanip>
#include <ctime>
#include <QString>
#include <QDebug>
#include <QChar>
#include <QUdpSocket>
#include <QByteArray>
#include <QPointer>
#include <QTime>
#include <QFile>

namespace std {
	class Log;
}

class Log
{
public:
	Log();
	~Log();
	bool CreateLog();
	void WriteLog(QString logMessage);
private:
	QFile m_LogFile;
};

#endif // LOG_H
