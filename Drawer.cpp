#include "Drawer.h"
#include<QPainter>

#include"math.h"
#include<QTextStream>
#include<QDebug>
#include<QPixmap>
#include<QIcon>



/* 类名：Drawer
 * 功能：控制磁场曲线绘制功能
 * displayDataprevious从外界获取磁场数值用于显示
 */


Drawer::Drawer(QWidget *parent) : QWidget(parent)
{
   WidthFactor = 8/11;   //修正装入外部layout后的差值
   //this->setWindowTitle("curvephoto");
   mydatetime=new QDateTime;

   //记录当前时间，这个变量设置是为了时间不被更新，能够一次确认
   timecount=0;
   xdivide=1.0/15.0;

   //因为其他量是float型变量，所以这个最好也是float类型变量，避免隐式类型转换时候出错
   xposmove= 0;

   //记录y轴放大比例，设置float类型，使其可以放大和缩小都可以
   myproportion=1.0;

   //仿真起始参数
   textnum=0.0;

   //记录初始数值
   CurrentNum = 0.0;

   //这两个变量是记录最初窗口的大小，使得这个程序可以正常运行在任何电脑上
   //注意这里要乘以0.8！！！！！！因为记录的是x轴的长度而不是整个窗口的长度
    currentwidth=this->width()*0.8;
    currenthight=this->height();

   sampletime=new QTimer;

   //设置程序图标
   QPixmap mypixmap("image_128px_1202981_easyicon.net.ico");
   QIcon myicon(mypixmap);
   this->setWindowIcon(myicon);

   QObject::connect(sampletime,SIGNAL(timeout()),this,SLOT(restart_SaveCurve()));

   //this->setFixedSize(QSize(800,600));

   //line.append(QPoint(0,0));
   displayDataprevious = 0;
   DataName = "/data-";
   PicName = "/pic-";

   Trigger = 0;    //初始化时间事件标志
}

void Drawer::paintEvent(QPaintEvent *event)
//void Drawer::paintCurve()
{
     QPainter painter(this);
     //设置背景颜色为灰色
     painter.fillRect(rect(),Qt::darkGray);
     //设置笔颜色字体，字体可调
     painter.setPen(QPen(Qt::black,3));
     //进行坐标轴变换,坐标原点设置到图像中央
     //由于图像大小是动态变化的，所以这个不能用一个变量去代替this.width()和this.height()
     painter.translate(this->width()*0.1,this->height()*0.9);
     //画坐标轴
     painter.setRenderHint(QPainter::Antialiasing);//消除锯齿
     painter.drawLine(0,0,this->width()*0.8,0);
     painter.drawLine(0,0,0,-this->height()*0.8);
     //画坐标轴末端箭头
     drawarrowhead(QPoint(this->width()*0.85,0.0),QPoint(this->width()*0.8,0.0),painter);
     drawarrowhead(QPoint(0.0,-this->height()*0.85),QPoint(0.0,-this->height()*0.8),painter);

     if(flag==true && Trigger == 1 )
     {
         //写出当前时间,设置时间只显示一次，避免重绘时被更新
         if(timecount==0)
         {
              time=mydatetime->currentDateTime().toString("yyyy-MM-dd-hh时mm分ss秒");
         }
         //输出图像顶端的当前时间
         painter.drawText(this->width()*0.3,-this->height()*0.85,time);
         timecount++;
         //标横纵轴
         //painter.drawText(this->width()*0.8,this->height()*0.05,QString::number(mycontinuetime/timemultiple)+"mins");
         //画箭头处的时间数值（例如 100s）
         painter.drawText(this->width()*0.8,this->height()*0.05,QString::number(mycontinuetime/timemultiple)+xtime);
         //画纵坐标的标尺(例如 ×1000nT )
         painter.drawText(QRect(QPoint(-this->width()*0.07,-this->height()*0.93),QSize(50,50)),Qt::AlignCenter,"×"+QString::number(myproportion)+"nT");
         //画坐标虚线

         //画图像曲线
         drawCurves(painter);
     }
     drawGrid(painter);
}
void Drawer::drawCurves(QPainter &painter)
{
    painter.setPen(Qt::red);
     //使得时间正确显示
     //qDebug()<<this->width()*0.8;
     // line.append(QPoint(this->width()*0.8*timeproportion*xposmove,-displayDataprevious/myproportion));
    //注意这里x,y两个值都需要乘以一开始的窗口大小
    line.append(QPointF(currentwidth*timeproportion*(xposmove -1 ),-(displayDataprevious - CurrentNum)/myproportion/100.0*0.4*currenthight - 0.4 * currenthight));
    //此处 (xposmove - 3.2) 是由于前4个数据不合适因此需要抛弃，根据调试结果，3.2比较合适
    //displayDataprevious/myproportion/100.0*0.4*currenthight  是由于 100个高度是当前窗口的0.4倍；- 0.4 * currenthight需要将0点移动到0的位置

    // line.push_front(QPointF(currentwidth*timeproportion*xposmove,-displayDataprevious/myproportion));
    //这里有一个问题，对于vector容器，这里第二个数字和第三个数字，总是line.at(),1 QPoint(10,-342103360)
    //line.at() 2 QPoint(10,-342103360)，所以这里做一个处理
    /***这个第二个数字和第三个数字问题依然存在，不知道是不是由于伪随机数导致****/
    for(int i=1;i<line.size();i++)//注意这里要减一，不然内存越界-1
    {
         QPointF PreviousPoint,ThisPoint;
         PreviousPoint=line.at(i-1);
         ThisPoint=line.at(i);
         //注意后面所相乘的是相对与原来窗口的比例，原来窗口的测量在构造函数中，这样使得程序可以运行在
         //不同分辨率的电脑
         PreviousPoint.setX(PreviousPoint.rx()*this->width()*8/10/currentwidth);
         PreviousPoint.setY(PreviousPoint.ry()*this->height()/currenthight);

         ThisPoint.setX(ThisPoint.rx()*this->width()*8/10/currentwidth);
         ThisPoint.setY(ThisPoint.ry()*this->height()/currenthight);

         painter.drawLine(PreviousPoint,ThisPoint);

    }
}
void Drawer::drawGrid(QPainter &painter)
{
      //画x轴
      for(int x=1;x<16;x++)
      {
          painter.setPen(Qt::DashLine);
          painter.drawLine(x*this->width()*0.8*xdivide,-this->height()*0.8,x*this->width()*0.8*xdivide,0);
          painter.setPen(Qt::SolidLine);
          painter.drawLine((x-0.5)*this->width()*0.8*xdivide,0,(x-0.5)*this->width()*0.8*xdivide,-10);
      }
      //画x轴时间标识
      painter.setPen(QPen(Qt::black,3));
      painter.drawText(this->width()*0.4,20,"TIME(S)");
      //画y轴

      for(int y=-5;y<=5;y++)
      {
          painter.setPen(Qt::DashLine);
          painter.drawLine(0,-y*this->height()*0.8*0.1-this->height()*0.4,this->width()*0.8,-y*this->height()*0.8*0.1-this->height()*0.4);
          painter.setPen(QPen(Qt::black,3));
          ystrnum=QString::number(y*20);
          if(y>=0)
          painter.drawText(-20,-y*this->height()*0.8*0.1-this->height()*0.4,ystrnum);
          else
          painter.drawText(-30,-y*this->height()*0.8*0.1-this->height()*0.4,ystrnum);

      }
      //画0点处虚线
      painter.setPen(Qt::DashLine);
      painter.drawLine(0,-this->height()*0.4,this->width()*0.8,-this->height()*0.4);
      painter.setPen(QPen(Qt::black,3));
      painter.drawText(-20,-this->height()*0.4,"0");
}

