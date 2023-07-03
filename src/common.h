#ifndef COMMON_H
#define COMMON_H
#include <QtGlobal>
#include <QSerialPort>
#include <QHash>
#include <QObject>
#include <QColor>

namespace Params{

struct {
    QString portName;
    qint32 baudRate;
    bool breakEnabled;
    QSerialPort::DataBits dataBits;
    bool dataTerminalReady;
    QSerialPort::FlowControl flowControl;
    QSerialPort::Parity parity;
    bool requestToSend;
    QSerialPort::StopBits stopBits;
    bool readOnly;
}SerialParams;

static const QHash<QString,int> hDataBits{ {QSerialPort::tr("data 8"), QSerialPort::DataBits::Data8},
                                           {QSerialPort::tr("data 5"), QSerialPort::DataBits::Data5},
                                           {QSerialPort::tr("data 6"), QSerialPort::DataBits::Data6},
                                           {QSerialPort::tr("data 7"), QSerialPort::DataBits::Data7},
                                           {QSerialPort::tr("Unknown data Bits"), QSerialPort::DataBits::UnknownDataBits}};

static const QHash<QString,int> hParity{ {QSerialPort::tr("No parity"), QSerialPort::Parity::NoParity},
                                         {QSerialPort::tr("Even parity"), QSerialPort::Parity::EvenParity},
                                         {QSerialPort::tr("Odd parity"), QSerialPort::Parity::OddParity},
                                         {QSerialPort::tr("Space parity"), QSerialPort::Parity::SpaceParity},
                                         {QSerialPort::tr("Mark parity"), QSerialPort::Parity::MarkParity},
                                         {QSerialPort::tr("Unknown parity"), QSerialPort::Parity::UnknownParity}};

static const QHash<QString,int> hStopBits{ {QSerialPort::tr("One stop"), QSerialPort::StopBits::OneStop},
                                           {QSerialPort::tr("One and half stop"), QSerialPort::StopBits::OneAndHalfStop},
                                           {QSerialPort::tr("Two stop"), QSerialPort::StopBits::TwoStop},
                                           {QSerialPort::tr("Unknown stop bits"), QSerialPort::StopBits::UnknownStopBits}};

static const QHash<QString,int> hFlowCntrl{ {QSerialPort::tr("No flow control"), QSerialPort::FlowControl::NoFlowControl},
                                            {QSerialPort::tr("Hardware control"), QSerialPort::FlowControl::HardwareControl},
                                            {QSerialPort::tr("Software control"), QSerialPort::FlowControl::SoftwareControl},
                                            {QSerialPort::tr("Unknown flow control"), QSerialPort::FlowControl::UnknownFlowControl}};
}

