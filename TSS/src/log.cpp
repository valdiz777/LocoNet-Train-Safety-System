#include "log.h"

Log::Log()
{

}

Log::~Log()
{

}

/// Creates a new file for each instance of TrainSafety
bool Log::CreateLog()
{
	QDateTime dateTime = dateTime.currentDateTime();
	QString dateTimeString = dateTime.toString("yyyy-MM-dd_hh:mm:ss");
	QString path = "";
	QString LogName = path + dateTimeString + "train_safety.Log";
	//m_LogFile(LogName);

	return true;
}

/// Write a Log message to the TrainSafety Log file
void Log::WriteLog(QString LogMessage)
{
	QDateTime dateTime = dateTime.currentDateTime();
	QString dateTimeString = dateTime.toString("yyyy-MM-dd_hh:mm:ss");
	if (m_LogFile.open(QIODevice::ReadWrite))
	{
		QTextStream stream(&m_LogFile);
		stream << dateTimeString << "\t" << LogMessage << endl;
	}
}
