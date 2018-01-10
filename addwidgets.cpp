#include "addwidgets.h"
#include <QWidget>
#include <QTabWidget>
#include <QQmlApplicationEngine>

extern int XwindowState;
extern int YwindowState;
extern int ZwindowState;
extern int XYZwindowState;


/* 类名：AddWidgets
 * 功能：用于添加 磁场数据 标签页，串口数据设置标签页，并处理串口发送到这里的数据，
 * 实例化：monitor, serialsettings, ShowCurve
 * 并且控制这些窗口的开关
 */


AddWidgets::AddWidgets(QWidget *parent)
    : QTabWidget(parent)
{
    show_Curve = new ShowCurve;
    //show_X_Curve = new ShowCurve;
   // show_Y_Curve = new ShowCurve;
   // show_Z_Curve = new ShowCurve;
    buf=new QByteArray;
    QObject::connect(myserial,SIGNAL(readyRead()),this,SLOT(readmyserial()));
    AddMonitorTable();  //磁场数据显示标签页
    AddSerialSetting();  //串口设定标签页
//    AddCompassShow();   //设置罗盘显示
    quickview = new QQuickView;
    viewpitch = new QQuickView;
//    timer.setInterval(50);
//    timer.start();
//    QObject::connect(&timer,SIGNAL(timeout()),this,SLOT(ontimeout()));

}
//qreal AddWidgets::ontimeout()
//{
//    a++;
//    return a;
//    // qDebug()<<"m_angle="<<m_angle*1000<<endl;
//   // return m_angle*1000;
//}


void AddWidgets::AddMonitorTable()
{

    monitortable = new monitor(this);
    addTab(monitortable, "磁场数据显示");

}


void AddWidgets::AddSerialSetting()
{

    SetSerial = new serialsetting(this);
    addTab(SetSerial, "改变串口设置");

}

void AddWidgets::CompassShow()
{
//      QQmlEngine engine;
//      QQmlComponent component(&engine, QUrl::fromLocalFile("C:/Users/a1941/Desktop/qtproject/Demo0222/DEMO/main.qml"));
//      QObject *myObject = component.create();
//      QQuickItem *item = qobject_cast<QQuickItem*>(myObject);
//      quickview=new QQuickView;
//      ApplicationData data;
//      qucickView->rootContext()->setContextProperty("applicationData",&data);
//      quickview->setSource(QUrl::fromLocalFile("../DEMO/main.qml"));
//      QQuickItem *item = qobject_cast<QQuickItem *>(component.create());
//      addTab(widget, "罗盘显示");
//     view = new QQuickView;
//     view->setSource(QUrl("../Demo/main.qml"));
//    AddWidgets data;

    /* edit by BriFuture */
    quickview->rootContext()->setContextProperty("dataSource", this);
    quickview->setSource(QUrl(QStringLiteral("qrc:/main.qml")));
//    quickview->setTitle("Heading Angle");
//    quickview->show();
    /* =======end======= */
}

void AddWidgets::PitchShow()
{
    /* edit by BriFuture */
//    viewpitch->setSource(QUrl::fromLocalFile("../DEMO/Pitch.qml"));
    viewpitch->setSource(QUrl(QStringLiteral("qrc:/Pitch.qml")));
    viewpitch->rootContext()->setContextProperty("dataSource", this);
    viewpitch->show();
    viewpitch->setTitle("Pitch and Roll");
    /* =======end======= */
}

/** Edit By BriFuture
 * analogy data input
*/
qreal AddWidgets::getData() {

    return m_angle;
}
qreal AddWidgets::getPitch() {

    return m_pitch;
}
qreal AddWidgets::getRoll()
{
    return m_roll;
}

void AddWidgets::MonitorLogclose()   //关闭log窗口
{
    monitortable -> closeViewlog();

}

void AddWidgets::MonitorLogopen()   //关闭log窗口
{
    monitortable -> openViewlog();
}

void AddWidgets::ShowXCurve()
{
        XwindowState = 1;
        show_Curve -> setWindowFlags(Qt::WindowStaysOnTopHint);
        show_Curve -> show();

        //QObject::connect(show_X_Curve,SIGNAL(rejected()),this,SLOT(closeXCurve()));  //无效
}
void AddWidgets::ShowYCurve()
{
        YwindowState = 1;
        show_Curve -> setWindowFlags(Qt::WindowStaysOnTopHint);
        show_Curve -> show();

        //QObject::connect(show_Y_Curve,SIGNAL(rejected()),this,SLOT(closeYCurve()));
}

void AddWidgets::ShowZCurve()
{

        ZwindowState = 1;
        show_Curve -> setWindowFlags(Qt::WindowStaysOnTopHint);
        show_Curve -> show();
        //QObject::connect(show_Z_Curve,SIGNAL(rejected()),this,SLOT(closeZCurve()));
}

void AddWidgets::ShowXYZCurve()
{

        XYZwindowState = 1;
        show_Curve -> setWindowFlags(Qt::WindowStaysOnTopHint);
        show_Curve -> show();
        //QObject::connect(show_Z_Curve,SIGNAL(rejected()),this,SLOT(closeZCurve()));
}

