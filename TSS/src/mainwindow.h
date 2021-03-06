#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include "qextserialenumerator.h"
#include <QDebug>
#include <QByteArray>
#include <QVariant>
#include <QComboBox>
#include <QIODevice>
#include <QTimer>
#include <QTime>
#include <QThread>
#include<QMessageBox>


#include "locoblock.h"
#include "locobyte.h"
#include "locotrain.h"
#include "locopacket.h"
#include "locoserial.h"
#include "trainmonitor.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	QString timeStamp();
	static bool debug;

public slots:
	void do_enableArgs();
	void do_genPacket();
	void do_refreshSerialList();
	void do_openSerial();
    void do_sendSerial();
    void do_packetReceived(LocoPacket);
	void do_bytesWritten(QByteArray);
	void do_printDescriptions(QString);
	void do_loadOPComboBox();
	void do_OPfromComboBox();
    void do_showCollisionEvt(QStringList collisionSections);
    void do_showSystemReady();
    void do_clearPairs();


	void handle_serialOpened();
	void handle_serialClosed();
    void msgBoxClosed(QAbstractButton *button);
signals:
	void locoserial_open(QextPortInfo _device);
	void locoserial_write(LocoPacket outgoingPacket);
	void nodeOn(QString node);
	void nodeOff(QString node);

private:
	Ui::MainWindow *ui;
	LocoPacket outgoingPacket;
	LocoPacket incomingPacket;
	QextPortInfo usbPorts;
	LocoSerial * locoserial;
	QThread threadSerial;
	TrainMonitor * trainmonitor;
	QThread threadMonitor;
	QList<Section> m_sectionList;
};

#endif // MAINWINDOW_H
