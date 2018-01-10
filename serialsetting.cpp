#include "serialsetting.h"
//#include "mainwindow.h"
#include <QDebug>
#include "mainwindow.h"

serialsetting::serialsetting(QWidget *parent)
    : QDialog(parent)
    , serialPortLabel(new QLabel(tr("Serial port:")))
    , serialPortComboBox(new QComboBox())
    , baudrateLabel(new QLabel(tr("Baud Rate")))
    , Baudrates(new QComboBox())
    , runButton(new QPushButton(tr("&OK")))
    , otherSettings(new QLabel(tr("Data:8bit    Stop:1bit    NoParity")))
   // , myserial(new QSerialPort())

{


    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        serialPortComboBox->addItem(info.portName());



    Baudrates->addItem(QWidget::tr("1200"));
    Baudrates->addItem(QWidget::tr("2400"));
    Baudrates->addItem(QWidget::tr("4800"));
    Baudrates->addItem(QWidget::tr("9600"));
    Baudrates->addItem(QWidget::tr("19200"));
    Baudrates->addItem(QWidget::tr("38400"));
    Baudrates->setCurrentIndex(3);



    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(serialPortLabel, 0, 0);
    mainLayout->addWidget(serialPortComboBox, 0, 1);
    mainLayout->addWidget(baudrateLabel, 1, 0);
    mainLayout->addWidget(Baudrates, 1, 1);
    mainLayout->addWidget(otherSettings, 2, 0, 1, 2);
    mainLayout->addWidget(runButton, 4, 0, 2, 2);


 //   mainLayout->addWidget(closeButton, 2, 1);


    QGridLayout *GLayout = new QGridLayout;               //设置框中的布局大小和结构
    QGroupBox *groupBox = new QGroupBox;     //设置一个框





    GLayout -> setRowStretch(0,0);                          //在布局的上边角落加弹簧
    GLayout -> setColumnStretch(0,0);
    GLayout -> setRowStretch(0,2);
    GLayout -> setColumnStretch(0,2);
    GLayout -> setRowStretch(2,0);                      //在布局的下边角落加弹簧
    GLayout -> setColumnStretch(2,0);
    GLayout -> setRowStretch(2,2);
    GLayout -> setColumnStretch(2,2);



    GLayout -> addWidget(groupBox,1,1);
    groupBox -> setLayout(mainLayout);                           //将布局放到框中

    setLayout(GLayout);
    setWindowTitle(tr("SerialSetting"));
    serialPortComboBox->setFocus();

    connect(runButton, &QPushButton::clicked, this, &serialsetting::SetSerial);
}

QSize serialsetting::sizeHint() const            //关键字const，自动运行
 {

    QDesktopWidget* desktopWidget = new QDesktopWidget;

    QRect screenRect = desktopWidget->screenGeometry();

    int currentScreenWid = screenRect.width();

    int currentScreenHei = screenRect.height();
     return QSize( currentScreenWid/4, currentScreenHei/4 );
 }

void serialsetting::SetSerial()  //设置波特率，串口号
{

    QString BaudrateText = Baudrates->currentText();
    QString SerialNameText = serialPortComboBox->currentText();
    int Baudrate = BaudrateText.toInt();



    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
          {
            qDebug()<<Baudrate<<info.portName()<<SerialNameText;
            if(info.portName() == SerialNameText)
            {
                qDebug()<<1;
                myserial->close();
                myserial->setPort(info);
                if(myserial->open(QIODevice::ReadWrite)==true) //读写方式打开串口
                {

                    myserial->setBaudRate(Baudrate);//设置波特率9600
                    qDebug()<<Baudrate<<info.portName();
                    myserial->setDataBits(QSerialPort::Data8);  //设置数据位8位
                    myserial->setStopBits(QSerialPort::OneStop); //设置停止位为一位
                    myserial->setParity(QSerialPort::NoParity);  //设置没有校验位
                }
                else {myserial->error();}
            }
          }



    extern int SerialwindowState;

    if(SerialwindowState == 0)   //关闭当前设置界面，打开主窗口（只一次）
    {
        this->hide();
        MainWindow* mw;
        mw = new MainWindow;
        mw -> show();
        SerialwindowState = 1;
    }

}


