#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	qRegisterMetaType<LocoByte>("LocoByte");
	qRegisterMetaType<LocoPacket>("LocoPacket");
	qRegisterMetaType<LocoBlock>("LocoBlock");
	qRegisterMetaType<LocoTrain>("LocoTrain");
    qRegisterMetaType<Section>("Section");
	qRegisterMetaType<QextPortInfo>("QextPortInfo");

	return a.exec();
}
