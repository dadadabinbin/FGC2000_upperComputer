#ifndef ADDWIDGETS_H
#define ADDWIDGETS_H


//#include<QQuickItem>
//#include <QQmlEngine>
//#include<QQmlComponent>
#include <QQmlContext>
#include <QTMath>
#include <QQuickView>
#include <QQuickView>
#include <QWidget>
#include <QTabWidget>
#include <QTimer>
#include <QQmlApplicationEngine>
#include "monitor.h"
#include "serialsetting.h"
#include "ShowCurve.h"


class AddWidgets : public QTabWidget
{
    Q_OBJECT
//    Q_PROPERTY(qreal angle READ getAngle WRITE setAngle NOTIFY angleChanged)

public:
    AddWidgets(QWidget *parent = 0);
    monitor* monitortable;
    QWidget* widget;
    QQuickView* quickview;
    friend class ApplicationData;
//    qreal angle();
//    Q_INVOKABLE qreal getCurrentData() const {
//           return m_angle;
//       }



public slots:
    void MonitorLogclose();
    void MonitorLogopen();   //打开/关闭log窗口

    //以下从monitor.h移出
    void JudgeCommand();
    void readmyserial();


    void ShowXCurve();
    void ShowYCurve();
    void ShowZCurve();
    void ShowXYZCurve();
    void CompassShow();
    void PitchShow();
//    qreal ontimeout();
    /*
     * 用于模拟数据
     * edit by BriFuture
    */
    qreal  getData();
    qreal  getPitch();
    qreal getRoll();

//    qreal getCurrentData();
//    Q_INVOKABLE   qreal getCurrentData()  {
//           qDebug()<<"m_angle="<<m_angle<<endl;
//       //    update();
//          return m_angle;
//      }

    /*
    void closeXYZCurve();
void closeYCurve();
    void closeXCurve();
    void closeZCurve();
    */
signals:
//     void angleChanged();

private:

    serialsetting* SetSerial;
    void AddSerialSetting();
    void AddMonitorTable();


    //以下从monitor.h移出
    QByteArray* buf;
    QByteArray OutputData[10];
    QByteArray TempByteArry;

    //从readmyserial();中移出的变量
    int i,SizeOfTempByteArry,SizeOfBuf;        //i为临时变量，SizeOfTempByteArry储存数组的长度，
                                               // SizeOfBuf为临时保存Buf内的信息的长度
    QByteArray tempbuf;                        //无法直接读出buf内信息的长度，因此设置临时变量 tempbuf
    QByteArray Temphead;
    int SizeOfHead;
    QByteArray MessageHeader;
    int SizeOfMessageHeader;
    //从readmyserial();中移出的变量

    QByteArray GetDataBetweenComas(QByteArray TempByteArry,int* i,int SizeOfTempByteArry);
    QByteArray GetDataBetweenComas(QByteArray TempByteArry,int* i,int SizeOfTempByteArry,char j);
    int NMEA_0183_hh(QByteArray TempByteArry);
    //从monitor.h移出



    //ShowCurve* show_X_Curve;
    //ShowCurve* show_Y_Curve;
    //ShowCurve* show_Z_Curve;
    ShowCurve* show_Curve;

    QQuickView *viewpitch;
     qreal m_angle;
     qreal m_pitch;
     qreal m_roll;
     QTimer timer;
     int a=7;



};

#endif  //ADDWIDGET_H



