#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QColor>
#include <QMap>
#include <QHash>

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
    ColorString res;
    QString temp;
    QString styleText = "%1";
};

#endif // PARSER_H
