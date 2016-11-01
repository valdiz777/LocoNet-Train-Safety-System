#include "locopacket.h"
#include <QDir>

/* Class: LocoPacket
 *
 * For creating, manipulating, and extracting LocoNet Packets
 *
 * Christopher Bero [csb0019@uah.edu], Valentine Nwachukwu [valdiz777@gmail.com]
 * Team 4A, Train Safety Team
 */

 /* Static Members */
bool LocoPacket::debug = false;

/*
 * Default Constructor
 */
LocoPacket::LocoPacket()
{
	clear();
    do_loadOpcodes();
}

/*
 * Overloaded Constructor
 */
LocoPacket::LocoPacket(QString _hex)
{
	_hex = _hex.remove(" ");
	_hex = _hex.remove(":");

	if ((_hex.count() % 2) != 0)
	{
		qDebug() << timeStamp() << "Hex packet is malformed! D:";
		locobyte_array.clear();
		return;
	}
	clear();
	set_allFromHex(_hex);
    do_loadOpcodes();
}

LocoPacket::LocoPacket(QByteArray _bytearray)
{
	clear();
	do_appendByteArray(_bytearray);
    do_loadOpcodes();
}

/*
 * Default Destructor
 */
LocoPacket::~LocoPacket()
{
	//
}

QString LocoPacket::timeStamp()
{
	return(QTime::currentTime().toString("[HH:mm:ss:zzz] "));
}

void LocoPacket::clear()
{
	locobyte_array.clear();
}

bool LocoPacket::do_loadOpcodes()
{

    QString filename = ":/config/packets";
    QFile infile(filename);
    if (!infile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Cannot find input file:" << filename;
        qDebug() << timeStamp() << "Unable to open packets config file.";
        return(false);
    }
    QTextStream in(&infile);
    while (!in.atEnd())
    {
        QStringList splitString = in.readLine().split(",");
        this->opcodes.insert(splitString[0], splitString[1]);
    }

    infile.close();
    if (debug) qDebug() << timeStamp() << "Done loading opccodes.";
	return(true);
}

/* set_allFromHex()
 *
 * Define binary, hex, and bits from a string
 */
void LocoPacket::set_allFromHex(QString _hex)
{
	_hex = _hex.remove(" ");
	_hex = _hex.remove(":");
	_hex = _hex.remove("-");
	_hex = _hex.remove(",");

	if ((_hex.count() % 2) != 0)
	{
		if (debug) qDebug() << timeStamp() << "Hex packet is malformed! D:";
		return;
	}
	int _length = _hex.count() / 2;
	locobyte_array.clear();
	for (int _packet = 0; _packet < _length; ++_packet)
	{
		LocoByte _tmp_locohex(_hex.mid((_packet * 2), 2));
		locobyte_array.append(_tmp_locohex);
		//if (debug) qDebug() << timeStamp() << "New packet: " << locobyte_array[_packet].get_hex() << " " << locobyte_array[_packet].get_binary();
	}
	if (hasOP())
	{
		if (debug) qDebug() << timeStamp() << "Valid OP code x)";
	}
	if (validChk())
	{
		if (debug) qDebug() << timeStamp() << "Valid Checksum x)";
	}
}

bool LocoPacket::operator ==(LocoPacket _arg)
{
	return(this->get_QByteArray() == _arg.get_QByteArray());
}

QString LocoPacket::get_packet()
{
	QString _result = "";
	for (int _index = 0; _index < locobyte_array.size(); ++_index)
	{
		_result.append(locobyte_array[_index].get_hex());
	}
	return (_result);
}

int LocoPacket::get_finalSize()
{
	int _result = -1;
	if (locobyte_array.count() >= 1)
	{
		_result = (locobyte_array[0].get_packetLength());
	}
	if (_result == 0 && locobyte_array.count() >= 2)
	{
		// N byte packet, check next section for count
		_result = (locobyte_array[1].get_packetLength());
	}
	return(_result);
}

int LocoPacket::get_size()
{
	return(locobyte_array.count());
}

QString LocoPacket::get_OPcode()
{
	if (locobyte_array.count() < 1)
	{
		return("00");
	}
	return(locobyte_array.first().get_hex());
}

