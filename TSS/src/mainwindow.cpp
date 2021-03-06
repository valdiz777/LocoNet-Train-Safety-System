#include "mainwindow.h"
#include "ui_mainwindow.h"


/**
 * tss - "Train Safety System"
 * Prevent train collisions in digitrax HO model
 * Multithreaded program that provides an anti-collision safety system for Loco trains for uah's train safety system lab.
 * Valentine Nwachukwu [valdiz777@gmail.com], Richard Oteri [ruo0001@uah.edu], Michael Jones [mkj0002@uah.edu], Adam Cowell [adam.a.colwell@gmail.com]
 *
 * License: I've tried hard not to step on any copyrights associated with Digitrax code,
 * don't sue me please.
 *
 * Comments: We would like to give a lot of credit to Chris Bero as most of the code here relating to locoserial
 * was repurposed from his repo: https://github.com/ctag/cpe453
 * Version:1.0
 */

 /**
  * Conventions:
  * Underscores indicate a local variable to a function
  * set_ to change a member variable
  * get_ to retrieve a member variable or variant
  * do_ to complete a task or slot
  * is_ to query a state of the object
  * handle_ to take care of a signal, similar to do_
  * do_run() in each thread class allows for proper instantiation of child variables
  */

  /**
   * Program Structure:
   * Two threads: GUI, Serial/USB
   * All interaction should be via Signals/Slots through the GUI thread.
   */

