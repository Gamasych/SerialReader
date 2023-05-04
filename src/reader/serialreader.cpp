#include "serialreader.h"
#include <QSerialPortInfo>
#include <QDebug>
#include <QColor>

QStringList SerialReader::getAvailPort()
{
    QList<QSerialPortInfo> portInfo = QSerialPortInfo::availablePorts();
    QStringList res;
    for(auto port : portInfo){
        res.append(port.portName());
    }
    return res;
}

QStringList SerialReader::getSpeed()
{
    QList<qint32> baudRates = QSerialPortInfo::standardBaudRates();
    QStringList res;
    for(auto baudRate : baudRates){
        res.append(QString::number(baudRate));
    }
    return res;
}

int SerialReader::openSerial(QString serialPort, quint32 baudRate)
{
    if(serialPort.isEmpty()) return -2;
    if(ser.isOpen()) ser.close();
    ser.setBaudRate(baudRate);
    ser.setPortName(serialPort);
    QObject::connect(&ser, &QSerialPort::readyRead, this, &SerialReader::readData);
    if (!ser.open(QIODevice::ReadWrite)) {
        return -1;
    }
    return 0;
}

int SerialReader::closeSerial()
{
    QObject::disconnect(&ser, &QSerialPort::readyRead, this, &SerialReader::readData);
    if(ser.isOpen()) ser.close();
    return 0;
}

void SerialReader::readData()
{
    auto res = ser.readAll();
    QString str(res);
    emit setMessage(str);
}
