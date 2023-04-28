#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#ifdef Q_OS_LINUX
#include <QFileSystemWatcher>
#endif
#include "serialreader.h"
#include "parser.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_tbUpdate_clicked();
    void on_tbAdd_clicked();
    void on_pbClear_clicked();
    void on_pbOpen_clicked();
    void getMessage(ColorString mes);
 #ifdef Q_OS_LINUX
    void eventFileWatch(const QString &path);
 #endif

protected:
    void showEvent(QShowEvent * event) override; //открытие окна
#ifdef Q_OS_WIN
    bool nativeEvent(const QByteArray& eventType, void* message,
                     long* result) override;

#endif
private:
    void session(bool save);
    void sortComboBox(uint baudrate);

    Ui::MainWindow *ui;
    SerialReader serial;
    Parser pars;
    QThread threadParser;
    QThread threadSerial;
    QString connectedDevice;
#ifdef Q_OS_LINUX
    QFileSystemWatcher fileWatch;
    QString path;
#endif
};
#endif // MAINWINDOW_H