bool MainWindow::debug = false;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	this->setWindowIcon(QIcon(":/images/logo"));

	// Declare threads
	locoserial = new LocoSerial;
	locoserial->moveToThread(&threadSerial);
	trainmonitor = new TrainMonitor;
	trainmonitor->moveToThread(&threadMonitor);
	outgoingPacket.clear();

	ui->lineEdit_opcode->setInputMask("hh");
	ui->lineEdit_arg1->setInputMask("hh");
	ui->lineEdit_arg2->setInputMask("hh");

	// GUI
	connect(ui->comboBox_opcodes, SIGNAL(activated(int)), this, SLOT(do_OPfromComboBox()));
	connect(ui->pushButton_genPacket, SIGNAL(clicked()), this, SLOT(do_genPacket()));
	connect(ui->lineEdit_opcode, SIGNAL(editingFinished()), this, SLOT(do_enableArgs()));
	connect(ui->pushButton_serialRefreshList, SIGNAL(clicked()), this, SLOT(do_refreshSerialList()));
	connect(ui->pushButton_serialConnect, SIGNAL(clicked()), this, SLOT(do_openSerial()));
	connect(ui->pushButton_serialDisconnect, SIGNAL(clicked()), locoserial, SLOT(do_close()));
    connect(ui->pushButton_serialDisconnect, SIGNAL(clicked()), this, SLOT(do_clearPairs()));
	connect(ui->pushButton_sendPacket, SIGNAL(clicked()), this, SLOT(do_sendSerial()));

	// Serial
	connect(this, &MainWindow::locoserial_open, locoserial, &LocoSerial::do_open);
	connect(this, &MainWindow::locoserial_write, locoserial, &LocoSerial::do_writePacket);
	connect(this, &MainWindow::nodeOff, locoserial, &LocoSerial::do_nodeOff);
	connect(this, &MainWindow::nodeOn, locoserial, &LocoSerial::do_nodeOn);
	connect(locoserial, &LocoSerial::receivedPacket, this, &MainWindow::do_packetReceived); // QT-5 style works
	connect(locoserial, &LocoSerial::writtenBytes, this, &MainWindow::do_bytesWritten);
	connect(locoserial, &LocoSerial::printPacketDesc, this, &MainWindow::do_printDescriptions); // QT-5 style works
	connect(locoserial, &LocoSerial::serialOpened, this, &MainWindow::handle_serialOpened);
	connect(locoserial, &LocoSerial::serialOpened, trainmonitor, &TrainMonitor::handle_serialOpened);
	connect(locoserial, &LocoSerial::serialClosed, this, &MainWindow::handle_serialClosed);
	connect(locoserial, &LocoSerial::occupancyDataReady, trainmonitor, &TrainMonitor::do_handleOccupancy);
    connect(locoserial, &LocoSerial::switchUpdated, trainmonitor, &TrainMonitor::do_handleSwitch);
	connect(locoserial, &LocoSerial::querySlot, locoserial, &LocoSerial::do_querySlot);
	connect(&threadSerial, &QThread::finished, locoserial, &QObject::deleteLater);

	// TrainMonitor
	connect(trainmonitor, &TrainMonitor::slotScan, locoserial, &LocoSerial::do_slotScan);
	connect(trainmonitor, &TrainMonitor::slotDispatch, locoserial, &LocoSerial::do_slotDispatch);
	connect(trainmonitor, &TrainMonitor::slotReq, locoserial, &LocoSerial::do_slotReq);
	connect(trainmonitor, &TrainMonitor::slotUse, locoserial, &LocoSerial::do_slotUse);
	connect(trainmonitor, &TrainMonitor::slotClear, locoserial, &LocoSerial::do_slotClear);
	connect(trainmonitor, &TrainMonitor::trackReset, locoserial, &LocoSerial::do_trackReset);
	connect(trainmonitor, &TrainMonitor::trackOn, locoserial, &LocoSerial::do_trackOn);
	connect(trainmonitor, &TrainMonitor::trackOff, locoserial, &LocoSerial::do_trackOff);
	connect(trainmonitor, &TrainMonitor::sectionOff, locoserial, &LocoSerial::do_sectionOff);
	connect(trainmonitor, &TrainMonitor::sectionOn, locoserial, &LocoSerial::do_sectionOn);
	connect(trainmonitor, &TrainMonitor::closeTurnout, locoserial, &LocoSerial::do_closeTurnout);
	connect(trainmonitor, &TrainMonitor::throwTurnout, locoserial, &LocoSerial::do_throwTurnout);
	connect(trainmonitor, &TrainMonitor::clearSectionOffCount, locoserial, &LocoSerial::do_clearSectionOff);
	connect(trainmonitor, &TrainMonitor::clearSectionOnCount, locoserial, &LocoSerial::do_clearSectionOn);
	connect(trainmonitor, &TrainMonitor::printSectionsOff, locoserial, &LocoSerial::do_getSectionsOff);
	connect(trainmonitor, &TrainMonitor::printSectionsOn, locoserial, &LocoSerial::do_getSectionsOn);
	connect(trainmonitor, &TrainMonitor::collisionEvt, this, &MainWindow::do_showCollisionEvt);
    connect(trainmonitor, &TrainMonitor::systemReady, this, &MainWindow::do_showSystemReady);
	connect(&threadMonitor, &QThread::finished, trainmonitor, &QObject::deleteLater);

	// Kickstart threads
	threadSerial.start();
	locoserial->do_run(); // Auto start locoserial
	threadMonitor.start(); // Auto start trainmonitor
	trainmonitor->do_run();

	// Configure interface
	ui->comboBox_opcodes->setEditable(false);
	ui->comboBox_opcodes->setInsertPolicy(QComboBox::InsertAtBottom);
	do_loadOPComboBox();
	do_refreshSerialList();

	if (debug) qDebug() << timeStamp() << "Interface loaded.";
}

MainWindow::~MainWindow()
{
	locoserial->do_close();
	// Clean up sub-threads
	threadSerial.quit();
	threadSerial.wait();
    threadMonitor.quit();
    threadMonitor.wait();
	delete ui;
}

/*
 * MAINWINDOW METHODS
 */

QString MainWindow::timeStamp()
{
	return(QTime::currentTime().toString("[HH:mm:ss:zzz] "));
}

