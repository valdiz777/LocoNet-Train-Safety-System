#include "locoserial.h"
#include <cstdlib>

LocoSerial::LocoSerial()
{
	debug = NULL;
	incomingPacket = NULL;
	outgoingPacket = NULL;
	usbBuffer = NULL;
}

LocoSerial::~LocoSerial()
{
	do_close();
	delete incomingPacket;
	delete outgoingPacket;
	delete debug;
	//usbBuffer->deleteLater();
}

QString LocoSerial::timeStamp()
{
	return(QTime::currentTime().toString("[HH:mm:ss:zzz] "));
}

void LocoSerial::do_run()
{
	incomingPacket = new LocoPacket;
	outgoingPacket = new LocoPacket;
	incomingPacket->clear();
	outgoingPacket->clear();
	debug = new bool;
	*debug = true;
	qDebug() << timeStamp() << "Serial thread initialized.";
}

void LocoSerial::do_writePacket(LocoPacket _packet)
{
	_packet.do_genChecksum();
	do_writeBytes(_packet.get_QByteArray());
	emit writtenPacket(_packet);
}

void LocoSerial::do_writeBytes(QByteArray _bytes)
{
    if (usbBuffer == NULL) {
		return;
	}
	if (!usbBuffer->isOpen())
	{
		return;
	}
	if (*debug) qDebug() << timeStamp() << "Writing to serial: " << _bytes << _bytes.toInt(0, 16);
	usbBuffer->write(_bytes);
	emit writtenBytes(_bytes);
	outgoingPacket->clear();
	outgoingPacket->do_appendByteArray(_bytes);
	parse(*outgoingPacket);
}

void LocoSerial::do_querySlot(LocoByte _slot)
{
	QString _queryText = "BB" + _slot.get_hex() + "00";
	LocoPacket _querySlot(_queryText);
    do_writePacket(_querySlot); // the write() method will generate a checksum for us
}

void LocoSerial::readTimerStart(int _msec)
{
	if (readTimer) {
		readTimerStop();
	}
	readTimer = new QTimer;
	connect(readTimer, SIGNAL(timeout()), this, SLOT(do_read()));
	readTimer->start(_msec);
}

void LocoSerial::readTimerStop()
{
	if (!readTimer) {
		return;
	}
	disconnect(readTimer, 0, 0, 0);
	readTimer->stop();
	readTimer->deleteLater();
}

void LocoSerial::do_close()
{
	readTimerStop();
	if (!usbBuffer) {
		return;
	}
	disconnect(usbBuffer, 0, 0, 0);
	usbBuffer->close();
	emit serialClosed();
	usbBuffer->deleteLater();
}

bool LocoSerial::do_open(QextPortInfo _port)
{
    usbBuffer = new QextSerialPort(_port.portName, QextSerialPort::EventDriven);;
	usbBuffer->close();
    usbBuffer->setBaudRate(BAUD57600);
    usbBuffer->setFlowControl(FLOW_HARDWARE);

	//usbBuffer->open(QIODevice::ReadWrite);
    if (*debug) qDebug() << timeStamp() << "Serial Device being opened: " << _port.portName;
	if (usbBuffer->open(QIODevice::ReadWrite)) //(usbBuffer->isOpen())
	{
		if (*debug) qDebug() << timeStamp() << "Serial port appears to have opened sucessfully.";
		connect(usbBuffer, SIGNAL(readyRead()), this, SLOT(do_read()));
		readTimerStart(20);
		emit serialOpened();
		return(true);
	}

	qDebug() << timeStamp() << "Serial port failed to open.";
	qDebug() << "Serial device error: " << usbBuffer->errorString();
	usbBuffer->close();
	emit serialClosed();
	return(false);
}

