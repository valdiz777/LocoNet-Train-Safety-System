#ifndef LOCOPACKET_H
#define LOCOPACKET_H

#include <QString>
#include <QDebug>
#include <QVector>
#include <QByteArray>
#include <QBitArray>
#include <QTime>

#include "locobyte.h"
//#include "locotrain.h"
//#include "locoblock.h"

/* LocoPacket()
 *
 * locohex: holds one byte of hex
 * locopacket: holds representation of one loconet packet
 * Checksum = Chk
 */

namespace constants
{
	const int OPC_INPUT_REP_SW = 32;
	const int OPC_INPUT_REP_HI = 16;
} // namespace constants

namespace std {
	class LocoPacket;
}

class LocoPacket
{
public:
	LocoPacket();
	LocoPacket(QString _hex);
	LocoPacket(QByteArray _bytearray);
	~LocoPacket();
	QString timeStamp();
	bool operator==(LocoPacket _arg);
	QString get_packet();
	QByteArray get_QByteArray();
	QBitArray get_QBitArray();
	int get_finalSize();
	int get_size();
    QString get_OPcode();
	LocoByte get_locobyte(int _byte);
	int get_locoaddr();
	int get_locoaddr(LocoByte byte1, LocoByte byte2);
	int get_sensoraddr();
	int get_sensoraddr(LocoByte byte1, LocoByte byte2);
	bool get_isEmpty();
    QVector<QString> get_Opcodes();
    QVector<QString> get_Names();
	void set_allFromHex(QString _hex);
	QString do_genChecksum();
	void do_appendByte(QString _byte);
	void do_appendByteArray(QByteArray _byteArray);
	void do_appendLocoByte(LocoByte _byte);
	bool validChk();
	bool hasOP(); // Returns that first byte is OP formatted
	bool validOP(); // Returns that first byte is OP and in our database
	bool is_followOnMsg();
	void clear();
	static bool debug;

protected:
	QString do_xor(LocoByte _byte1, LocoByte _byte2);
    bool do_loadOpcodes();

private:
	QString packet_text_hex;
	QString packet_text_binary;
	QString packet_text_desc;
	QVector<LocoByte> locobyte_array;
    QHash<QString, QString> opcodes;
};

#endif // LOCOPACKET_H
