#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include "reader/serialreader.h"
#include "parser/parser.h"
#include "parameters/serial_settings.h"

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
    void on_pbClear_clicked();
    void getMessage(ColorString mes);
    void on_actionParams_triggered();
    void on_actionConnect_triggered(bool checked);

protected:
    void showEvent(QShowEvent * event) override; //открытие окна

private:
    Ui::MainWindow *ui;
    SerialSettings paramsWindow;
    SerialReader serial;
    Parser pars;
    QThread threadParser;
    QThread threadSerial;
};
#endif // MAINWINDOW_H
