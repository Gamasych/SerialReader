#include "add_baudrate.h"
#include "ui_add_baudrate.h"
#include <QIntValidator>

add_baudrate::add_baudrate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::add_baudrate)
{
    ui->setupUi(this);
    ui->lineBaudrateEdit->setValidator(new QIntValidator(MIN_BAUDRATE, MAX_BAUDRATE, this) );
}

add_baudrate::~add_baudrate()
{
    delete ui;
}

void add_baudrate::on_addBaudrateButton_clicked()
{
    emit addBaudrateSignal(ui->lineBaudrateEdit->text().toUInt());
    ui->lineBaudrateEdit->clear();
}
