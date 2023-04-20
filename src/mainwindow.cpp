#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QSettings>
#include <algorithm>
#include <QDebug>
#include <QInputDialog>
#include <qt_windows.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qRegisterMetaType<ColorString>("ColorString");
    ui->setupUi(this);
    ui->cbSerialPort->addItems(serial.getAvailPort());
    ui->cbSpeed->addItems(serial.getSpeed());
    session(false);
    //TODO go to thread
    //TODO dark theme
    connect(&serial, &SerialReader::setMessage, &pars, &Parser::getMessage);
    connect(&pars, &Parser::setColorMes, this, &MainWindow::getMessage);
}

MainWindow::~MainWindow()
{
    session(true);
    delete ui;
    disconnect(&serial, &SerialReader::setMessage, &pars, &Parser::getMessage);
}

void MainWindow::on_tbUpdate_clicked()
{
    //TODO if connect and device delete disconnect and send error
    ui->cbSerialPort->clear();
    ui->cbSerialPort->addItems(serial.getAvailPort());
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
    if(serial.openSerial(ui->cbSerialPort->currentText(), ui->cbSpeed->currentText().toUInt()) != 0) ui->statusbar->showMessage(tr("Error open serial port"), 100);
    else{
        ui->pbOpen->setEnabled(false);
        ui->pbClose->setEnabled(true);
    }
}

void MainWindow::on_pbClose_clicked()
{
    serial.closeSerial();
    ui->pbOpen->setEnabled(true);
    ui->pbClose->setEnabled(false);
}

void MainWindow::getMessage(ColorString mes)
{
    QTextCharFormat fmt = ui->textEdit->currentCharFormat();
    fmt.setForeground(mes.clrText);
    fmt.setBackground(mes.clrBackGround);
    ui->textEdit->setCurrentCharFormat(fmt);
    ui->textEdit->insertPlainText(mes.str);
}

void MainWindow::session(bool save)
{
    //TODO save last device
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
            ui->cbSpeed->clear();
            for(auto item : listOfRecBauds) ui->cbSpeed->addItem(item);
            ui->cbSpeed->setCurrentIndex(settings.value(strCurrBaudrate).toUInt());
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
        settings.endGroup();
    }
}

void MainWindow::sortComboBox(uint baudrate)
{
    //TODO бинарный поиск
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
//TODO comment and for linux

// Function that receive messages
// This is windows-specific
bool MainWindow::nativeEvent(const QByteArray&, void* message, long*)
{
    MSG * msg = static_cast< MSG * > (message);

    // Does this specific message interest us?
    if(msg->message == WM_DEVICECHANGE)
    {
        on_tbUpdate_clicked();
    }

    // Qt handles the rest
    return false;
}
