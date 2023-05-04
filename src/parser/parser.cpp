#include "parser.h"
#include "qdebug.h"

//https://habr.com/ru/companies/macloud/articles/558316/
Parser::Parser()
{
    config();
}

Parser::~Parser()
{
    buffer.clear();
    bgCltr.clear();
    xtermColor.clear();
    style.clear();
    textCltr.clear();
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
                        tempClr = xtermColor.find(lParams.at(i+2).toUInt()).value();
                    }
                    else if(params == 2){//RGB
                        if(lParams.length() <= i+4) return;
                        tempClr = QColor::fromRgbF(lParams.at(i+2).toUInt(), lParams.at(i+3).toUInt(), lParams.at(i+4).toUInt());
                    }
                    if(numb == 38) res.clrText = tempClr;
                    else if(numb == 48) res.clrBackGround = tempClr;
                }
                else if(numb <= 10 && style.find(numb) != style.end())
                    styleText = styleText.arg(style.find(numb).value());

                else if(numb <= textCltr.lastKey() && textCltr.find(numb) != textCltr.end()){
                    res.clrText = textCltr.find(numb).value();
                }

                else if(numb <= bgCltr.lastKey() && bgCltr.find(numb) != bgCltr.end())
                    res.clrBackGround = bgCltr.find(numb).value();
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

