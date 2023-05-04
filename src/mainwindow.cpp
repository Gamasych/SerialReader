#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QSettings>
#include <algorithm>
#include <QDebug>
#include <QInputDialog>
#ifdef Q_OS_WIN
#include <qt_windows.h>
#endif
#ifdef Q_OS_LINUX
#include <xcb/xcb.h>
#include <sys/utsname.h>
#endif
#include <QFile>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qRegisterMetaTypeStreamOperators<QList<QString>>("Baudrates");
    qRegisterMetaType<ColorString>("ColorString");
    ui->setupUi(this);
    ui->cbSerialPort->addItems(SerialReader::getAvailPort());
    ui->cbSpeed->addItems(SerialReader::getSpeed());
    session(false);

    //перенос парсера и чтение порта в отдельные потоки
    pars.moveToThread(&threadParser);
    serial.moveToThread(&threadSerial);
    threadParser.start();
    threadSerial.start();
    connect(&serial, &SerialReader::setMessage, &pars, &Parser::getMessage, Qt::QueuedConnection);
    connect(&pars, &Parser::setColorMes, this, &MainWindow::getMessage, Qt::QueuedConnection);

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
    connect(&fileWatch, &QFileSystemWatcher::directoryChanged, this, &MainWindow::eventFileWatch, Qt::QueuedConnection);
    connect(&fileWatch, &QFileSystemWatcher::fileChanged, this, &MainWindow::eventFileWatch, Qt::QueuedConnection);
#endif
}

MainWindow::~MainWindow()
{
    session(true);
    threadParser.quit();
    threadParser.wait();
    threadSerial.quit();
    threadSerial.wait();

    delete ui;
    disconnect(&serial, &SerialReader::setMessage, &pars, &Parser::getMessage);
    disconnect(&pars, &Parser::setColorMes, this, &MainWindow::getMessage);
}

void MainWindow::on_tbUpdate_clicked()
{
    ui->cbSerialPort->clear();
    ui->cbSerialPort->addItems(SerialReader::getAvailPort());
}

void MainWindow::on_tbAdd_clicked()
{
    bool ok;
    int temp = QInputDialog::getInt(this, tr("Input BaudRate"), tr("BaudRate"), 0, 0, 2147483647, 1, &ok);
    if (!ok) return;
    sortComboBox(temp);
}

void MainWindow::on_pbClear_clicked()
{
    ui->textEdit->clear();
}

void MainWindow::on_pbOpen_clicked()
{
    bool disabl;
    if(connectedDevice.isEmpty()){
        if(serial.openSerial(ui->cbSerialPort->currentText(), ui->cbSpeed->currentText().toUInt()) != 0) QMessageBox::warning(this, tr("Device"), tr("Error open serial port"));
        else{
            ui->pbOpen->setText(tr("Close"));
            disabl = true;
            connectedDevice = ui->cbSerialPort->currentText();
        }
    }
    else{
        ui->pbOpen->setText(tr("Open"));
        serial.closeSerial();
        connectedDevice.clear();
        disabl = false;
    }
    ui->tbAdd->setDisabled(disabl);
    ui->cbSerialPort->setDisabled(disabl);
    ui->cbSpeed->setDisabled(disabl);

}

void MainWindow::getMessage(ColorString mes)
{
    QTextCharFormat fmt = ui->textEdit->currentCharFormat();
    fmt.setForeground(mes.clrText);
    fmt.setBackground(mes.clrBackGround);
    ui->textEdit->setCurrentCharFormat(fmt);
    ui->textEdit->insertPlainText(mes.str);
}
#ifdef Q_OS_LINUX
void MainWindow::eventFileWatch(const QString &)
{
    if(QFile::exists(path) && !fileWatch.directories().contains(path)) fileWatch.addPath(path);
    else if(!QFile::exists(path)) fileWatch.addPath("/dev");
    updateDevice();
}
#endif
#ifdef Q_OS_WIN
// События от Windows
bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long*)
{
    MSG * msg = static_cast< MSG * > (message);
    // Событие присоединения нового устройства
    if(msg->message == WM_DEVICECHANGE) updateDevice();
    return false;
}
#endif
void MainWindow::session(bool save)
{
    QString strCurrBaudrate = "Current Baudrate";
    QString strBaudrates = "Baudrates";
    QSettings settings;
    if(!save)
    {
        settings.beginGroup( "SizeMainWindow" );
        if(settings.value("first").toInt() == 1)
        {
            settings.endGroup();
            settings.beginGroup("Data");
            QList<QString> listOfRecBauds = settings.value(strBaudrates).value<QList<QString>>();
            if(listOfRecBauds.length() != 0) ui->cbSpeed->clear();
            for(auto item : listOfRecBauds) ui->cbSpeed->addItem(item);
            ui->cbSpeed->setCurrentIndex(settings.value(strCurrBaudrate).toUInt() >= listOfRecBauds.length() ? 0 : settings.value(strCurrBaudrate).toUInt());
            QString device = settings.value("LastDevice").toString();
            int pos = ui->cbSerialPort->findText(device);
            ui->cbSerialPort->setCurrentIndex(pos >= 0 ? pos : 0);
            settings.endGroup();
        }
        else settings.endGroup();
    }
    else {
        settings.beginGroup( "SizeMainWindow" );
        settings.setValue("first", 1);
        settings.endGroup();

        settings.beginGroup("Data");
        QList<QString> listOfBaud;
        settings.setValue(strCurrBaudrate, ui->cbSpeed->currentIndex());
        for(int i = 0; i < ui->cbSpeed->count(); i++) listOfBaud.append(ui->cbSpeed->itemText(i));
        settings.setValue(strBaudrates, QVariant::fromValue(listOfBaud));
        settings.setValue("LastDevice", ui->cbSerialPort->currentText());
        settings.endGroup();
    }
}

void MainWindow::sortComboBox(uint baudrate)
{
    for(int i = 0; i < ui->cbSpeed->count(); i++)
    {
        if(ui->cbSpeed->itemText(i).toUInt() == baudrate) break;
        else if(ui->cbSpeed->itemText(i).toUInt() > baudrate)
        {
            ui->cbSpeed->insertItem(i, QString::number(baudrate));
            break;
        }
        if(i == ui->cbSpeed->count() - 1)
        {
            ui->cbSpeed->addItem(QString::number(baudrate));
            break;
        }
    }
}

void MainWindow::updateDevice()
{
    QStringList items;
    items = SerialReader::getAvailPort();
    if(!connectedDevice.isEmpty() && !items.contains(connectedDevice)){
        on_pbOpen_clicked();
        QMessageBox::warning(this, tr("Device"), tr("Error! The device in use is disconnected"));
    }
    if(items.length() == 0)
        ui->cbSerialPort->clear();

    else{
        for(int i = 0; i < ui->cbSerialPort->count(); i++ )
        {
            if(!items.contains(ui->cbSerialPort->itemText(i))){
                ui->cbSerialPort->removeItem(i);
                --i;
            }
            else items.removeOne(ui->cbSerialPort->itemText(i));
        }
        if(items.length() != 0){
            ui->cbSerialPort->addItems(items);
            ui->cbSerialPort->setCurrentText(items.at(0));
        }
    }
}

void MainWindow::showEvent(QShowEvent *)
{
    //https://github.com/ColinDuquesnoy/QDarkStyleSheet
    QFile f(":/qdarkstyle/dark/darkstyle.qss");
    if (f.exists()){
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }
    else QMessageBox::warning(this, tr("Theme"), tr("Error open file"));
}