/*以下为数据处理部分：将串口接收到的数据进行分类，并将其分配到各个显示区域
 */

//*********取出两个逗号之间的数据，输入TempByteArry数组，上一次指针停留处 *i，整个数组的长度*******
QByteArray AddWidgets::GetDataBetweenComas(QByteArray TempByteArry,int* i,int SizeOfTempByteArry)
{

    QByteArray DataBetweenComas;
    int s;
    for( s= (*i)+1;TempByteArry[(*i)+1]!=','&&*i<SizeOfTempByteArry&&TempByteArry[(*i)+1]!='\r';(*i)++);
    DataBetweenComas = TempByteArry.mid(s,(*i)-s+1);
    (*i)++;
    //qDebug()<<*i<<"数组长度为："<<(*i)-s<<endl<<"数据为："<<DataBetweenComas<<endl;
    return DataBetweenComas;
}
//*********取出当前地址到任意符号之前的数据，输入TempByteArry数组，上一次指针停留处 *i，整个数组的长度，和需要查找的符号*******
QByteArray AddWidgets::GetDataBetweenComas(QByteArray TempByteArry,int* i,int SizeOfTempByteArry,char j)
{

    QByteArray DataBetweenComas;
    int s;
    for( s= (*i)+1;TempByteArry[(*i)+1]!=j&&*i<SizeOfTempByteArry&&TempByteArry[(*i)+1]!='\r';(*i)++);
    DataBetweenComas = TempByteArry.mid(s,(*i)-s+1);
    (*i)++;
    //qDebug()<<*i<<"数组长度为："<<(*i)-s<<endl<<"数据为："<<DataBetweenComas<<endl;
    //qDebug()<<"目标符号为："<<j;
    return DataBetweenComas;
}

int AddWidgets::NMEA_0183_hh(QByteArray TempByteArry) //求每个字符的异或值 NMEA_0183通讯标准
{
    int SizeOfData=TempByteArry.size();
    if (TempByteArry[0]=='$')
    {

        char charhh = '\0';
        for(int i=1; i<=SizeOfData&&TempByteArry[i]!='*'; i++)
        {
            charhh = TempByteArry[i]^charhh;
        }
        int hh = charhh;
        return hh;
    }
    else
    {
        qDebug()<<"It's not a NMEA 0183 Code!!!";
        return 0;
    }
}

//**********************************读串口******************************************
void AddWidgets::readmyserial()   //读串口
{

    *buf = myserial->readLine();

//   int Tcount;
//   if(Tcount == 1)
//   {
//       *buf="$PTNTCCD,79,-122,-12027,-4912,1381,13064,287.9,N*2F\r\n" ;
//       Tcount = 0;
//    }
//   else
//   {
//       *buf="$PTNTCCD,89,-132,-12037,-4922,1391,13074,287.9,N*2F\r\n" ;
//       Tcount = 1;
//   }

//  *buf="$HCHDG,296.6,0.0,E,-0.1,W*77\r\n";
//  *buf="96.6,0.0,E,-0.1,W*77\r\n";
//    qDebug()<<"2";



    TempByteArry+=*buf;                        //拼接连续几行的数据

    SizeOfTempByteArry=TempByteArry.size();




    tempbuf=*buf;
    SizeOfBuf=tempbuf.size();

    //qDebug()<<"3";

   if(tempbuf[SizeOfBuf-1]=='\n')                     //如果不是换行符，则等待下一行拼接
    {
       //将数据放入LineEditLog，并将LineEditLog与monitor中的函数showdata相连接
       extern int ViewlogState;
       if (ViewlogState == 1)
       {
            monitortable -> showdata(TempByteArry );
            //QObject::connect(monitortable -> LineEditLog,SIGNAL(textchanged(const QString &)),monitortable,SLOT(showdata(*buf )));
       }
       //
       //********************************将数据的头部保存起来，并提取$之后的所有数据，存为新的字符串***************

       for(SizeOfHead=0;TempByteArry[SizeOfHead]!='$'
           &&SizeOfHead<SizeOfTempByteArry;SizeOfHead++);  //截掉头部$之前的数据


       if(SizeOfHead!=0)                                   //如果头部不为（$），将头部存起来
       {
           Temphead=TempByteArry.mid(0,SizeOfHead);
           //qDebug()<<"Temphead为："<<Temphead<<endl;
       }

       if(SizeOfHead!=SizeOfTempByteArry)                   //如果头部不占据整个字符串，则将其截掉，并保存新的字符串
       {
          TempByteArry=TempByteArry.mid(SizeOfHead);      //将数据头部去掉，并调整为一个新的字符串
          SizeOfTempByteArry-=SizeOfHead;                 //将数组的长度记录调整为改变后的
       }
       else
       {
           qDebug()<<"There is no MessageHeader, it is an ASCII Code";
       }

      // qDebug()<<"TempByteArry数组的长度为"<<SizeOfTempByteArry<<endl<<"TempByteArry内的数据为："<<TempByteArry<<endl;


       //************取出报文头（MessageHeader）并保存报文头的长度（SizeOfMessageHeader）*****

       for(SizeOfMessageHeader = 0; TempByteArry[SizeOfMessageHeader] != ','
           &&SizeOfMessageHeader<SizeOfTempByteArry; SizeOfMessageHeader++);
       MessageHeader = TempByteArry.mid(0,SizeOfMessageHeader);
       //qDebug()<<"报文头："<<MessageHeader<<endl;
       JudgeCommand();

   }
}