void Parser::config()
{
    //html style for text
    style.insert(0, QString("%1"));
    style.insert(1, QString("<b>%1</b>")); //жирный
    style.insert(2, QString("%1")); //NOTE блеклый (найти)
    style.insert(3, QString("<i>%1</i>")); //Курсив
    style.insert(4, QString("<u>%1</u>"));//подчеркнутый
    style.insert(5, QString("%1")); //NOTE мигание (не робит)
    style.insert(7, QString("<ol reversed>%1</ol>")); //NOTE обратный (не робит)
    style.insert(8, QString("<s>%1<s>"));//зачеркнутый
    style.insert(9, QString("<s>%1<s>"));//зачеркнутый
    style.insert(21, QString("%1"));//NOTE двойное подчеркивание (найти)
    style.insert(51, QString("%1"));//NOTE обрамленный (найти)
    style.insert(52, QString("%1"));//NOTE окруженный (найти)
    style.insert(53, QString("%1"));//NOTE надчеркнутый (найти)

    //standart text color
    textCltr.insert(30, Qt::black);
    textCltr.insert(31, Qt::red);
    textCltr.insert(32, Qt::green);
    textCltr.insert(33, Qt::yellow);
    textCltr.insert(34, Qt::blue);
    textCltr.insert(35, Qt::magenta);
    textCltr.insert(36, Qt::cyan);
    textCltr.insert(37, Qt::white);

    //standart background color
    bgCltr.insert(40, Qt::black);
    bgCltr.insert(41, Qt::red);
    bgCltr.insert(42, Qt::green);
    bgCltr.insert(43, Qt::yellow);
    bgCltr.insert(44, Qt::blue);
    bgCltr.insert(45, Qt::magenta);
    bgCltr.insert(46, Qt::cyan);
    bgCltr.insert(47, Qt::white);

    //256 color
    xtermColor.insert(0, QColor(0,0,0));
    xtermColor.insert(1, QColor(128,0,0));
    xtermColor.insert(2, QColor(0,128,0));
    xtermColor.insert(3, QColor(128,128,0));
    xtermColor.insert(4, QColor(0,0,128));
    xtermColor.insert(5, QColor(128,0,128));
    xtermColor.insert(6, QColor(0,128,128));
    xtermColor.insert(7, QColor(192,192,192));
    xtermColor.insert(8, QColor(128,128,128));
    xtermColor.insert(9, QColor(255,0,0));
    xtermColor.insert(10, QColor(0,255,0));
    xtermColor.insert(11, QColor(255,255,0));
    xtermColor.insert(12, QColor(0,0,255));
    xtermColor.insert(13, QColor(255,0,255));
    xtermColor.insert(14, QColor(0,255,255));
    xtermColor.insert(15, QColor(255,255,255));
    xtermColor.insert(16, QColor(0,0,0));
    xtermColor.insert(17, QColor(0,0,95));
    xtermColor.insert(18, QColor(0,0,135));
    xtermColor.insert(19, QColor(0,0,175));
    xtermColor.insert(20, QColor(0,0,215));
    xtermColor.insert(21, QColor(0,0,255));
    xtermColor.insert(22, QColor(0,95,0));
    xtermColor.insert(23, QColor(0,95,95));
    xtermColor.insert(24, QColor(0,95,135));
    xtermColor.insert(25, QColor(0,95,175));
    xtermColor.insert(26, QColor(0,95,215));
    xtermColor.insert(27, QColor(0,95,255));
    xtermColor.insert(28, QColor(0,135,0));
    xtermColor.insert(29, QColor(0,135,95));
    xtermColor.insert(30, QColor(0,135,135));
    xtermColor.insert(31, QColor(0,135,175));
    xtermColor.insert(32, QColor(0,135,215));
    xtermColor.insert(33, QColor(0,135,255));
    xtermColor.insert(34, QColor(0,175,0));
    xtermColor.insert(35, QColor(0,175,95));
    xtermColor.insert(36, QColor(0,175,135));
    xtermColor.insert(37, QColor(0,175,175));
    xtermColor.insert(38, QColor(0,175,215));
    xtermColor.insert(39, QColor(0,175,255));
    xtermColor.insert(40, QColor(0,215,0));
    xtermColor.insert(41, QColor(0,215,95));
    xtermColor.insert(42, QColor(0,215,135));
    xtermColor.insert(43, QColor(0,215,175));
    xtermColor.insert(44, QColor(0,215,215));
    xtermColor.insert(45, QColor(0,215,255));
    xtermColor.insert(46, QColor(0,255,0));
    xtermColor.insert(47, QColor(0,255,95));
    xtermColor.insert(48, QColor(0,255,135));
    xtermColor.insert(49, QColor(0,255,175));
    xtermColor.insert(50, QColor(0,255,215));
    xtermColor.insert(51, QColor(0,255,255));
    xtermColor.insert(52, QColor(95,0,0));
    xtermColor.insert(53, QColor(95,0,95));
    xtermColor.insert(54, QColor(95,0,135));
    xtermColor.insert(55, QColor(95,0,175));
    xtermColor.insert(56, QColor(95,0,215));
    xtermColor.insert(57, QColor(95,0,255));
    xtermColor.insert(58, QColor(95,95,0));
    xtermColor.insert(59, QColor(95,95,95));
    xtermColor.insert(60, QColor(95,95,135));
    xtermColor.insert(61, QColor(95,95,175));
    xtermColor.insert(62, QColor(95,95,215));
    xtermColor.insert(63, QColor(95,95,255));
    xtermColor.insert(64, QColor(95,135,0));
    xtermColor.insert(65, QColor(95,135,95));
    xtermColor.insert(66, QColor(95,135,135));
    xtermColor.insert(67, QColor(95,135,175));
    xtermColor.insert(68, QColor(95,135,215));
    xtermColor.insert(69, QColor(95,135,255));
    xtermColor.insert(70, QColor(95,175,0));
    xtermColor.insert(71, QColor(95,175,95));
    xtermColor.insert(72, QColor(95,175,135));
    xtermColor.insert(73, QColor(95,175,175));
    xtermColor.insert(74, QColor(95,175,215));
    xtermColor.insert(75, QColor(95,175,255));
    xtermColor.insert(76, QColor(95,215,0));
    xtermColor.insert(77, QColor(95,215,95));
    xtermColor.insert(78, QColor(95,215,135));
    xtermColor.insert(79, QColor(95,215,175));
    xtermColor.insert(80, QColor(95,215,215));
    xtermColor.insert(81, QColor(95,215,255));
    xtermColor.insert(82, QColor(95,255,0));
    xtermColor.insert(83, QColor(95,255,95));
    xtermColor.insert(84, QColor(95,255,135));
    xtermColor.insert(85, QColor(95,255,175));
    xtermColor.insert(86, QColor(95,255,215));
    xtermColor.insert(87, QColor(95,255,255));
    xtermColor.insert(88, QColor(135,0,0));
    xtermColor.insert(89, QColor(135,0,95));
    xtermColor.insert(90, QColor(135,0,135));
    xtermColor.insert(91, QColor(135,0,175));
    xtermColor.insert(92, QColor(135,0,215));
    xtermColor.insert(93, QColor(135,0,255));
    xtermColor.insert(94, QColor(135,95,0));
    xtermColor.insert(95, QColor(135,95,95));
    xtermColor.insert(96, QColor(135,95,135));
    xtermColor.insert(97, QColor(135,95,175));
    xtermColor.insert(98, QColor(135,95,215));
    xtermColor.insert(99, QColor(135,95,255));
    xtermColor.insert(100, QColor(135,135,0));
    xtermColor.insert(101, QColor(135,135,95));
    xtermColor.insert(102, QColor(135,135,135));
    xtermColor.insert(103, QColor(135,135,175));
    xtermColor.insert(104, QColor(135,135,215));
    xtermColor.insert(105, QColor(135,135,255));
    xtermColor.insert(106, QColor(135,175,0));
    xtermColor.insert(107, QColor(135,175,95));
    xtermColor.insert(108, QColor(135,175,135));
    xtermColor.insert(109, QColor(135,175,175));
    xtermColor.insert(110, QColor(135,175,215));
    xtermColor.insert(111, QColor(135,175,255));
    xtermColor.insert(112, QColor(135,215,0));
    xtermColor.insert(113, QColor(135,215,95));
    xtermColor.insert(114, QColor(135,215,135));
    xtermColor.insert(115, QColor(135,215,175));
    xtermColor.insert(116, QColor(135,215,215));
    xtermColor.insert(117, QColor(135,215,255));
    xtermColor.insert(118, QColor(135,255,0));
    xtermColor.insert(119, QColor(135,255,95));
    xtermColor.insert(120, QColor(135,255,135));
    xtermColor.insert(121, QColor(135,255,175));
    xtermColor.insert(122, QColor(135,255,215));
    xtermColor.insert(123, QColor(135,255,255));
    xtermColor.insert(124, QColor(175,0,0));
    xtermColor.insert(125, QColor(175,0,95));
    xtermColor.insert(126, QColor(175,0,135));
    xtermColor.insert(127, QColor(175,0,175));
    xtermColor.insert(128, QColor(175,0,215));
    xtermColor.insert(129, QColor(175,0,255));
    xtermColor.insert(130, QColor(175,95,0));
    xtermColor.insert(131, QColor(175,95,95));
    xtermColor.insert(132, QColor(175,95,135));
    xtermColor.insert(133, QColor(175,95,175));
    xtermColor.insert(134, QColor(175,95,215));
    xtermColor.insert(135, QColor(175,95,255));
    xtermColor.insert(136, QColor(175,135,0));
    xtermColor.insert(137, QColor(175,135,95));
    xtermColor.insert(138, QColor(175,135,135));
    xtermColor.insert(139, QColor(175,135,175));
    xtermColor.insert(140, QColor(175,135,215));
    xtermColor.insert(141, QColor(175,135,255));
    xtermColor.insert(142, QColor(175,175,0));
    xtermColor.insert(143, QColor(175,175,95));
    xtermColor.insert(144, QColor(175,175,135));
    xtermColor.insert(145, QColor(175,175,175));
    xtermColor.insert(146, QColor(175,175,215));
    xtermColor.insert(147, QColor(175,175,255));
    xtermColor.insert(148, QColor(175,215,0));
    xtermColor.insert(149, QColor(175,215,95));
    xtermColor.insert(150, QColor(175,215,135));
    xtermColor.insert(151, QColor(175,215,175));
    xtermColor.insert(152, QColor(175,215,215));
    xtermColor.insert(153, QColor(175,215,255));
    xtermColor.insert(154, QColor(175,255,0));
    xtermColor.insert(155, QColor(175,255,95));
    xtermColor.insert(156, QColor(175,255,135));
    xtermColor.insert(157, QColor(175,255,175));
    xtermColor.insert(158, QColor(175,255,215));
    xtermColor.insert(159, QColor(175,255,255));
    xtermColor.insert(160, QColor(215,0,0));
    xtermColor.insert(161, QColor(215,0,95));
    xtermColor.insert(162, QColor(215,0,135));
    xtermColor.insert(163, QColor(215,0,175));
    xtermColor.insert(164, QColor(215,0,215));
    xtermColor.insert(165, QColor(215,0,255));
    xtermColor.insert(166, QColor(215,95,0));
    xtermColor.insert(167, QColor(215,95,95));
    xtermColor.insert(168, QColor(215,95,135));
    xtermColor.insert(169, QColor(215,95,175));
    xtermColor.insert(170, QColor(215,95,215));
    xtermColor.insert(171, QColor(215,95,255));
    xtermColor.insert(172, QColor(215,135,0));
    xtermColor.insert(173, QColor(215,135,95));
    xtermColor.insert(174, QColor(215,135,135));
    xtermColor.insert(175, QColor(215,135,175));
    xtermColor.insert(176, QColor(215,135,215));
    xtermColor.insert(177, QColor(215,135,255));
    xtermColor.insert(178, QColor(215,175,0));
    xtermColor.insert(179, QColor(215,175,95));
    xtermColor.insert(180, QColor(215,175,135));
    xtermColor.insert(181, QColor(215,175,175));
    xtermColor.insert(182, QColor(215,175,215));
    xtermColor.insert(183, QColor(215,175,255));
    xtermColor.insert(184, QColor(215,215,0));
    xtermColor.insert(185, QColor(215,215,95));
    xtermColor.insert(186, QColor(215,215,135));
    xtermColor.insert(187, QColor(215,215,175));
    xtermColor.insert(188, QColor(215,215,215));
    xtermColor.insert(189, QColor(215,215,255));
    xtermColor.insert(190, QColor(215,255,0));
    xtermColor.insert(191, QColor(215,255,95));
    xtermColor.insert(192, QColor(215,255,135));
    xtermColor.insert(193, QColor(215,255,175));
    xtermColor.insert(194, QColor(215,255,215));
    xtermColor.insert(195, QColor(215,255,255));
    xtermColor.insert(196, QColor(255,0,0));
    xtermColor.insert(197, QColor(255,0,95));
    xtermColor.insert(198, QColor(255,0,135));
    xtermColor.insert(199, QColor(255,0,175));
    xtermColor.insert(200, QColor(255,0,215));
    xtermColor.insert(201, QColor(255,0,255));
    xtermColor.insert(202, QColor(255,95,0));
    xtermColor.insert(203, QColor(255,95,95));
    xtermColor.insert(204, QColor(255,95,135));
    xtermColor.insert(205, QColor(255,95,175));
    xtermColor.insert(206, QColor(255,95,215));
    xtermColor.insert(207, QColor(255,95,255));
    xtermColor.insert(208, QColor(255,135,0));
    xtermColor.insert(209, QColor(255,135,95));
    xtermColor.insert(210, QColor(255,135,135));
    xtermColor.insert(211, QColor(255,135,175));
    xtermColor.insert(212, QColor(255,135,215));
    xtermColor.insert(213, QColor(255,135,255));
    xtermColor.insert(214, QColor(255,175,0));
    xtermColor.insert(215, QColor(255,175,95));
    xtermColor.insert(216, QColor(255,175,135));
    xtermColor.insert(217, QColor(255,175,175));
    xtermColor.insert(218, QColor(255,175,215));
    xtermColor.insert(219, QColor(255,175,255));
    xtermColor.insert(220, QColor(255,215,0));
    xtermColor.insert(221, QColor(255,215,95));
    xtermColor.insert(222, QColor(255,215,135));
    xtermColor.insert(223, QColor(255,215,175));
    xtermColor.insert(224, QColor(255,215,215));
    xtermColor.insert(225, QColor(255,215,255));
    xtermColor.insert(226, QColor(255,255,0));
    xtermColor.insert(227, QColor(255,255,95));
    xtermColor.insert(228, QColor(255,255,135));
    xtermColor.insert(229, QColor(255,255,175));
    xtermColor.insert(230, QColor(255,255,215));
    xtermColor.insert(231, QColor(255,255,255));
    xtermColor.insert(232, QColor(8,8,8));
    xtermColor.insert(233, QColor(18,18,18));
    xtermColor.insert(234, QColor(28,28,28));
    xtermColor.insert(235, QColor(38,38,38));
    xtermColor.insert(236, QColor(48,48,48));
    xtermColor.insert(237, QColor(58,58,58));
    xtermColor.insert(238, QColor(68,68,68));
    xtermColor.insert(239, QColor(78,78,78));
    xtermColor.insert(240, QColor(88,88,88));
    xtermColor.insert(241, QColor(98,98,98));
    xtermColor.insert(242, QColor(108,108,108));
    xtermColor.insert(243, QColor(118,118,118));
    xtermColor.insert(244, QColor(128,128,128));
    xtermColor.insert(245, QColor(138,138,138));
    xtermColor.insert(246, QColor(148,148,148));
    xtermColor.insert(247, QColor(158,158,158));
    xtermColor.insert(248, QColor(168,168,168));
    xtermColor.insert(249, QColor(178,178,178));
    xtermColor.insert(250, QColor(188,188,188));
    xtermColor.insert(251, QColor(198,198,198));
    xtermColor.insert(252, QColor(208,208,208));
    xtermColor.insert(253, QColor(218,218,218));
    xtermColor.insert(254, QColor(228,228,228));

}