namespace ParserSettings{
static const QHash<uint, QColor> hTextColor{
    {30, Qt::black},
    {31, Qt::red},
    {32, Qt::green},
    {33, Qt::yellow},
    {34, Qt::blue},
    {35, Qt::magenta},
    {36, Qt::cyan},
    {37, Qt::white}
};

static const QHash<uint, QColor> hBackgroundColor{
    //standart background color
    {40, Qt::black},
    {41, Qt::red},
    {42, Qt::green},
    {43, Qt::yellow},
    {44, Qt::blue},
    {45, Qt::magenta},
    {46, Qt::cyan},
    {47, Qt::white}
};

static const QHash<uint, QString> hStyle{
    {0, QString("%1")},
    {1, QString("<b>%1</b>")},//жирный
    {2, QString("%1")},//NOTE блеклый (найти)
    {3, QString("<i>%1</i>")}, //Курсив
    {4, QString("<u>%1</u>")},//подчеркнутый
    {5, QString("%1")}, //NOTE мигание (не робит)
    {7, QString("<ol reversed>%1</ol>")}, //NOTE обратный (не робит)
    {8, QString("<s>%1<s>")},//зачеркнутый
    {9, QString("<s>%1<s>")},//зачеркнутый
    {21, QString("%1")},//NOTE двойное подчеркивание (найти)
    {51, QString("%1")},//NOTE обрамленный (найти)
    {52, QString("%1")},//NOTE окруженный (найти)
    {53, QString("%1")}//NOTE надчеркнутый (найти)
};

static const QHash<uint, QColor> hXTermColor{
    {0, QColor(0,0,0)},
    {1, QColor(128,0,0)},
    {2, QColor(0,128,0)},
    {3, QColor(128,128,0)},
    {4, QColor(0,0,128)},
    {5, QColor(128,0,128)},
    {6, QColor(0,128,128)},
    {7, QColor(192,192,192)},
    {8, QColor(128,128,128)},
    {9, QColor(255,0,0)},
    {10, QColor(0,255,0)},
    {11, QColor(255,255,0)},
    {12, QColor(0,0,255)},
    {13, QColor(255,0,255)},
    {14, QColor(0,255,255)},
    {15, QColor(255,255,255)},
    {16, QColor(0,0,0)},
    {17, QColor(0,0,95)},
    {18, QColor(0,0,135)},
    {19, QColor(0,0,175)},
    {20, QColor(0,0,215)},
    {21, QColor(0,0,255)},
    {22, QColor(0,95,0)},
    {23, QColor(0,95,95)},
    {24, QColor(0,95,135)},
    {25, QColor(0,95,175)},
    {26, QColor(0,95,215)},
    {27, QColor(0,95,255)},
    {28, QColor(0,135,0)},
    {29, QColor(0,135,95)},
    {30, QColor(0,135,135)},
    {31, QColor(0,135,175)},
    {32, QColor(0,135,215)},
    {33, QColor(0,135,255)},
    {34, QColor(0,175,0)},
    {35, QColor(0,175,95)},
    {36, QColor(0,175,135)},
    {37, QColor(0,175,175)},
    {38, QColor(0,175,215)},
    {39, QColor(0,175,255)},
    {40, QColor(0,215,0)},
    {41, QColor(0,215,95)},
    {42, QColor(0,215,135)},
    {43, QColor(0,215,175)},
    {44, QColor(0,215,215)},
    {45, QColor(0,215,255)},
    {46, QColor(0,255,0)},
    {47, QColor(0,255,95)},
    {48, QColor(0,255,135)},
    {49, QColor(0,255,175)},
    {50, QColor(0,255,215)},
    {51, QColor(0,255,255)},
    {52, QColor(95,0,0)},
    {53, QColor(95,0,95)},
    {54, QColor(95,0,135)},
    {55, QColor(95,0,175)},
    {56, QColor(95,0,215)},
    {57, QColor(95,0,255)},
    {58, QColor(95,95,0)},
    {59, QColor(95,95,95)},
    {60, QColor(95,95,135)},
    {61, QColor(95,95,175)},
    {62, QColor(95,95,215)},
    {63, QColor(95,95,255)},
    {64, QColor(95,135,0)},
    {65, QColor(95,135,95)},
    {66, QColor(95,135,135)},
    {67, QColor(95,135,175)},
    {68, QColor(95,135,215)},
    {69, QColor(95,135,255)},
    {70, QColor(95,175,0)},
    {71, QColor(95,175,95)},
    {72, QColor(95,175,135)},
    {73, QColor(95,175,175)},
    {74, QColor(95,175,215)},
    {75, QColor(95,175,255)},
    {76, QColor(95,215,0)},
    {77, QColor(95,215,95)},
    {78, QColor(95,215,135)},
    {79, QColor(95,215,175)},
    {80, QColor(95,215,215)},
    {81, QColor(95,215,255)},
    {82, QColor(95,255,0)},
    {83, QColor(95,255,95)},
    {84, QColor(95,255,135)},
    {85, QColor(95,255,175)},
    {86, QColor(95,255,215)},
    {87, QColor(95,255,255)},
    {88, QColor(135,0,0)},
    {89, QColor(135,0,95)},
    {90, QColor(135,0,135)},
    {91, QColor(135,0,175)},
    {92, QColor(135,0,215)},
    {93, QColor(135,0,255)},
    {94, QColor(135,95,0)},
    {95, QColor(135,95,95)},
    {96, QColor(135,95,135)},
    {97, QColor(135,95,175)},
    {98, QColor(135,95,215)},
    {99, QColor(135,95,255)},
    {100, QColor(135,135,0)},
    {101, QColor(135,135,95)},
    {102, QColor(135,135,135)},
    {103, QColor(135,135,175)},
    {104, QColor(135,135,215)},
    {105, QColor(135,135,255)},
    {106, QColor(135,175,0)},
    {107, QColor(135,175,95)},
    {108, QColor(135,175,135)},
    {109, QColor(135,175,175)},
    {110, QColor(135,175,215)},
    {111, QColor(135,175,255)},
    {112, QColor(135,215,0)},
    {113, QColor(135,215,95)},
    {114, QColor(135,215,135)},
    {115, QColor(135,215,175)},
    {116, QColor(135,215,215)},
    {117, QColor(135,215,255)},
    {118, QColor(135,255,0)},
    {119, QColor(135,255,95)},
    {120, QColor(135,255,135)},
    {121, QColor(135,255,175)},
    {122, QColor(135,255,215)},
    {123, QColor(135,255,255)},
    {124, QColor(175,0,0)},
    {125, QColor(175,0,95)},
    {126, QColor(175,0,135)},
    {127, QColor(175,0,175)},
    {128, QColor(175,0,215)},
    {129, QColor(175,0,255)},
    {130, QColor(175,95,0)},
    {131, QColor(175,95,95)},
    {132, QColor(175,95,135)},
    {133, QColor(175,95,175)},
    {134, QColor(175,95,215)},
    {135, QColor(175,95,255)},
    {136, QColor(175,135,0)},
    {137, QColor(175,135,95)},
    {138, QColor(175,135,135)},
    {139, QColor(175,135,175)},
    {140, QColor(175,135,215)},
    {141, QColor(175,135,255)},
    {142, QColor(175,175,0)},
    {143, QColor(175,175,95)},
    {144, QColor(175,175,135)},
    {145, QColor(175,175,175)},
    {146, QColor(175,175,215)},
    {147, QColor(175,175,255)},
    {148, QColor(175,215,0)},
    {149, QColor(175,215,95)},
    {150, QColor(175,215,135)},
    {151, QColor(175,215,175)},
    {152, QColor(175,215,215)},
    {153, QColor(175,215,255)},
    {154, QColor(175,255,0)},
    {155, QColor(175,255,95)},
    {156, QColor(175,255,135)},
    {157, QColor(175,255,175)},
    {158, QColor(175,255,215)},
    {159, QColor(175,255,255)},
    {160, QColor(215,0,0)},
    {161, QColor(215,0,95)},
    {162, QColor(215,0,135)},
    {163, QColor(215,0,175)},
    {164, QColor(215,0,215)},
    {165, QColor(215,0,255)},
    {166, QColor(215,95,0)},
    {167, QColor(215,95,95)},
    {168, QColor(215,95,135)},
    {169, QColor(215,95,175)},
    {170, QColor(215,95,215)},
    {171, QColor(215,95,255)},
    {172, QColor(215,135,0)},
    {173, QColor(215,135,95)},
    {174, QColor(215,135,135)},
    {175, QColor(215,135,175)},
    {176, QColor(215,135,215)},
    {177, QColor(215,135,255)},
    {178, QColor(215,175,0)},
    {179, QColor(215,175,95)},
    {180, QColor(215,175,135)},
    {181, QColor(215,175,175)},
    {182, QColor(215,175,215)},
    {183, QColor(215,175,255)},
    {184, QColor(215,215,0)},
    {185, QColor(215,215,95)},
    {186, QColor(215,215,135)},
    {187, QColor(215,215,175)},
    {188, QColor(215,215,215)},
    {189, QColor(215,215,255)},
    {190, QColor(215,255,0)},
    {191, QColor(215,255,95)},
    {192, QColor(215,255,135)},
    {193, QColor(215,255,175)},
    {194, QColor(215,255,215)},
    {195, QColor(215,255,255)},
    {196, QColor(255,0,0)},
    {197, QColor(255,0,95)},
    {198, QColor(255,0,135)},
    {199, QColor(255,0,175)},
    {200, QColor(255,0,215)},
    {201, QColor(255,0,255)},
    {202, QColor(255,95,0)},
    {203, QColor(255,95,95)},
    {204, QColor(255,95,135)},
    {205, QColor(255,95,175)},
    {206, QColor(255,95,215)},
    {207, QColor(255,95,255)},
    {208, QColor(255,135,0)},
    {209, QColor(255,135,95)},
    {210, QColor(255,135,135)},
    {211, QColor(255,135,175)},
    {212, QColor(255,135,215)},
    {213, QColor(255,135,255)},
    {214, QColor(255,175,0)},
    {215, QColor(255,175,95)},
    {216, QColor(255,175,135)},
    {217, QColor(255,175,175)},
    {218, QColor(255,175,215)},
    {219, QColor(255,175,255)},
    {220, QColor(255,215,0)},
    {221, QColor(255,215,95)},
    {222, QColor(255,215,135)},
    {223, QColor(255,215,175)},
    {224, QColor(255,215,215)},
    {225, QColor(255,215,255)},
    {226, QColor(255,255,0)},
    {227, QColor(255,255,95)},
    {228, QColor(255,255,135)},
    {229, QColor(255,255,175)},
    {230, QColor(255,255,215)},
    {231, QColor(255,255,255)},
    {232, QColor(8,8,8)},
    {233, QColor(18,18,18)},
    {234, QColor(28,28,28)},
    {235, QColor(38,38,38)},
    {236, QColor(48,48,48)},
    {237, QColor(58,58,58)},
    {238, QColor(68,68,68)},
    {239, QColor(78,78,78)},
    {240, QColor(88,88,88)},
    {241, QColor(98,98,98)},
    {242, QColor(108,108,108)},
    {243, QColor(118,118,118)},
    {244, QColor(128,128,128)},
    {245, QColor(138,138,138)},
    {246, QColor(148,148,148)},
    {247, QColor(158,158,158)},
    {248, QColor(168,168,168)},
    {249, QColor(178,178,178)},
    {250, QColor(188,188,188)},
    {251, QColor(198,198,198)},
    {252, QColor(208,208,208)},
    {253, QColor(218,218,218)},
    {254, QColor(228,228,228)}
};
}

#endif // COMMON_H