void LocoSerial::do_read()
{
	if (!usbBuffer->isOpen())
	{
		return; // not open
	}

	// Read immediately if data is available
	while (usbBuffer->bytesAvailable() > 0)
	{
		//if (*debug) qDebug() << timeStamp() << "Reading byte from serial.";
		LocoByte _byte;
		_byte.set_fromByteArray(usbBuffer->read(1));
		if (!_byte.get_isOP() && incomingPacket->get_size() == 0)
		{
			if (*debug) qDebug() << timeStamp() << "Skipping non opcode with empty incoming packet.";
			continue; // no need to assign anything but an OP to position 0
		}
		if (_byte.get_isOP())
		{
			if (*debug) qDebug() << timeStamp() << "OP code: " << _byte.get_hex();
		}
		if (_byte.get_isOP() && (incomingPacket->get_size() > 0))
		{
			if (*debug) qDebug() << timeStamp() << "Dropping packet to catch incoming op code.";
			emit droppedPacket(); // So this signal is not being handled any where btw....
			incomingPacket->clear(); // it is always followed by a clear on the incoming packet
		}
		incomingPacket->do_appendLocoByte(_byte);
		//incomingPacket->do_appendByteArray(usbBuffer->read(1)); // Load in a byte from the serial buffer
		//if (*debug) qDebug() << timeStamp() << "packet:" << incomingPacket->get_size() << ":" << incomingPacket->get_finalSize();
		if (incomingPacket->get_size() == incomingPacket->get_finalSize() && incomingPacket->get_size() >= 2) // packet is the right size
		{
			if (incomingPacket->validChk()) // packet has a valid checksum, bingo!
			{
				//if (*debug) qDebug() << timeStamp() << "Received a full packet! " << incomingPacket->get_packet();
				emit receivedPacket(*incomingPacket);
				if (*debug) emit printPacketDesc(parse(*incomingPacket));
			}
			else {
				emit droppedPacket();
			}
			incomingPacket->clear(); // doesn't matter if packet has valid checksum here, time to move to the next packet
			continue;
		}
		if (incomingPacket->get_size() > incomingPacket->get_finalSize() && incomingPacket->get_size() >= 2)
		{
			if (*debug) qDebug() << timeStamp() << "Dropping packet for exceeding length stated by op code.";
			emit droppedPacket();
			incomingPacket->clear();
			continue;
		}
	}
}

/*
 * MACROS
 */

void LocoSerial::do_trackOn()
{
	LocoPacket _packet;
	_packet.set_allFromHex("837C");
    do_writePacket(_packet);
}

void LocoSerial::do_trackOff()
{
	LocoPacket _packet;
    _packet.set_allFromHex("827D");
	do_writePacket(_packet);
}

void LocoSerial::do_trackReset()
{
	do_trackOff();
	QTimer::singleShot(10000, this, SLOT(do_trackOn()));
}

void LocoSerial::do_slotScan(LocoByte _slot)
{
	if (_slot.get_decimal() < 1 || _slot.get_decimal() > 119)
	{
		// Invalid slot #
		return;
	}
	LocoPacket _packet;
	//_slot.set_fromHex(QString("%1").arg(_index, 2, 16, QChar('0')));
	_packet.do_appendByte("BB"); // OP code
	_packet.do_appendLocoByte(_slot);
	_packet.do_appendByte("00");
	_packet.do_genChecksum();
	do_writePacket(_packet);
}

void LocoSerial::do_slotClear(LocoByte _slot)
{
	LocoPacket _packet;
	_packet.do_appendByte("B5"); // OP code
	_packet.do_appendLocoByte(_slot);
	_packet.do_appendByte("03");
	_packet.do_genChecksum();
	do_writePacket(_packet);
	//QTimer::singleShot(20, this, SLOT(do_slotScan(_slot)));
	//do_slotScan(_slot);
}

void LocoSerial::do_slotDispatch(LocoByte _slot)
{
	LocoPacket _packet;
	_packet.do_appendByte("BA"); // OP code
	_packet.do_appendLocoByte(_slot);
	_packet.do_appendByte("00");
	_packet.do_genChecksum();
	do_writePacket(_packet);
	//QTimer::singleShot(20, this, SLOT(do_slotScan(_slot)));
	//do_slotScan(_slot);
}

void LocoSerial::do_slotReq(LocoByte _adr)
{
	LocoPacket _packet;
	_packet.do_appendByte("BF"); // OP code
	_packet.do_appendByte("00");
	_packet.do_appendLocoByte(_adr);
	_packet.do_genChecksum();
	do_writePacket(_packet);
}