void MainWindow::do_enableArgs()
{
	LocoPacket _packet(ui->lineEdit_opcode->text());
	int _args = _packet.get_finalSize() - 2;
	if (_args > 0) {
		ui->lineEdit_arg1->setEnabled(true);
	}
	else {
		ui->lineEdit_arg1->setEnabled(false);
		ui->lineEdit_arg2->setEnabled(false);
		return;
	}
	if (_args > 1) {
		ui->lineEdit_arg2->setEnabled(true);
	}
	else {
		ui->lineEdit_arg2->setEnabled(false);
	}
}

void MainWindow::do_genPacket()
{
	QString _hex = "";
	outgoingPacket.set_allFromHex(ui->lineEdit_opcode->text());
	int _numArgs = outgoingPacket.get_finalSize() - 2;
	LocoPacket * _packet;
	_hex.append(ui->lineEdit_opcode->text());
	if (_numArgs > 0) {
		_hex.append(ui->lineEdit_arg1->text());
	} if (_numArgs > 1) {
		_hex.append(ui->lineEdit_arg2->text());
	}
	_packet = new LocoPacket(_hex);
	_packet->do_genChecksum();

	outgoingPacket = *_packet;
	ui->textBrowser_packets->append(timeStamp() + "Generated: " + _packet->get_packet());
	ui->lineEdit_packet->setText(_packet->get_packet());
}

void MainWindow::do_loadOPComboBox()
{
	ui->comboBox_opcodes->clear();
	LocoPacket _tmp;
    QVector<QString> _opcodes = _tmp.get_Opcodes();
    QVector<QString> _names = _tmp.get_Names();
	if (_opcodes.count() != _names.count())
	{
		qDebug() << timeStamp() << "database opcodes and names don't match.";
		return;
	}
	for (int _index = 0; _index < _opcodes.count(); ++_index)
	{
		QString _text = _names[_index];
		_text.append(" [" + _opcodes[_index] + "]");
		ui->comboBox_opcodes->insertItem(_index, _text);
	}
}

void MainWindow::do_OPfromComboBox()
{
	LocoPacket _tmp;
    QVector<QString> _opcodes = _tmp.get_Opcodes();
	QString _hex = _opcodes[ui->comboBox_opcodes->currentIndex()];
	ui->lineEdit_opcode->setText(_hex);
	do_enableArgs();
}

void MainWindow::do_showCollisionEvt(QStringList collisionSections)
{
    QString info = collisionSections[0];
    info.append("\nResolve sections");
    collisionSections.removeFirst();
    for (QString section : collisionSections)
	{
        if (!section.isEmpty())
        {
            info.append(" [" + section + "] ");
            qDebug() << "Emitting node off for:" << section;
            emit nodeOff(section);
        }
	}

	QMessageBox *msgBox = new QMessageBox;
    msgBox->setIcon(QMessageBox::Icon::Critical);
    msgBox->setWindowIcon(QIcon(":/images/logo"));
	msgBox->setAttribute(Qt::WA_DeleteOnClose);
	msgBox->setText("Collision event detected!");
	msgBox->setInformativeText(info);
	msgBox->setStandardButtons(QMessageBox::Ok);
	msgBox->setDefaultButton(QMessageBox::Ok);
    msgBox->setProperty("collisionSections", collisionSections);
	msgBox->setModal(false);
    msgBox->open(this, SLOT(msgBoxClosed(QAbstractButton*)));
}


void MainWindow::do_showSystemReady()
{
    QString info = "Initialization finished.\n System Is Ready!";

    QMessageBox *msgBox = new QMessageBox;
    msgBox->setIcon(QMessageBox::Icon::Information);
    msgBox->setWindowIcon(QIcon(":/images/logo"));
    msgBox->setAttribute(Qt::WA_DeleteOnClose);
    msgBox->setText("System Initialization Complete!");
    msgBox->setInformativeText(info);
    msgBox->setStandardButtons(QMessageBox::Ok);
    msgBox->setDefaultButton(QMessageBox::Ok);
    msgBox->setModal(false);
    msgBox->show();
}

void MainWindow::do_clearPairs()
{
    trainmonitor->clearSectionPairs();
}


