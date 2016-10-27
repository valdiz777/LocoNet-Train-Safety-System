#include "locoutils.h"


/*
 * Default Contructor
 */
LocoUtils::LocoUtils()
{
    /********************************************
        So la da da di we like to party
        Dancing with Molly
        Doing whatever we want,
        This is our house
        This is our rules
        And we can't stop (whoa)
        And we won't stop (whoa)
        Can't you see it's we who own the night
        Can't you see it we who bout' that life
        And we can't stop (whoa)
        And we won't stop (whoa)
        We run things, things don't run we
        Don't take nothing from nobody yeah yeah
    ************************************************/
}

/*
 * Default Destructor
 */
LocoUtils::~LocoUtils()
{
    // Let QT take care of it
}


/*
 * Packet related functions
 */
QString LocoUtils::parse_E7(LocoPacket _packet)
{
    QString _description = "E7:";
    // Parse packet into usable variables
    bool _busy = _packet.get_locobyte(3).get_oneBit(2);
    bool _active = _packet.get_locobyte(3).get_oneBit(3);
    LocoByte _slot = _packet.get_locobyte(2);
    LocoByte _adr = _packet.get_locobyte(4);
    LocoByte _speed = _packet.get_locobyte(5);
    bool _dir = _packet.get_locobyte(6).get_oneBit(2);

    LocoTrain _newTrain;
    _newTrain.set_adr(_adr);
    _newTrain.set_reverse(_dir);
    _newTrain.set_slot(_slot);
    _newTrain.set_speed(_speed);
    QString _state = "NULL";
    if (_busy && _active) {
        _state = "IN_USE";
    }
    else if (_busy && !_active) {
        _state = "IDLE";
    }
    else if (!_busy && _active) {
        _state = "COMMON";
    }
    else {
        _state = "FREE";
    }
    _newTrain.set_state(_state);

    LocoUtils emitter;
    emitter.trainUpdated(_newTrain);
    _description.append(" Speed: " + _speed.get_hex() + " Slot: " + _slot.get_hex() + " Direction: " + QString::number(_dir) + " State: " + _state);

    if (_description == "E7:") {
        _description.append(" No action taken?");
    }
    return (_description);
}

QString LocoUtils::parse_EF(LocoPacket _packet) {
    QString _description = "EF: Write slot data.";
    return(_description);
}

QString LocoUtils::parse_E5(LocoPacket _packet) {
    QString _description = "E5: Move 8 bytes Peer to Peer.";
    return(_description);
}

QString LocoUtils::parse_ED(LocoPacket _packet) {
    QString _description = "ED: Send n-byte packet.";
    return(_description);
}

QString LocoUtils::parse_85(LocoPacket _packet) {
    QString _description = "85: Requesting track state IDLE / EMG STOP.";
    return(_description);
}

QString LocoUtils::parse_83(LocoPacket _packet) {
    QString _description = "83: Requesting track state ON.";
    return(_description);
}

QString LocoUtils::parse_82(LocoPacket _packet) {
    QString _description = "82: Requesting track state OFF.";
    return(_description);
}

QString LocoUtils::parse_81(LocoPacket _packet) {
    QString _description = "81: MASTER sent BUSY code.";
    return(_description);
}

QString LocoUtils::parse_BF(LocoPacket _packet) {
    QString _description = "BF: Requesting locomotive address.";
    return(_description);
}

QString LocoUtils::parse_BD(LocoPacket _packet) {
    QString _description = "BD: Requesting switch with LACK function."; // LACK - Long ACKnowledge
    return(_description);
}

QString LocoUtils::parse_BC(LocoPacket _packet) {
    QString _description = "BC: Requesting state of switch.";
    return(_description);
}

QString LocoUtils::parse_BB(LocoPacket _packet) {
    QString _description = "BB: Requesting SLOT data/status block.";
    return(_description);
}

QString LocoUtils::parse_BA(LocoPacket _packet) {
    QString _description = "BA: Move slot SRC to DEST.";
    return(_description);
}

QString LocoUtils::parse_B9(LocoPacket _packet) {
    QString _description = "B9: Link slot ARG1 to slot ARG2.";
    return(_description);
}

QString LocoUtils::parse_B8(LocoPacket _packet) {
    QString _description = "B8: Unlink slot ARG1 from slot ARG2.";
    return(_description);
}

QString LocoUtils::parse_B6(LocoPacket _packet) {
    QString _description = "B6: Set FUNC bits in a CONSIST uplink element.";
    return(_description);
}

QString LocoUtils::parse_B5(LocoPacket _packet) {
    QString _description = "B5: Write slot stat1.";
    return(_description);
}

QString LocoUtils::parse_B4(LocoPacket _packet) {
    QString _description = "B4: Long Acknowledge - LACK.";
    return(_description);
}

