#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    qRegisterMetaTypeStreamOperators<QList<QString>>("Baudrates");
    QApplication a(argc, argv);

    /*QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "SerialReader_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }*/
    a.setOrganizationName("AIS");
    a.setOrganizationDomain("KB");
    a.setApplicationName("lizator");
    MainWindow w;
    w.show();
    return a.exec();
}