void Drawer::timerEvent(QTimerEvent *event)
{
    if(flag==true)
    {
        if(event->timerId()==myTimerId)
        {
            Trigger = 1;
            update();
            //displayDataprevious=textnum;
            //displayDataprevious =(monitortable -> LineEditCCD[3] -> text()) -> toInt();
            textnum+=1;
            //x轴移动的变量得放在这里，如果放在画图像处就会导致每次重绘时都发生变化
            xposmove+=1.0;
            //paintCurve();
        }
        else
        {
            //Trigger = 0;
            QWidget::timerEvent(event);
        }
    }

}
void Drawer::openDraw(int interval, int continuetime, float proportion,int timeselect, QString mydir, float currentNum)
{
    //设置文件保存路径
    savedir=mydir;
    //使得屏幕可以重绘
    flag=true;
    //设置总的采样时长
    timemultiple=mytimeselcet(timeselect);
    mycontinuetime=timemultiple*continuetime;
    //设置最后写入文件的时间
    showtime=continuetime;
    //承接变量，使得变量作用域从局部变量变为全局变量，从而实现类之间的参数传递
    mytimeselect=timeselect;
    myproportion=proportion;

    myinterval=interval;

    //开启采样间隔定时器
    myTimerId=startTimer(interval);
    //让x轴的时间变化正确
    timeproportion=(float)interval/(float)mycontinuetime;

    //开启采样总时长定时器
    sampletime->setInterval(mycontinuetime+ 2 * interval);//此处需要增加4个时间间隔，否则会出现短缺
    sampletime->start();

    CurrentNum = currentNum; //记录初始数值，并将屏幕0点设置为此

    //update();
}

void Drawer::closeDraw()
{
    flag=false;
    timecount=0;
    killTimer(myTimerId);
    sampletime->stop();
    //update();
}


//利用多边形和几何知识画三角形箭头
void Drawer::drawarrowhead(QPoint lastPoint,QPoint endPoint,QPainter &painter)
{

    float x1 = lastPoint.x();         //lastPoint 起点
    float y1 = lastPoint.y();
    float x2 = endPoint.x();           //endPoint 终点
    float y2 = endPoint.y();
    float l = 10.0;                   //箭头的那长度
    float a = 0.5;                       //箭头与线段角度
    float x3 = x2 - l * cos(atan2((y2 - y1) , (x2 - x1)) - a);
    float y3 = y2 - l * sin(atan2((y2 - y1) , (x2 - x1)) - a);
    float x4 = x2 - l * sin(atan2((x2 - x1) , (y2 - y1)) - a);
    float y4 = y2 - l * cos(atan2((x2 - x1) , (y2 - y1)) - a);

    painter.drawLine(x1,y1,x3,y3);
    painter.drawLine(x1,y1,x4,y4);
    painter.drawLine(lastPoint,endPoint);

}

