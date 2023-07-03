#include "serial_settings.h"
#include "ui_serial_settings.h"
#include "../common.h"
#include "../reader/serialreader.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#ifdef Q_OS_WIN
#include <qt_windows.h>
#endif
#ifdef Q_OS_LINUX
#include <xcb/xcb.h>
#include <sys/utsname.h>
#endif
#include <QDebug>
SerialSettings::SerialSettings(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SerialSettings)
{
    //9600 QSerialPort::Data8 QSerialPort::NoFlowControl QSerialPort::NoParity QFlags<QSerialPort::PinoutSignal>(DataTerminalReadySignal|ClearToSendSignal) QSerialPort::OneStop
    ui->setupUi(this);
    this->setWindowTitle(tr("Device params"));

    ui->BaudComboBox->addItems(SerialReader::getSpeed());
    ui->DataBitsComboBox->addItems(Params::hDataBits.keys());
    ui->ParityComboBox->addItems(Params::hParity.keys());
    ui->StopBitsComboBox->addItems(Params::hStopBits.keys());
    ui->FlowControlComboBox->addItems(Params::hFlowCntrl.keys());

    on_updatePushButton_clicked();
#ifdef Q_OS_LINUX
    utsname kernelInfo;
    const int code = uname(&kernelInfo);
    assert(code == 0); (void)code;

    auto releaseVers = QString::fromStdString(kernelInfo.release);
    releaseVers.resize(releaseVers.indexOf('.', releaseVers.indexOf('.', 0) + 1));
    bool ok;

    double vers = releaseVers.toDouble(&ok);
    if(ok && vers >= 2.5)
        path = "/dev/serial/by-id/";
    else
        path = "/dev/";
    if(!fileWatch.addPath(path)) fileWatch.addPath("/dev");
    connect(&fileWatch, &QFileSystemWatcher::directoryChanged, this, &SerialSettings::eventFileWatch, Qt::QueuedConnection);
    connect(&fileWatch, &QFileSystemWatcher::fileChanged, this, &SerialSettings::eventFileWatch, Qt::QueuedConnection);
#endif
}

SerialSettings::~SerialSettings()
{
    delete ui;
}

void SerialSettings::on_updatePushButton_clicked()
{
    ui->portNameComboBox->clear();
    ui->portNameComboBox->addItems(SerialReader::getAvailPort());
}
/*
void SerialSettings::on_toolButton_triggered(QAction *arg1)
{
    bool ok;
    int temp = QInputDialog::getInt(this, tr("Input BaudRate"), tr("BaudRate"), 0, 0, 2147483647, 1, &ok);
    if (!ok) return;
    sortComboBox(temp);
}

void SerialSettings::sortComboBox(uint baudrate)
{
    for(int i = 0; i < ui->BaudComboBox->count(); i++)
    {
        if(ui->BaudComboBox->itemText(i).toUInt() == baudrate) break;
        else if(ui->BaudComboBox->itemText(i).toUInt() > baudrate)
        {
            ui->BaudComboBox->insertItem(i, QString::number(baudrate));
            break;
        }
        if(i == ui->BaudComboBox->count() - 1)
        {
            ui->BaudComboBox->addItem(QString::number(baudrate));
            break;
        }
    }
}
*/
#ifdef Q_OS_LINUX
void SerialSettings::eventFileWatch(const QString &)
{
    if(QFile::exists(path) && !fileWatch.directories().contains(path)) fileWatch.addPath(path);
    else if(!QFile::exists(path)) fileWatch.addPath("/dev");
    updateDevice();
}
#endif
#ifdef Q_OS_WIN
// События от Windows
bool SerialSettings::nativeEvent(const QByteArray &eventType, void *message, long*)
{
    MSG * msg = static_cast< MSG * > (message);
    // Событие присоединения нового устройства
    if(msg->message == WM_DEVICECHANGE) updateDevice();
    return false;
}
#endif

void SerialSettings::updateDevice()
{
    QStringList items;
    items = SerialReader::getAvailPort();
    if(!connectedDevice.isEmpty() && !items.contains(connectedDevice)){
        //on_pbOpen_clicked(); Disconnect
        QMessageBox::warning(this, tr("Device"), tr("Error! The device in use is disconnected"));
    }
    if(items.length() == 0)
        ui->portNameComboBox->clear();

    else{
        for(int i = 0; i < ui->portNameComboBox->count(); i++ )
        {
            if(!items.contains(ui->portNameComboBox->itemText(i))){
                ui->portNameComboBox->removeItem(i);
                --i;
            }
            else items.removeOne(ui->portNameComboBox->itemText(i));
        }
        if(items.length() != 0){
            ui->portNameComboBox->addItems(items);
            ui->portNameComboBox->setCurrentText(items.at(0));
        }
    }
}

void SerialSettings::showEvent(QShowEvent *)
{
    //https://github.com/ColinDuquesnoy/QDarkStyleSheet
    ui->BaudComboBox->setEditable(true);
    QFile f(":/qdarkstyle/dark/darkstyle.qss");
    if (f.exists()){
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }
    else QMessageBox::warning(this, tr("Theme"), tr("Error open file"));
}

void SerialSettings::on_BaudComboBox_editTextChanged(const QString &arg1)
{
    auto newBaud = ui->BaudComboBox->itemText(0).toInt();
    ui->BaudComboBox->removeItem(0);
    for(int i = 0; i < ui->BaudComboBox->count(); ++i){
        if(ui->BaudComboBox->itemText(i).toInt() > newBaud){
            ui->BaudComboBox->insertItem(i, QString::number(newBaud));
            break;
        }
    }
}

