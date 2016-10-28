#ifndef LOCOUTILS_H
#define LOCOUTILS_H
#define TO_HEX(i) (i <= 9 ? '0' + i : 'A' - 10 + i)

#define MAX 128

#include "locopacket.h"
#include "section.h"
#include "locotrain.h"
#include "locoblock.h"
#include "locoserial.h"
#include <QProcess>

namespace std {
   class LocoUtils;
}

class LocoUtils  : public QObject
{
    Q_OBJECT

public:
    LocoUtils();
    ~LocoUtils();
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

public slots:
    void do_run();
    void do_sectionOff(int boardNum, int section);
    void do_sectionOn(int boardNum, int section);

signals:
    void trainUpdated(LocoTrain);
    void blockUpdated(LocoBlock);
    void switchUpdated(int _adr, bool _state);
    void occupancyDataReady(QString, bool);
    void querySlot(LocoByte _arg);
};



#endif // LOCOUTILS_H