LocoByte LocoPacket::get_locobyte(int _byte)
{
	if (_byte >= locobyte_array.count())
	{
		return LocoByte("FF");
	}
	return locobyte_array[_byte];
}

QString LocoPacket::do_xor(LocoByte _byte1, LocoByte _byte2)
{
	if (debug) qDebug() << timeStamp() << "doXor() ";
	LocoByte _result;
	for (short unsigned int _bit = 0; _bit < 8; ++_bit)
	{
		if (debug) qDebug() << timeStamp() << "doxor: " << _bit << " - " << _byte1.get_oneBit(_bit) << " : " << _byte2.get_oneBit(_bit);

		if (_byte1.get_oneBit(_bit) == 0 && _byte2.get_oneBit(_bit) == 1) {
			_result.set_oneBit(_bit, 1);
		}
		else if (_byte1.get_oneBit(_bit) == 1 && _byte2.get_oneBit(_bit) == 0) {
			_result.set_oneBit(_bit, 1);
		}
		else if (_byte1.get_oneBit(_bit) == 1 && _byte2.get_oneBit(_bit) == 1) {
			_result.set_oneBit(_bit, 0);
		}
		else if (_byte1.get_oneBit(_bit) == 0 && _byte2.get_oneBit(_bit) == 0) {
			_result.set_oneBit(_bit, 0);
		}
	}
	if (debug) qDebug() << timeStamp() << "end doXor: hex: " << _result.get_hex();
	return (_result.get_hex());
}

QString LocoPacket::do_genChecksum()
{
	if (validChk() && hasOP())
	{
		if (debug) qDebug() << timeStamp() << "Not generating a checksum for an already valid packet ^-^";
		QString _chk = "";
		_chk.append(locobyte_array[locobyte_array.size() - 1].get_hex());
		return(_chk);
	}
	LocoByte _checksum;
	for (int _index = 0; _index < locobyte_array.size(); ++_index)
	{
		if (debug) qDebug() << timeStamp() << "hexIndex: " << _index;
		if (debug) qDebug() << timeStamp() << "genChecksum: " << locobyte_array[_index].get_hex() << " : " << locobyte_array[_index].get_binary() << " and: " << _checksum.get_hex() << " : " << _checksum.get_binary();
		_checksum.set_fromHex(do_xor(_checksum, locobyte_array[_index]));
	}
	_checksum.do_genComplement();
	locobyte_array.append(_checksum);
	return(_checksum.get_hex());
}

/* do_appendByte()
 *
 * Will append a new byte to the end of the packet, replacing the checksum if it exists
 */
void LocoPacket::do_appendByte(QString _byte)
{
	LocoByte _newByte(_byte);
	if (validChk())
	{
		locobyte_array.removeLast();
		locobyte_array.append(_newByte);
	}
	else {
		locobyte_array.append(_newByte);
	}
}

void LocoPacket::do_appendByteArray(QByteArray _byteArray)
{
	for (int _index = 0; _index < _byteArray.count(); ++_index)
	{
		LocoByte _newByte;
		_newByte.set_fromByteArray(_byteArray/*.mid(_index, 1)*/);
		locobyte_array.append(_newByte);
	}
}

void LocoPacket::do_appendLocoByte(LocoByte _byte)
{
	locobyte_array.append(_byte);
}

/* get_validChk()
 *
 * Returns whether the packet has a valid OP code
 */
bool LocoPacket::validChk()
{
	LocoByte _hexHolder;
	for (int _index = 0; _index < locobyte_array.size(); ++_index)
	{
		if (debug) qDebug() << timeStamp() << "hexIndex: " << _index;
		if (debug) qDebug() << timeStamp() << "validPacket: working: " << locobyte_array[_index].get_hex() << " : " << locobyte_array[_index].get_binary() << " and: " << _hexHolder.get_hex() << " : " << _hexHolder.get_binary();
		LocoByte tmp = _hexHolder;
		_hexHolder.set_fromHex(do_xor(tmp, locobyte_array[_index]));
	}
	//qDebug() << timeStamp() << "checksum: " << _hexHolder.get_hex();
	if (_hexHolder.get_hex() == "FF")
	{
		return(true);
	}
	return(false);
}

bool LocoPacket::hasOP()
{
	return(locobyte_array[0].get_isOP());
}

