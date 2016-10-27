#ifndef LOCOSERIAL_H
#define LOCOSERIAL_H

/**
 * Loco Serial Worker Class
 *
 * For threaded usb connections to Digitrax's Loconet
 */

#include <QString>
#include <QDebug>
#include "qextserialport.h"
#include "qextserialenumerator.h"
#include <QByteArray>
#include <QTimer>
#include <QTime>
#include <QPointer>

#include "locopacket.h"
#include "locotrain.h"
#include "locoblock.h"
#include "locoutils.h"

namespace std {
	class LocoSerial;
}

class LocoSerial : public QObject
{
	Q_OBJECT

public:
	LocoSerial();
	~LocoSerial();
	QString timeStamp();

signals:
	void receivedPacket(LocoPacket);
	void writtenPacket(LocoPacket);
	void writtenBytes(QByteArray);
	void droppedPacket();
    //void trainUpdated(LocoTrain);
    //void blockUpdated(LocoBlock);
    //void switchUpdated(int _adr, bool _state);
	void serialOpened();
	void serialClosed();
	void printPacketDesc(QString);
    //void occupancyDataReady(QString, bool);
    //void delayWrite(QByteArray _bytes);

	public slots:
	void do_writePacket(LocoPacket _packet);
	void do_writeBytes(QByteArray _bytes);
    void do_querySlot(LocoByte _slot);
	void do_close();
    bool do_open(QextPortInfo _port);
	void do_read();
    QString parse(LocoPacket _packet);
	void do_run();
	void do_slotScan(LocoByte _slot);
	void do_slotDispatch(LocoByte _slot);
	void do_slotClear(LocoByte _slot);
	void do_slotReq(LocoByte _slot);
	void do_slotUse(LocoByte _slot);
	void do_trackReset();
	void do_trackOn();
	void do_trackOff();

protected:
	void readTimerStop();
	void readTimerStart(int _msec);

	protected slots:

private:
    QPointer<QextSerialPort> usbBuffer;
	QPointer<QTimer> readTimer;
	LocoPacket * incomingPacket;
	LocoPacket * outgoingPacket;
	bool * debug;
    //int * waitForReply;
    //QVector<LocoPacket> * outgoingQueue;
};

#endif // LOCOSERIAL_H