void MainWindow::msgBoxClosed(QAbstractButton *button)
{
    QMessageBox *msgBox = (QMessageBox *)sender();
    QStringList collisionSections = msgBox->property("collisionSections").toStringList();
	QMessageBox::StandardButton btn = msgBox->standardButton(button);
	if (btn == QMessageBox::Ok)
	{
		for (QString section : collisionSections)
		{
			if (!section.isEmpty())
			{
                qDebug() << "Emitting nodeON for:" << section;
				emit nodeOn(section);
			}
		}
	}
	else
	{
        throw "unknown button";
    }
}

void MainWindow::do_refreshSerialList()
{
	int _index = 0;
	ui->comboBox_serialList->clear();
	foreach(QextPortInfo portInfo, QextSerialEnumerator::getPorts())
	{
		ui->comboBox_serialList->insertItem(_index, portInfo.portName);
		ui->textBrowser_console->append(portInfo.portName);
		_index++;
	}
}

void MainWindow::do_packetReceived(LocoPacket _packet)
{
	if (debug) qDebug() << timeStamp() << "Reading packet to text browser. " << _packet.get_packet();
	ui->textBrowser_console->append(timeStamp() + _packet.get_packet());

	// Sort packets for easier reading
	QString _op = _packet.get_OPcode();
	if (_op == "B2")
	{
		ui->textBrowser_sorted_b2->append(timeStamp() + _packet.get_packet());
	}
	else if (_op == "A0") {
		ui->textBrowser_sorted_a0->append(timeStamp() + _packet.get_packet());
	}
	else if (_op == "E7") {
		ui->textBrowser_sorted_e7->append(timeStamp() + _packet.get_packet());
	}
}

void MainWindow::do_bytesWritten(QByteArray _bytes)
{
	QString _byteText = QString::fromLatin1(_bytes.toHex());
	if (debug) qDebug() << timeStamp() << "Writing bytes to text browser. " << _byteText;
	ui->textBrowser_packets->append(timeStamp() + "Written: " + _byteText);
}

void MainWindow::do_printDescriptions(QString description)
{
	ui->textBrowser_console->append(timeStamp() + description);
}

/*
 * LOCOSERIAL METHODS
 */

void MainWindow::handle_serialOpened()
{
	ui->textBrowser_console->append(timeStamp() + "Serial port opened x)");
	ui->pushButton_serialConnect->setEnabled(false);
	ui->pushButton_serialDisconnect->setEnabled(true);
	ui->comboBox_serialList->setEnabled(false);
	ui->pushButton_serialRefreshList->setEnabled(false);
}

void MainWindow::handle_serialClosed()
{
	ui->textBrowser_console->append(timeStamp() + "Serial port closed :D");
	ui->pushButton_serialConnect->setEnabled(true);
	ui->pushButton_serialDisconnect->setEnabled(false);
	ui->comboBox_serialList->setEnabled(true);
	ui->pushButton_serialRefreshList->setEnabled(true);
}

void MainWindow::do_openSerial()
{
	int _portIndex = ui->comboBox_serialList->currentIndex();
	QextPortInfo _device = QextSerialEnumerator::getPorts().at(_portIndex);
	emit locoserial_open(_device);
}

void MainWindow::do_sendSerial()
{
	outgoingPacket.set_allFromHex(ui->lineEdit_packet->text());
	if (!outgoingPacket.validChk())
	{
        qDebug() << timeStamp() << "Not a valid LocoPacket, Incorrect Opcode.";
		return;
	}
	ui->textBrowser_packets->append(timeStamp() + "Asking for write: " + outgoingPacket.get_packet().toLatin1());

	// Only interact with a thread via Sig/Slots.
	emit locoserial_write(outgoingPacket);

	if (debug) qDebug() << timeStamp() << "Firing off to serial: " << outgoingPacket.get_packet().toLatin1();
	if (debug) qDebug() << timeStamp() << outgoingPacket.get_QByteArray() << outgoingPacket.get_QBitArray();
}



