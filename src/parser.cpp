#include "parser.h"
#include "qdebug.h"

Parser::Parser()
{
    style.insert(0, QString("%1"));
    style.insert(1, QString("<b>%1</b>"));
    style.insert(4, QString("<u>%1</u>"));
    style.insert(5, QString("%1"));
    style.insert(7, QString("<ol reversed>%1</ol>")); // не робит
    style.insert(8, QString("<s>%1<s>"));

    textCltr.insert(30, Qt::black);
    textCltr.insert(31, Qt::red);
    textCltr.insert(32, Qt::green);
    textCltr.insert(33, Qt::yellow);
    textCltr.insert(34, Qt::blue);
    textCltr.insert(35, Qt::magenta);
    textCltr.insert(36, Qt::cyan);
    textCltr.insert(37, Qt::white);

    bgCltr.insert(40, Qt::black);
    bgCltr.insert(41, Qt::red);
    bgCltr.insert(42, Qt::green);
    bgCltr.insert(43, Qt::yellow);
    bgCltr.insert(44, Qt::blue);
    bgCltr.insert(45, Qt::magenta);
    bgCltr.insert(46, Qt::cyan);
    bgCltr.insert(47, Qt::white);
}

Parser::~Parser()
{
    qDebug() << buffer;
    buffer.clear();
}
//TODO refactor
void Parser::getMessage(QString mes)
{
    buffer.append(mes);
    while(!buffer.isEmpty()){
        struct ColorString res;
        qsizetype pos = buffer.indexOf("", 0, Qt::CaseInsensitive);
        //Если нет ESC символа выводим строку без изменений
        if(pos < 0){
            res.str = buffer;
            emit setColorMes(res);
            buffer.clear();
        }
        //Если ESC символ стоит не первым выводим строку без изменений до символа
        else if(pos != 0){
            res.str = buffer.left(pos);
            emit setColorMes(res);
            buffer.remove(0, pos);
        }
        //Если ESC символ стоит первым
        else{
            qsizetype posNext = buffer.indexOf("", 2, Qt::CaseInsensitive);
            //Если есть следующий за первым ESC символ. Находим атрибуты, очищаем буффер и отправляем данные
            if(posNext > 2){
                buffer.remove(0,2);
                QString temp = buffer.left(buffer.indexOf('m'));
                buffer.remove(0, buffer.indexOf('m')+1);
                auto lParams = temp.split(';', Qt::SkipEmptyParts);
                temp = "%1";
                for(auto par : lParams){
                    bool ok;
                    uint numb = par.toUInt(&ok);
                    if(!ok) break;
                    if(numb <= 10 && style.find(numb) != style.end())
                        temp = style.find(numb).value();

                    else if(numb <= textCltr.lastKey() && textCltr.find(numb) != textCltr.end())
                        res.clrText = textCltr.find(numb).value();

                    else if(numb <= bgCltr.lastKey() && bgCltr.find(numb) != bgCltr.end())
                        res.clrBackGround = bgCltr.find(numb).value();
                }
                posNext = buffer.indexOf("", 0, Qt::CaseInsensitive);
                res.str = temp.arg(buffer.left(posNext));
                emit setColorMes(res);
                buffer.remove(0, posNext);
            }
            else if(buffer.right(1) == '\r' || buffer.right(1) == '\n'){
                buffer.remove(0,2);
                QString temp = buffer.left(buffer.indexOf('m'));
                buffer.remove(0, buffer.indexOf('m')+1);
                auto lParams = temp.split(';', Qt::SkipEmptyParts);
                temp = "%1";
                for(auto par : lParams){
                    bool ok;
                    uint numb = par.toUInt(&ok);
                    if(!ok) break;
                    if(numb <= 10 && style.find(numb) != style.end())
                        temp = style.find(numb).value();

                    else if(numb <= textCltr.lastKey() && textCltr.find(numb) != textCltr.end())
                        res.clrText = textCltr.find(numb).value();

                    else if(numb <= bgCltr.lastKey() && bgCltr.find(numb) != bgCltr.end())
                        res.clrBackGround = bgCltr.find(numb).value();
                    res.str = temp.arg(buffer.left(posNext));
                    emit setColorMes(res);
                    buffer.clear();
                }
            }
            else return;
        }
    }
}
