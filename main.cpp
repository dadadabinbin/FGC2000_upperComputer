#include<QApplication>
#include<QQuickView>
#include<QQmlContext>
#include"mainwindow.h"
#include"monitor.h"
#include "serialsetting.h"

#include<QQuickView>


QSerialPort* myserial;    //串口通讯设为全局变量
int SerialwindowState = 0;  //用于控制serialsetting控件是否关闭窗口
int currentScreenWidth ,currentScreenHeight;
int ViewlogState = 0;
int XwindowState = 0;
int YwindowState = 0;
int ZwindowState = 0;
int XYZwindowState = 0;

int main(int argc,char** argv)
{

    QApplication app(argc,argv);
    myserial = new QSerialPort;    //设置串口

    QDesktopWidget*  desktopWidget;
    desktopWidget = new QDesktopWidget;

    QRect screenRect = desktopWidget -> screenGeometry();

    currentScreenWidth = screenRect.width();

    currentScreenHeight = screenRect.height();


/*    //
    comboBox c;
    fgcDemo fg;
    monitor m;



    //QObject::connect(fg.action,SIGNAL(triggered(bool)),w.button4,SIGNAL(clicked(bool)));

    c.show();
    c.setWindowTitle("FGC2000 PC Demo Interface");
  */
    //MainWindow w;
    //w.show();
    serialsetting SerialSettingWindow;
    SerialSettingWindow.show();
//    showCompass();
    return app.exec();
}




