#ifndef SERIALREADER_H
#define SERIALREADER_H

#include <QSerialPort>
#include <QList>

class SerialReader: public QObject
{
    Q_OBJECT
public:
    SerialReader();
    QStringList getAvailPort();
    QStringList getSpeed();
    int openSerial(QString serialPort, quint32 baudRate);
    int closeSerial();

private slots:
    void readData();

signals:
    void getMessage(QString data);

private:
    QSerialPort ser;
};

#endif // SERIALREADER_H