void Drawer::restart_SaveCurve()
{
    SaveCurve();
    restart();
    openDraw(myinterval,mycontinuetime/timemultiple,myproportion,mytimeselect,savedir, CurrentNum);
    Trigger = 0;
}



void Drawer::restart()
{
      //清理容器缓存
#if 1
 //注意这里在for循环中千万千万不要将it迭代器加一！！！！！！！！！！因为erase()返回值会自动将it加一
   for(QVector<QPointF>::iterator it=line.begin();it<line.end();)
   {
       //注意pop_front不能将Vector变空，这个不是栈模型
      // line.pop_front();
       line.erase(it);
   }
#endif
#if 0
   for(QVector<QPointF>::iterator it=line.begin();it<line.end();it++)
   {
       qDebug()<<*it<<endl;
   }
#endif
   //不能用clear这个成员变量，因为他会让Vector释放内存，使得不能再用
   //Removes all the elements from the vector and releases the memory used by the vector.
    //line.clear();
   //为了使得第二次绘制的时候还是从原点开始，这里必须对xposmove重新设置初值！！！！
    xposmove = 0;
    //关闭定时器
   // killTimer(myTimerId);
    //使得屏幕重绘停止
    flag=false;
    //使时间重新确定
    timecount = 0;

}

void Drawer::SaveCurve()
{
    //qDebug()<<"hello world"<<endl;

    //截图
    /*************************文件名称不能有”：“号*********************************************/
    QPixmap pixmap;
    pixmap=this->grab(QRect(0,0,this->width(),this->height()));
    QString pic1=savedir;
    //QString pic2="/pic-";
    QString pic2= PicName;
    QString pic3=".png";
    pic1+=pic2;
    pic1+=time;
    pic1+=pic3;
    pixmap.save(pic1);
    QString str1=savedir;
    //QString str2="/data-";
    QString str2= DataName;
    QString str3=".txt";
    str1+=str2;
    str1+=time;
    str1+=str3;
    /*最好是截图完了再去关闭定时器*/
    //停止而不是关闭两个定时器
    sampletime->stop();
    //开启采样间隔定时器
    killTimer(myTimerId);
    QFile myfile(str1);
    if (myfile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&myfile);
        //用QString不用QByteArray避免乱码
        QString samplemessage="采样间隔:";
         samplemessage=samplemessage+QString::number(myinterval)+"ms";
        QString comtinuemessage="采样时长:";
        if(timemultiple==1000)
          comtinuemessage=comtinuemessage+QString::number(showtime)+"s";
        else if(timemultiple==60000)
          comtinuemessage=comtinuemessage+QString::number(showtime)+"min";
        else if(timemultiple==3600000)
          comtinuemessage=comtinuemessage+QString::number(showtime)+"h";
        QString ymessage="Magnetic Field Strangth:";
         //ymessage=ymessage+QString::number(20*myproportion)+"nT/刻度";
           ymessage=ymessage+"nT";
        out<<samplemessage<<endl<<comtinuemessage<<endl<<ymessage<<endl;
    #if 1
        for(QVector<QPointF>::iterator it=line.begin();it<line.end();it++)
        {
            //-(displayDataprevious - CurrentNum)/myproportion/100.0*0.4*currenthight - 0.4 * currenthight
            //out<<it->y()<<endl;
            out<<  - ( ( it->y()) /0.4 /currenthight +1) *100 *myproportion + CurrentNum <<endl;
        }
    #endif
    }
      //关闭文件，避免发生错误
       myfile.close();
}

long int Drawer::mytimeselcet(int timeselect)
{
    if(timeselect==0)
    {
        xtime="s";
        return 1000;
    }
    else if(timeselect==1)
    {
        xtime="min";
        return 60000;
    }
    else if(timeselect==2)
    {
        xtime="h";
        return 3600000;
    }
}

void Drawer::mouseDoubleClickEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::LeftButton)
    {
        //设置双击全屏
        if(windowState() != Qt::WindowMaximized)
            setWindowState(Qt::WindowMaximized);
    }

}
/*
void Drawer::closeEvent(QCloseEvent *event)
{
    flag=false;
    timecount=0;
    killTimer(myTimerId);
    sampletime->stop();
}
*/

QSize Drawer::sizeHint() const            //关键字const，自动运行
 {
    QDesktopWidget* desktopWidget = new QDesktopWidget;

    QRect screenRect = desktopWidget->screenGeometry();

    int currentScreenWid = screenRect.width();

    int currentScreenHei = screenRect.height();
     return QSize( currentScreenWid/3, currentScreenHei/3 );
 }



