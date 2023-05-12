#include "serial_settings.h"
#include "ui_serial_settings.h"

SerialSettings::SerialSettings(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SerialSettings)
{
    ui->setupUi(this);
}

SerialSettings::~SerialSettings()
{
    delete ui;
}
