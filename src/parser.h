#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QColor>
#include <QMap>

struct ColorString{
    QString str;
    QColor clrText = QColor::fromRgb(224, 225, 227);
    QColor clrBackGround = Qt::transparent;
};

class Parser : public QObject
{
    Q_OBJECT
public:
    Parser();
    ~Parser();

public slots:
    void getMessage(QString mes);

signals:
    void setColorMes(ColorString str);

private:
    QString buffer;
    QMap<uint, QColor> textCltr;
    QMap<uint, QColor> bgCltr;
    QMap<uint, QString> style;
};

#endif // PARSER_H
