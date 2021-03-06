#ifndef LOCOTRAIN_H
#define LOCOTRAIN_H

#include <QString>
#include <QDebug>
#include <QChar>
#include <QBitArray>
#include <QTime>

#include "locobyte.h"

namespace std {
	class LocoTrain;
}

class LocoTrain
{
public:
	LocoTrain();
	~LocoTrain();
	QString timeStamp();
	bool operator==(LocoTrain _arg);
	void set_name(QString _name);
	void set_adr(LocoByte _adr);
	void set_slot(LocoByte _slot);
	void set_reverse(bool _forward);
	void set_speed(LocoByte _speed);
	void set_state(QString _state);
	LocoByte get_adr();
	LocoByte get_slot();
	LocoByte get_speed();
	QString get_state();
	bool get_direction();
	QString get_descrtiption();

protected:

private:
	QString name;
	LocoByte adr;
	LocoByte slot;
	bool reverse; // 1 = backward
	LocoByte speed;
	static bool debug;
	QString state;
	bool lighting;
	LocoByte sound;

};

#endif // LOCOTRAIN_H







