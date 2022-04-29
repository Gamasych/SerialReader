#ifndef ADD_BAUDRATE_H
#define ADD_BAUDRATE_H

#include <QWidget>

#define MIN_BAUDRATE 0
#define MAX_BAUDRATE 4000000

namespace Ui {
class add_baudrate;
}

class add_baudrate : public QWidget
{
    Q_OBJECT

public:
    explicit add_baudrate(QWidget *parent = nullptr);
    virtual ~add_baudrate();

signals:
    void addBaudrateSignal(uint baudrate);

private slots:
    void on_addBaudrateButton_clicked();

private:
    Ui::add_baudrate *ui;
};

#endif // ADD_BAUDRATE_H