void LocoSerial::do_slotUse(LocoByte _slot)
{
	LocoPacket _packet;
	_packet.do_appendByte("BA"); // OP code
	_packet.do_appendLocoByte(_slot);
	_packet.do_appendLocoByte(_slot);
	_packet.do_genChecksum();
	do_writePacket(_packet);
}

void LocoSerial::do_closeTurnout(int locoTurnout)
{
    QString on = "B0" + QString::number( locoTurnout-1, 16 ) + "30";
    QString off = "B0" + QString::number( locoTurnout-1, 16 ) + "20";
    LocoPacket outputOn;
    outputOn.set_allFromHex(on);
    LocoPacket outputOff;
    outputOff.set_allFromHex(off);
    do_writePacket(outputOn);
    do_writePacket(outputOff);
}

void LocoSerial::do_throwTurnout(int locoTurnout)
{
    QString on = "B0" + QString::number( locoTurnout-1, 16 ) + "10";
    QString off = "B0" + QString::number( locoTurnout-1, 16 ) + "00";
    LocoPacket outputOn;
    outputOn.set_allFromHex(on);
    LocoPacket outputOff;
    outputOff.set_allFromHex(off);
    do_writePacket(outputOn);
    do_writePacket(outputOff);
}



QString LocoSerial::parse(LocoPacket _packet)
{
	QString _opCode = (_packet.get_OPcode());

	if (!_packet.hasOP() || !_packet.validChk()) {
		return("packet is malformed >.<");
	}

	// This looks so gross >.<
	if (_opCode == "E7") {
        return(parse_E7(_packet));
	}
	else if (_opCode == "B2") {
        return(parse_B2(_packet));
	}
	else if (_opCode == "85") {
        return(parse_85(_packet));
	}
	else if (_opCode == "83") {
        return(parse_83(_packet));
	}
	else if (_opCode == "82") {
        return(parse_82(_packet));
	}
	else if (_opCode == "81") {
        return(parse_81(_packet));
	}
	else if (_opCode == "BF") {
        return(parse_BF(_packet));
	}
	else if (_opCode == "BD") {
        return(parse_BD(_packet));
	}
	else if (_opCode == "BC") {
        return(parse_BC(_packet));
	}
	else if (_opCode == "BB") {
        return(parse_BB(_packet));
	}
	else if (_opCode == "BA") {
        return(parse_BA(_packet));
	}
	else if (_opCode == "B9") {
        return(parse_B9(_packet));
	}
	else if (_opCode == "B8") {
        return(parse_B8(_packet));
	}
	else if (_opCode == "B6") {
        return(parse_B6(_packet));
	}
	else if (_opCode == "B5") {
        return(parse_B5(_packet));
	}
	else if (_opCode == "B4") {
        return(parse_B4(_packet));
	}
	else if (_opCode == "B1") {
        return(parse_B1(_packet));
	}
	else if (_opCode == "B0") {
        return(parse_B0(_packet));
	}
	else if (_opCode == "A2") {
        return(parse_A2(_packet));
	}
	else if (_opCode == "A1") {
        return(parse_A1(_packet));
	}
	else if (_opCode == "A0") {
        return(parse_A0(_packet));
	}
	else if (_opCode == "EF") {
        return(parse_EF(_packet));
	}
	else if (_opCode == "E5") {
        return(parse_E5(_packet));
	}
	else if (_opCode == "ED") {
        return(parse_ED(_packet));
	}

	return ("opcode [" + _opCode + "] doesn't match anything in parser :c");
}

/*
 * Packet related functions
 */
QString LocoSerial::parse_E7(LocoPacket _packet)
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

    emit trainUpdated(_newTrain);
    _description.append(" Speed: " + _speed.get_hex() + " Slot: " + _slot.get_hex() + " Direction: " + QString::number(_dir) + " State: " + _state);

    if (_description == "E7:") {
        _description.append(" No action taken?");
    }
    return (_description);
}

