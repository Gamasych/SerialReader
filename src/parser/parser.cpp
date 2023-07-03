#include "parser.h"
#include "qdebug.h"
#include "../common.h"

//https://habr.com/ru/companies/macloud/articles/558316/
Parser::Parser()
{
}

Parser::~Parser()
{
    buffer.clear();
}
//TODO refactor
void Parser::getMessage(QString mes)
{
    buffer.append(mes);
    while(!buffer.isEmpty()){
        res.str.clear();
        qsizetype pos = buffer.indexOf("", 0, Qt::CaseInsensitive);
        //Если нет ESC символа выводим строку без изменений
        if(pos < 0){
            res.str = styleText.arg(buffer);
            emit setColorMes(res);
            buffer.clear();
        }
        //Если ESC символ стоит не первым выводим строку без изменений до символа
        else if(pos != 0){
            res.str = styleText.arg(buffer.left(pos));
            emit setColorMes(res);
            buffer.remove(0, pos);
        }
        //Если ESC символ стоит первым
        else{
            //Если несколько ESC символов идут друг за другом, упаковываем в один и собираем данные
            while(buffer.at(0) == ''){
                qsizetype posEndEsc = buffer.indexOf('m', 0, Qt::CaseInsensitive);
                if(posEndEsc < 0) return;
                temp.append(buffer.mid(2, posEndEsc - 2));
                buffer.remove(0, posEndEsc + 1);
            }
            //Разбираем значения и записываем параметры
            //Заданы дефолтные значения
            auto lParams = temp.split(';', Qt::SkipEmptyParts);
            for(uint i = 0; i < lParams.length(); ++i){
                bool ok;
                uint numb = lParams.at(i).toUInt(&ok);
                if(!ok) break;
                if(numb == 0){
                    styleText = "%1";
                    res = ColorString();
                }
                else if(numb == 38 || numb == 48){
                    QColor tempClr;
                    if(lParams.length() <= i+1) return;
                    uint params = lParams.at(i+1).toUInt();
                    if(params == 5){//256 цветов
                        if(lParams.length() <= i+2) return;
                        tempClr = ParserSettings::hXTermColor.find(lParams.at(i+2).toUInt()).value();
                    }
                    else if(params == 2){//RGB
                        if(lParams.length() <= i+4) return;
                        tempClr = QColor::fromRgbF(lParams.at(i+2).toUInt(), lParams.at(i+3).toUInt(), lParams.at(i+4).toUInt());
                    }
                    if(numb == 38) res.clrText = tempClr;
                    else if(numb == 48) res.clrBackGround = tempClr;
                }
                else if(numb <= 10 && ParserSettings::hStyle.find(numb) != ParserSettings::hStyle.end())
                    styleText = styleText.arg(ParserSettings::hStyle.find(numb).value());

                else if(numb <= ParserSettings::hTextColor.keyEnd().base().key() && ParserSettings::hTextColor.find(numb) != ParserSettings::hTextColor.end()){
                    res.clrText = ParserSettings::hTextColor.find(numb).value();
                }

                else if(numb <= ParserSettings::hBackgroundColor.keyEnd().base().key() && ParserSettings::hBackgroundColor.find(numb) != ParserSettings::hBackgroundColor.end())
                    res.clrBackGround = ParserSettings::hBackgroundColor.find(numb).value();
                else
                    qDebug() << "unknown";
            }

            auto posNext = buffer.indexOf("", 0, Qt::CaseInsensitive);
            if(posNext < 0) posNext = buffer.length();
            res.str = styleText.arg(buffer.left(posNext));
            emit setColorMes(res);
            temp.clear();
            buffer.remove(0, posNext);
        }
    }
}
