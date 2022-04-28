#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->cbSerialPort->addItems(serial.getAvailPort());
    ui->cbSpeed->addItems(serial.getSpeed());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tbUpdate_clicked()
{
    ui->cbSerialPort->clear();
    ui->cbSerialPort->addItems(serial.getAvailPort());
}

void MainWindow::on_tbAdd_clicked()
{
    //добавление скорости
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

