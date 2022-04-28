#include "serialreader.h"
#include <QSerialPortInfo>
#include <QDebug>

SerialReader::SerialReader()
{

}

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
    if(ser.isOpen()) ser.close();
    return 0;
}

void SerialReader::readData()
{
    QString res = QString::fromStdString(ser.readAll().toStdString());
    while(1){
        int pos = res.indexOf("[");
        if(pos >= 0)
            do{
            if(res.at(pos) == 'm'){
                res.remove(pos, 1);
                res.insert(pos, ' ');
                break;
            }
            res.remove(pos, 1);
        }while(pos < res.length());

        else break;
    }
    emit getMessage(res);
}