QString LocoUtils::parse_B2(LocoPacket _packet)
{
    QString _description = "B2:";
    LocoByte _adr1 = _packet.get_locobyte(1);
    LocoByte _adr2 = _packet.get_locobyte(2);
    bool _aux = _adr2.get_qBitArray()[2];
    bool _occupied;

    int _loco_sensor = ((_packet.get_sensoraddr(_adr1, _adr2) - 1) * 2 + ((_adr2.get_decimal() & constants::OPC_INPUT_REP_SW) != 0 ? 2 : 1));

    int _section = ((_loco_sensor + 16 - 1) / 16) % 10;
    _section = (_section == 0) ? 16 : _section;
    int _bdNum = (_loco_sensor % 16);
    _bdNum = (_bdNum == 0) ? 16 : _bdNum;

    QString _address = QString::number(_loco_sensor);




    QString trackSection = QString::number(_section) + "-" + QString::number(_bdNum);


    if ((_adr2.get_decimal() & constants::OPC_INPUT_REP_HI) != 0) {

        _description.append(" LS" + QString::number(_loco_sensor) + ": OCCUPANCY AT: " + QString::number(_section) + "-" + QString::number(_bdNum));
        _occupied = true;
    }
    else
    {
        _description.append(" LS" + QString::number(_loco_sensor) + ": VACANCY AT: " + trackSection);
        _occupied = false;
    }

    LocoBlock _newBlock(_address, _aux, _occupied);
    LocoUtils emitter;
    if (emitter.getTimeDiff() > 100) {

        emitter.blockUpdated(_newBlock);
        emitter.occupancyDataReady(trackSection, _occupied);
    }

    return(_description);
}

QString LocoUtils::parse_B1(LocoPacket _packet) {
    QString _description = "B1: Turnout sensor state report.";
    return(_description);
}

QString LocoUtils::parse_B0(LocoPacket _packet) {
    QString _description = "B0:";
    LocoByte _arg1 = _packet.get_locobyte(1);
    LocoByte _arg2 = _packet.get_locobyte(2);
    bool _state = _arg2.get_qBitArray()[2];


    int _swch = _arg1.get_decimal();
    _swch++;

    if(!_state)
    {
        _description.append(" The Switch: " + QString::number(_swch) + " is thrown ");
    }
    else {
        _description.append(" The Switch: " + QString::number(_swch) + " is closed ");
    }

    QByteArray _adr;
    _adr.append(_arg2.get_hex().mid(1, 1)); // Load MS byte of address
    _adr.append(_arg1.get_hex()); // Load LS 2 bytes of address

    LocoUtils emitter;
    emitter.switchUpdated(_swch, _state);
    return(_description);
}

QString LocoUtils::LocoUtils::parse_A2(LocoPacket _packet) {
    QString _description = "A2: Setting slot sound functions.";
    return(_description);
}

QString LocoUtils::LocoUtils::parse_A1(LocoPacket _packet) {
    QString _description = "A1: Setting slot direction.";
    return(_description);
}

QString LocoUtils::LocoUtils::parse_A0(LocoPacket _packet) {
    QString _description = "A0: Setting slot speed.";
    LocoByte _arg1 = _packet.get_locobyte(1);
    LocoByte _arg2 = _packet.get_locobyte(2);
    LocoUtils emitter;
    emitter.querySlot(_arg1); // Ask for E7 slot data
    return(_description);
}


int LocoUtils::getTimeDiff()
{
    LocoUtils util;
    int temp = QTime::currentTime().toString("zzz").toInt();
    int result = abs((util.last_time -  temp));
    util.last_time = temp;

    return result;
}

void LocoUtils::do_sectionOff(int boardNum, int section)
{
    bool _isStal = true;

    // Prepare message and check if star or stal
    switch (boardNum)
    {
    case 5: {
        boardNum = 1;
        _isStal = false;
        break;
    }
    case 6: {
        boardNum = 2;
        _isStal = false;
        break;
    }
    case 7: {
        boardNum = 3;
        _isStal = false;
        break;
    }
    case 8: {
        boardNum = 4;
        _isStal = false;
        break;
    }

    }
    QString _echocmd = "echo " + QString::number(boardNum)+ QString(QChar(TO_HEX(section-1)))+"0";

    // Send message
    echo.setStandardOutputProcess(&netcat);
    echo.start(_echocmd);
    if (_isStal) {
        netcat.start("netcat " + stal);
    } else {
        netcat.start("netcat " + star);
    }

    netcat.setProcessChannelMode(QProcess::ForwardedChannels);

    // Wait for it to start
    if(!echo.waitForStarted())
        qFatal("Failed to turn off section, echo process not working\n");

    bool retval = false;
    QByteArray buffer;
    while ((retval = netcat.waitForFinished()));
        buffer.append(netcat.readAll());
    qDebug() << "Section Off Output: " << buffer;

}

void LocoUtils::do_sectionOn(int boardNum, int section)
{
    bool _isStal = true;

    // Prepare message and check if star or stal
    switch (boardNum)
    {
    case 5: {
        boardNum = 1;
        _isStal = false;
        break;
    }
    case 6: {
        boardNum = 2;
        _isStal = false;
        break;
    }
    case 7: {
        boardNum = 3;
        _isStal = false;
        break;
    }
    case 8: {
        boardNum = 4;
        _isStal = false;
        break;
    }

    }
    QString _echocmd = "echo " + QString::number(boardNum)+ QString(QChar(TO_HEX(section-1)))+"1";

    // Send message
    echo.setStandardOutputProcess(&netcat);
    echo.start(_echocmd);
    if (_isStal) {
        netcat.start("netcat " + stal);
    } else {
        netcat.start("netcat " + star);
    }

    netcat.setProcessChannelMode(QProcess::ForwardedChannels);

    // Wait for it to start
    if(!echo.waitForStarted())
        qFatal("Failed to turn off section, echo process not working\n");

    bool retval = false;
    QByteArray buffer;
    while ((retval = netcat.waitForFinished()));
        buffer.append(netcat.readAll());
    qDebug() << "Section Off Output: " << buffer;

}
