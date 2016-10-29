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
#include <QProcess>

#include "locopacket.h"
#include "locotrain.h"
#include "locoblock.h"

#define TO_HEX(i) (i <= 9 ? '0' + i : 'A' - 10 + i)

#define MAX 128

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
    QString parse_E7(LocoPacket _packet);
    QString parse_EF(LocoPacket _packet);
    QString parse_E5(LocoPacket _packet);
    QString parse_ED(LocoPacket _packet);
    QString parse_B2(LocoPacket _packet);
    QString parse_85(LocoPacket _packet);
    QString parse_83(LocoPacket _packet);
    QString parse_82(LocoPacket _packet);
    QString parse_81(LocoPacket _packet);
    QString parse_BF(LocoPacket _packet);
    QString parse_BD(LocoPacket _packet);
    QString parse_BC(LocoPacket _packet);
    QString parse_BB(LocoPacket _packet);
    QString parse_BA(LocoPacket _packet);
    QString parse_B9(LocoPacket _packet);
    QString parse_B8(LocoPacket _packet);
    QString parse_B6(LocoPacket _packet);
    QString parse_B5(LocoPacket _packet);
    QString parse_B4(LocoPacket _packet);
    QString parse_B1(LocoPacket _packet);
    QString parse_B0(LocoPacket _packet);
    QString parse_A2(LocoPacket _packet);
    QString parse_A1(LocoPacket _packet);
    QString parse_A0(LocoPacket _packet);
    int getTimeDiff();
    int last_time = 0;
    QString stal = "172.21.0.63 23";
    QString star = "172.21.0.64 23";
    QProcess echo;
    QProcess netcat;

    int getSectionOffCmds() const;
    void setSectionOffCmds(int value);

    int getSectionOnCmds() const;
    void setSectionOnCmds(int value);

signals:
    void receivedPacket(LocoPacket);
    void writtenPacket(LocoPacket);
	void writtenBytes(QByteArray);
	void droppedPacket();
	void serialOpened();
	void serialClosed();
	void printPacketDesc(QString);
    void trainUpdated(LocoTrain);
    void blockUpdated(LocoBlock);
    void switchUpdated(int _adr, bool _state);
    void occupancyDataReady(QString, bool);
    void querySlot(LocoByte _arg);

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
    void do_closeTurnout(int locoTurnout);
    void do_throwTurnout(int locoTurnout);
    void do_sectionOff(int boardNum, int section);
    void do_sectionOn(int boardNum, int section);
    void do_clearSectionOff();
    void do_clearSectionOn();
    void do_getSectionsOff();
    void do_getSectionsOn();
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
    int sectionOnCmds = 0;
    int sectionOffCmds = 0;
    //int * waitForReply;
    //QVector<LocoPacket> * outgoingQueue;
};

#endif // LOCOSERIAL_H