QString LocoSerial::parse_EF(LocoPacket _packet) {
    QString _description = "EF: Write slot data.";
    return(_description);
}

QString LocoSerial::parse_E5(LocoPacket _packet) {
    QString _description = "E5: Move 8 bytes Peer to Peer.";
    return(_description);
}

QString LocoSerial::parse_ED(LocoPacket _packet) {
    QString _description = "ED: Send n-byte packet.";
    return(_description);
}

QString LocoSerial::parse_85(LocoPacket _packet) {
    QString _description = "85: Requesting track state IDLE / EMG STOP.";
    return(_description);
}

QString LocoSerial::parse_83(LocoPacket _packet) {
    QString _description = "83: Requesting track state ON.";
    return(_description);
}

QString LocoSerial::parse_82(LocoPacket _packet) {
    QString _description = "82: Requesting track state OFF.";
    return(_description);
}

QString LocoSerial::parse_81(LocoPacket _packet) {
    QString _description = "81: MASTER sent BUSY code.";
    return(_description);
}

QString LocoSerial::parse_BF(LocoPacket _packet) {
    QString _description = "BF: Requesting locomotive address.";
    return(_description);
}

QString LocoSerial::parse_BD(LocoPacket _packet) {
    QString _description = "BD: Requesting switch with LACK function."; // LACK - Long ACKnowledge
    return(_description);
}

QString LocoSerial::parse_BC(LocoPacket _packet) {
    QString _description = "BC: Requesting state of switch.";
    return(_description);
}

QString LocoSerial::parse_BB(LocoPacket _packet) {
    QString _description = "BB: Requesting SLOT data/status block.";
    return(_description);
}

QString LocoSerial::parse_BA(LocoPacket _packet) {
    QString _description = "BA: Move slot SRC to DEST.";
    return(_description);
}

QString LocoSerial::parse_B9(LocoPacket _packet) {
    QString _description = "B9: Link slot ARG1 to slot ARG2.";
    return(_description);
}

QString LocoSerial::parse_B8(LocoPacket _packet) {
    QString _description = "B8: Unlink slot ARG1 from slot ARG2.";
    return(_description);
}

QString LocoSerial::parse_B6(LocoPacket _packet) {
    QString _description = "B6: Set FUNC bits in a CONSIST uplink element.";
    return(_description);
}

QString LocoSerial::parse_B5(LocoPacket _packet) {
    QString _description = "B5: Write slot stat1.";
    return(_description);
}

QString LocoSerial::parse_B4(LocoPacket _packet) {
    QString _description = "B4: Long Acknowledge - LACK.";
    return(_description);
}

QString LocoSerial::parse_B2(LocoPacket _packet)
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


    emit blockUpdated(_newBlock);
    qDebug() << "emitting occupancyDataReady" << endl;
    emit occupancyDataReady(trackSection, _occupied);
    return(_description);
}

QString LocoSerial::parse_B1(LocoPacket _packet) {
    QString _description = "B1: Turnout sensor state report.";
    return(_description);
}

QString LocoSerial::parse_B0(LocoPacket _packet) {
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


    emit switchUpdated(_swch, _state);
    return(_description);
}

QString LocoSerial::LocoSerial::parse_A2(LocoPacket _packet) {
    QString _description = "A2: Setting slot sound functions.";
    return(_description);
}

QString LocoSerial::LocoSerial::parse_A1(LocoPacket _packet) {
    QString _description = "A1: Setting slot direction.";
    return(_description);
}

QString LocoSerial::LocoSerial::parse_A0(LocoPacket _packet) {
    QString _description = "A0: Setting slot speed.";
    LocoByte _arg1 = _packet.get_locobyte(1);
    LocoByte _arg2 = _packet.get_locobyte(2);

    emit querySlot(_arg1); // Ask for E7 slot data
    return(_description);
}


int LocoSerial::getTimeDiff()
{
    int temp = QTime::currentTime().toString("zzz").toInt();
    int result = abs((last_time -  temp));
    last_time = temp;

    return result;
}

void LocoSerial::do_sectionOff(int boardNum, int section)
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

void LocoSerial::do_sectionOn(int boardNum, int section)
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
