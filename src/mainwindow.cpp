#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qRegisterMetaTypeStreamOperators<QList<QString>>("Baudrates");
    qRegisterMetaType<ColorString>("ColorString");
    ui->setupUi(this);
    addActions({ui->actionConnect, ui->actionParams});
    //перенос парсера и чтение порта в отдельные потоки
    pars.moveToThread(&threadParser);
    serial.moveToThread(&threadSerial);
    threadParser.start();
    threadSerial.start();
    connect(&serial, &SerialReader::setMessage, &pars, &Parser::getMessage, Qt::QueuedConnection);
    connect(&pars, &Parser::setColorMes, this, &MainWindow::getMessage, Qt::QueuedConnection);
    this->setWindowTitle(tr("Serial reader"));
}

MainWindow::~MainWindow()
{
    threadParser.quit();
    threadParser.wait();
    threadSerial.quit();
    threadSerial.wait();

    delete ui;
    disconnect(&serial, &SerialReader::setMessage, &pars, &Parser::getMessage);
    disconnect(&pars, &Parser::setColorMes, this, &MainWindow::getMessage);
}

void MainWindow::on_pbClear_clicked()
{
    ui->textEdit->clear();
}

void MainWindow::getMessage(ColorString mes)
{
    QTextCharFormat fmt = ui->textEdit->currentCharFormat();
    fmt.setForeground(mes.clrText);
    fmt.setBackground(mes.clrBackGround);
    ui->textEdit->setCurrentCharFormat(fmt);
    ui->textEdit->insertPlainText(mes.str);
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

/*
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
}*/

void MainWindow::on_actionParams_triggered()
{
    paramsWindow.show();
}

void MainWindow::on_actionConnect_triggered(bool checked)
{
    if(checked){
           if(serial.openSerial("com8", 115200) != 0) QMessageBox::warning(this, tr("Device"), tr("Error open serial port"));
           else{
               ui->actionConnect->setText(tr("Close"));
           }
       }
       else{
           ui->actionConnect->setText(tr("Open"));
           serial.closeSerial();
       }

}