bool LocoPacket::get_isEmpty() {
	if (locobyte_array.size() > 0) {
		return false;
	}
	return true;
}

// Check for valid OP code in sqlite
bool LocoPacket::validOP()
{
	if (get_isEmpty()) {
        return(false);
	}
    if (opcodes.isEmpty())
	{
        bool _status = do_loadOpcodes();
        if (!_status)
        {
            return(false); // Can't determine validity
        }
	}
	QString _op = locobyte_array[0].get_hex();
	if (debug) qDebug() << timeStamp() << "Checking opcodes database for " << _op;
    bool found = false;
    for (QString opcode : opcodes.keys())
    {
        if (_op == opcode) found = true;

    }

    if (false)
    {
        if (debug) qDebug() << timeStamp() << "Query to find valid OP codes failed.";
        return(false);
    }
	else {
        if (debug) qDebug() << timeStamp() << "OPCODE:" << _op << " was found in the dataset.";
        return(true);
    }

	return(false);
}

QVector<QString> LocoPacket::get_Opcodes()
{
    QVector<QString> _opcodes;
    for (QString opcode : this->opcodes.keys())
    {
        _opcodes.append(opcode);
    }
	return(_opcodes);
}

QVector<QString> LocoPacket::get_Names()
{
	QVector<QString> _names;
    for (QString name : this->opcodes.values())
    {
        _names.append(name);
    }
	return(_names);
}

bool LocoPacket::is_followOnMsg()
{
	return (locobyte_array[0].get_hasFollowMsg());
}

QBitArray LocoPacket::get_QBitArray()
{
	int _bytes = locobyte_array.count();
	QBitArray _bitArray = QBitArray(_bytes * 8);
	for (int _byteIndex = 0; _byteIndex < _bytes; ++_byteIndex)
	{
		for (int _bitIndex = 0; _bitIndex < 8; ++_bitIndex)
		{
			int _pos = (_byteIndex * 8) + _bitIndex;
			_bitArray[_pos] = locobyte_array[_byteIndex].get_oneBit(_bitIndex);
		}
	}
	return _bitArray;
}

QByteArray LocoPacket::get_QByteArray()
{
	QByteArray _byteArray;
	QBitArray _bitArray = get_QBitArray();
	short int _bytes = locobyte_array.count();
	for (short int _byteIndex = 0; _byteIndex < _bytes; ++_byteIndex)
	{
		for (int _bitIndex = 0; _bitIndex < 8; ++_bitIndex)
		{
			int _pos = (_byteIndex * 8) + (7 - _bitIndex); // This is disgusting :|
			_byteArray[_byteIndex] = (_byteArray[_byteIndex] | (_bitArray[_pos] ? 1 : 0) << (_bitIndex));
		}
	}
	return (_byteArray);
}

/**
 * Convert LocoNet packet into a locomotive address.
 *
 * @return 1-4096 address
 */
int LocoPacket::get_locoaddr()
{
	return get_locoaddr(locobyte_array[1], locobyte_array[2]);
}

/**
 * Convert bytes from LocoNet packet into a locomotive address.
 *
 * @param byte1
 *            Byte containing the upper bits.
 * @param byte2
 *            Byte containting the lower bits.
 * @return 1-4096 address
 */
int LocoPacket::get_locoaddr(LocoByte byte1, LocoByte byte2)
{
	return (((byte1.get_decimal() & 0x7f) * 128) + (byte2.get_decimal() & 0x7f));
}

/**
 * Convert LocoNet packet into a 1-based address for a sensor or
 * turnout.
 *
 * @return 1-4096 address
 */
int LocoPacket::get_sensoraddr()
{
	return get_sensoraddr(locobyte_array[1], locobyte_array[2]);
}

/**
 * Convert bytes from LocoNet packet into a 1-based address for a sensor or
 * turnout.
 *
 * @param byte1
 *            Byte containing the upper bits
 * @param byte2
 *            Byte containing the lower bits
 * @return 1-4096 address
 */
int LocoPacket::get_sensoraddr(LocoByte byte1, LocoByte byte2)
{
	return (((byte2.get_decimal() & 0x0f) * 128) + (byte1.get_decimal() & 0x7f)) + 1;
}

/**
 * Sometimes I wonder /
 * Whether my code should wander /
 * Those tretcherous glades /
 */










