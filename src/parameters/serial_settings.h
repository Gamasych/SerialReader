#ifndef SERIAL_SETTINGS_H
#define SERIAL_SETTINGS_H

#include <QMainWindow>

namespace Ui {
class SerialSettings;
}

class SerialSettings : public QMainWindow
{
    Q_OBJECT

public:
    explicit SerialSettings(QWidget *parent = nullptr);
    ~SerialSettings();

private:
    Ui::SerialSettings *ui;
};

#endif // SERIAL_SETTINGS_H
