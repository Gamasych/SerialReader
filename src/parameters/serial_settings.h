#ifndef SERIAL_SETTINGS_H
#define SERIAL_SETTINGS_H

#include <QMainWindow>
#ifdef Q_OS_LINUX
#include <QFileSystemWatcher>
#endif

namespace Ui {
class SerialSettings;
}

class SerialSettings : public QMainWindow
{
    Q_OBJECT

public:
    explicit SerialSettings(QWidget *parent = nullptr);
    ~SerialSettings();
    void deviceWasConnected(bool connected);

private slots:
    void on_updatePushButton_clicked();
    //void on_toolButton_triggered(QAction *arg1);
#ifdef Q_OS_LINUX
   void eventFileWatch(const QString &path);
#endif

   void on_BaudComboBox_editTextChanged(const QString &arg1);

protected:
    void showEvent(QShowEvent * event) override; //открытие окна
#ifdef Q_OS_WIN
    bool nativeEvent(const QByteArray& eventType, void* message,
                     long* result) override;
#endif
private:
    //void sortComboBox(uint baudRate);
    void updateDevice();

    Ui::SerialSettings *ui;
    QString connectedDevice;
#ifdef Q_OS_LINUX
    QFileSystemWatcher fileWatch;
    QString path;
#endif

};

#endif // SERIAL_SETTINGS_H
