#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serialreader.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_tbUpdate_clicked();
    void on_tbAdd_clicked();
    void on_pbClear_clicked();
    void on_pbOpen_clicked();
    void on_pbClose_clicked();
    void getMessage(QString mes);
    void addBaudrateSlot(uint baudrate);

private:
    void session(bool save);
    void sortComboBox(uint baudrate);

    Ui::MainWindow *ui;
    SerialReader serial;
    QWidget *add_baudrate_widget;
};
#endif // MAINWINDOW_H
