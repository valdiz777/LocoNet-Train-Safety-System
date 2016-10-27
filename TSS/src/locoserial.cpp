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
        qDebug() << "TESTING SectionOFF HANDLER" << endl;
        //LocoUtils utils; utils.do_sectionOff(1,10);
        LocoUtils utils; utils.do_sectionOn(1,10);
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
    LocoUtils util;

	// This looks so gross >.<
	if (_opCode == "E7") {
        return(util.parse_E7(_packet));
	}
	else if (_opCode == "B2") {
        return(util.parse_B2(_packet));
	}
	else if (_opCode == "85") {
        return(util.parse_85(_packet));
	}
	else if (_opCode == "83") {
        return(util.parse_83(_packet));
	}
	else if (_opCode == "82") {
        return(util.parse_82(_packet));
	}
	else if (_opCode == "81") {
        return(util.parse_81(_packet));
	}
	else if (_opCode == "BF") {
        return(util.parse_BF(_packet));
	}
	else if (_opCode == "BD") {
        return(util.parse_BD(_packet));
	}
	else if (_opCode == "BC") {
        return(util.parse_BC(_packet));
	}
	else if (_opCode == "BB") {
        return(util.parse_BB(_packet));
	}
	else if (_opCode == "BA") {
        return(util.parse_BA(_packet));
	}
	else if (_opCode == "B9") {
        return(util.parse_B9(_packet));
	}
	else if (_opCode == "B8") {
        return(util.parse_B8(_packet));
	}
	else if (_opCode == "B6") {
        return(util.parse_B6(_packet));
	}
	else if (_opCode == "B5") {
        return(util.parse_B5(_packet));
	}
	else if (_opCode == "B4") {
        return(util.parse_B4(_packet));
	}
	else if (_opCode == "B1") {
        return(util.parse_B1(_packet));
	}
	else if (_opCode == "B0") {
        return(util.parse_B0(_packet));
	}
	else if (_opCode == "A2") {
        return(util.parse_A2(_packet));
	}
	else if (_opCode == "A1") {
        return(util.parse_A1(_packet));
	}
	else if (_opCode == "A0") {
        return(util.parse_A0(_packet));
	}
	else if (_opCode == "EF") {
        return(util.parse_EF(_packet));
	}
	else if (_opCode == "E5") {
        return(util.parse_E5(_packet));
	}
	else if (_opCode == "ED") {
        return(util.parse_ED(_packet));
	}

	return ("opcode [" + _opCode + "] doesn't match anything in parser :c");
}

