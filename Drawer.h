#ifndef DRAWER_H
#define DRAWER_H

#include<stdlib.h>
#include <QWidget>
#include<QString>
#include<QTimer>
#include<QDateTime>
#include<QTimerEvent>
#include<QMouseEvent>
#include<QFile>
#include<QMessageBox>
#include<QCloseEvent>
#include<QDesktopWidget>

class Drawer : public QWidget
{
    Q_OBJECT
public:
    explicit Drawer(QWidget *parent = 0);

    void drawCurves(QPainter &painter);
    void drawGrid(QPainter &painter);
    void drawarrowhead(QPoint lastpoint,QPoint endPoint,QPainter &painter);
    float displayDataprevious;
    bool flag;
    int timecount;
    int myTimerId;
    QTimer* sampletime;
    float CurrentNum;
    QString DataName;
    QString PicName;
    float xposmove;   //为了让x轴移动所加的变量

    //这个变量是用来存储选择时间单位的最终结果
    long int timemultiple;
    int mytimeselect;//让其变成全局变量
    long int mycontinuetime;
    float myproportion;//y轴放大倍数
    float timeproportion;
    int myinterval;
    QString savedir;

private:
    //后加变量

    float xdivide;

    QString ystrnum;

    QDateTime* mydatetime;



    QString time;//记录显示的时间
    QString xtime;//显示x轴箭头处的时间
    //这里用指针，程序的栈空间应该不够用了
   // QString* timeunitstr;



    QVector<QPointF>  line;
    float textnum;


    //这两个变量是记录显示时候点的变量让显示的时候，能够实时反应真实数据情况
    QPoint mypointfront;
    QPoint mypointnext;

    //这两个变量是记录最初窗口的大小，使得这个程序可以正常运行在任何电脑上
    float currentwidth;
    float currenthight;

    //这个函数是用来选择时间单位的倍数关系
    long int mytimeselcet(int timeselect);



    //这两个变量是为了最后写入文件的时候确定写入采样总时间和它的单位
    int showtime;



   //这个函数用来定时器事件
   void timerEvent(QTimerEvent *event);
   //这个函数用来双击全屏
   void mouseDoubleClickEvent(QMouseEvent *ev);

   //void closeEvent(QCloseEvent *event);

   QSize sizeHint() const;
   float WidthFactor;
   void SaveCurve();

   //这个函数用来重绘图案
   void paintEvent(QPaintEvent *event);
   //void paintCurve();

public:
   void restart();
   int Trigger;


signals:

public slots:
     void openDraw(int interval, int continuetime, float proportion,int timeselect, QString mydir, float currentNum);
     void closeDraw();
     void restart_SaveCurve();


};

#endif // DRAWER_H