void  AddWidgets::JudgeCommand()   //判断命令
{
       //------------------------------------------HDT-------------------------------------------

       if(MessageHeader == "$HCHDT")
       {
           //qDebug()<<"HDT";
           for (i=0;i<=1;i++)                             //1个数据有用，最后一个可能为校验位
           {
           OutputData[i] = GetDataBetweenComas(TempByteArry,&SizeOfMessageHeader,SizeOfTempByteArry);
           }
           monitortable -> LineEditHDT[0]->setText(OutputData[0]+"\r\n");       //特殊情况，只输出一个数
       }



       //------------------------------------------CCD-----------------------------------------


       else if(MessageHeader == "$PTNTCCD")              //7个数据有用，最后一个可能为校验位,带*号
       {
           //qDebug()<<"CCD";
           for (i=0;i<=6;i++)
           {
           OutputData[i] = GetDataBetweenComas(TempByteArry,&SizeOfMessageHeader,SizeOfTempByteArry);
           monitortable -> LineEditCCD[i]->setText(OutputData[i]+"\r\n");
           }
           OutputData[i] = GetDataBetweenComas(TempByteArry,&SizeOfMessageHeader,SizeOfTempByteArry,'*');
           monitortable -> LineEditCCD[i]->setText(OutputData[i]+"\r\n");

           show_Curve -> X_Data -> setText(OutputData[3]);   //将X轴的数据放入画图窗口中的对话框
           show_Curve -> Y_Data -> setText(OutputData[4]);   //将Y轴的数据放入画图窗口中的对话框
           show_Curve -> Z_Data -> setText(OutputData[5]);   //将Z轴的数据放入画图窗口中的对话框
           m_angle=qAtan2(OutputData[4].toDouble(), OutputData[3].toDouble())*57.2957805;
           m_pitch=qAtan2(OutputData[5].toDouble(), OutputData[3].toDouble())*57.2957805;
           m_roll=qAtan2(OutputData[5].toDouble(), OutputData[4].toDouble())*57.2957805;


       }



       //------------------------------------------HDG-------------------------------------------


       else if(MessageHeader == "$HCHDG")              //4个数据有用，最后一个可能为校验位
       {
           //qDebug()<<"HDG";
           for (i=0;i<=3;i++)
           {
           OutputData[i] = GetDataBetweenComas(TempByteArry,&SizeOfMessageHeader,SizeOfTempByteArry);
           monitortable -> LineEditHDG[i]->setText(OutputData[i]+"\r\n");
           }
           OutputData[i] = GetDataBetweenComas(TempByteArry,&SizeOfMessageHeader,SizeOfTempByteArry,'*');
           monitortable -> LineEditHDG[i]->setText(OutputData[i]+"\r\n");

       }

       //-------------------------------------------HPR------------------------------------------


       //qDebug()<<getHPR;
       else if(MessageHeader == "$PTNTHPR")
       {
           //qDebug()<<"HPR";
           for (i=0;i<=6;i++)
           {
           OutputData[i] = GetDataBetweenComas(TempByteArry,&SizeOfMessageHeader,SizeOfTempByteArry);
           monitortable -> LineEditHPR[i]->setText(OutputData[i]+"\r\n");
           }
       }


       //--------------------------------------------RCD------------------------------------------


       else if(MessageHeader == "$PTNTRCD")              //5个数据有用，最后一个可能为校验位
       {
           //qDebug()<<"RCD";
           for (i=0;i<=5;i++)     //for语句i++最后还会被执行一次，相当于添加到函数体的最末尾
           {
           OutputData[i] = GetDataBetweenComas(TempByteArry,&SizeOfMessageHeader,SizeOfTempByteArry);
           monitortable -> LineEditRCD[i]->setText(OutputData[i]+"\r\n");
           }
           OutputData[i] = GetDataBetweenComas(TempByteArry,&SizeOfMessageHeader,SizeOfTempByteArry,'*');
           monitortable -> LineEditRCD[i]->setText(OutputData[i]+"\r\n");
       }
       //-----------------------------------其它情况，将整个数组输出---------------------------------
       else
       {   //qDebug()<<"ASCII:"<<Temphead;
           OutputData[0]=Temphead;
           monitortable -> LineEditASCII[0]->setText(OutputData[0]);

       }



       TempByteArry="";                                 //清空数组，准备下一次接受数据


 //最外边if的
}



