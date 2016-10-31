#include "trainmonitor.h"
#include <iostream>
#include <QList>
#include <QQueue>
#include <queue>
#include <QString>
#include <QDir>
#include <QFile>

int main()
{
    TrainMonitor train;
    train.generateSectionList();

    QString filename = QDir::currentPath().append("/TrainMonitorTest.txt");
    QFile infile(filename);
    if (!infile.open(QIODevice::ReadOnly | QIODevice::Text)) {

        qFatal("Cannot find input file: %s", filename.toLatin1().constData());
        return 1;
    }

    QTextStream in(&infile);
    while (!in.atEnd())
    {
        QString input = in.readLine();
        train.do_handleOccupancy(input,true);
    }
    infile.close();

}
