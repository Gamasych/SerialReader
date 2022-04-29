#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QSettings>
#include "add_baudrate.h"
#include <algorithm>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->cbSerialPort->addItems(serial.getAvailPort());
    ui->cbSpeed->addItems(serial.getSpeed());
    add_baudrate_widget = new add_baudrate();
    session(false);
    connect(add_baudrate_widget, SIGNAL(addBaudrateSignal(uint)), this, SLOT(addBaudrateSlot(uint)));
}

MainWindow::~MainWindow()
{
    session(true);
    delete ui;
}

void MainWindow::on_tbUpdate_clicked()
{
    ui->cbSerialPort->clear();
    ui->cbSerialPort->addItems(serial.getAvailPort());
}

void MainWindow::on_tbAdd_clicked()
{
    add_baudrate_widget->show();
}

void MainWindow::on_pbClear_clicked()
{
    ui->textEdit->clear();
}

void MainWindow::on_pbOpen_clicked()
{
    if(serial.openSerial(ui->cbSerialPort->currentText(), ui->cbSpeed->currentText().toUInt()) != 0) ui->statusbar->showMessage(tr("Error open serial port"), 100);
    else{
        connect(&serial, SIGNAL(getMessage(QString)), this, SLOT(getMessage(QString)));
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

void MainWindow::getMessage(QString mes)
{
    ui->textEdit->insertPlainText(mes);
}

void MainWindow::addBaudrateSlot(uint baudrate)
{
    sortComboBox(baudrate);
    add_baudrate_widget->close();
}

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
