#include "monitor.h"

#include <QDebug>

/* 类名：monitor
 * 功能：控制主监视窗口（磁场数据显示）和串口数据显示窗口Log的功能
 *
 */


monitor::monitor(QWidget *parent) :
    QWidget(parent)
{
     LineEditLog = new QLineEdit;
     sumbuf = new QByteArray;


     this->setWindowTitle("Monitor NMEA Sentence");  //设置窗口标题

     button1=new QPushButton("打开串口");
     button2=new QPushButton("关闭串口");

     QObject::connect(button1,SIGNAL(clicked()),this,SLOT(on_pushButton_clicked()));
     QObject::connect(button2,SIGNAL(clicked()),this,SLOT(on_pushButton_2_clicked()));

     //设置相关信号与槽



//-------------------------------构造窗口------------------------------
     QGridLayout *grid = new QGridLayout;

     QByteArray Labels[10];

     Labels[0] = "Heading:";Labels[1] ="Deviation:"; Labels[2] ="Dev Status:";
     Labels[3] ="Variation:";Labels[4] ="Var Status:";
     grid->addWidget(createExclusiveGroup(5, Labels, "HDG Update Data", LineEditHDG), 0, 0);

     Labels[0] = "Heading:";
     grid->addWidget(createExclusiveGroup(1, Labels, "HDT Update Data", LineEditHDT), 0, 1);

     Labels[0] = "Pitch:";Labels[1] ="Roll:"; Labels[2] ="MagX:";
     Labels[3] ="MagY:";Labels[4] ="MagZ:";Labels[5] ="Mag Total:"; Labels[6] ="Heading:";
     grid->addWidget(createExclusiveGroup(7, Labels, "RCD Update Data", LineEditRCD), 0, 2);

     Labels[0] = "Heading:";Labels[1] ="Heading Status:"; Labels[2] ="Pitch:";
     Labels[3] ="Pitch Status:";Labels[4] ="Roll:";Labels[5] ="Roll Status:";
     Labels[6] ="Frame Counts:";
     grid->addWidget(createExclusiveGroup(7, Labels, "HPR Update Data", LineEditHPR), 1, 0);

     Labels[0] = "Heading:";
     grid->addWidget(createExclusiveGroup(1, Labels, "ASCII Update Data", LineEditASCII), 1, 1);

     Labels[0] = "TiltX:";Labels[1] ="TiltY:"; Labels[2] ="TiltY:";
     Labels[3] ="MagX:";     Labels[4] ="MagY:";Labels[5] ="MagZ:";Labels[6] ="Mag Total:";
     Labels[7] ="Heading:";
     grid->addWidget(createExclusiveGroup(8, Labels, "CCD Update Data", LineEditCCD), 1, 2);

 //    grid->addWidget(LineEditLog, 2, 1, 1, 2);
     grid->addWidget(button1, 2, 1);
     grid->addWidget(button2, 2, 2);


     setLayout(grid);

     setWindowTitle(tr("Monitor NMEA Sentence"));
     resize(480, 320);

}


QGroupBox *monitor::createExclusiveGroup(int NumOfLine, QByteArray Labels[], QByteArray Title, QLineEdit *LineEdit[])
{
    QGroupBox *groupBox = new QGroupBox(tr(Title));     //设置一个框
    QLabel **Label = new QLabel *[NumOfLine];                           //设置所有标签的数组
//    QLineEdit *LineEdit[NumOfLine];

    int i=0,ColumnNum=2,RowNum=1;

    QGridLayout *GLayout = new QGridLayout;               //设置框中的布局大小和结构

    GLayout->setRowStretch(0,0);                          //在布局的上边角落加弹簧
    GLayout->setColumnStretch(0,0);
    GLayout->setRowStretch(0,ColumnNum+1);
    GLayout->setColumnStretch(0,ColumnNum+1);

    while (i<=(NumOfLine-1))                             //将标签和文本框分别放到布局中
    {
        Label[i] = new QLabel(Labels[i]);
        LineEdit[i] = new QLineEdit;

        //qDebug()<<i;
        GLayout->addWidget(Label[i],RowNum,ColumnNum-1);
        GLayout->addWidget(LineEdit[i],RowNum++,ColumnNum);
        i++;
    }

    QPushButton *Query = new QPushButton("Query");          //在布局的底部放置按钮
    GLayout->addWidget(Query,RowNum++,ColumnNum);


    GLayout->setRowStretch(RowNum,0);                      //在布局的下边角落加弹簧
    GLayout->setColumnStretch(RowNum,0);
    GLayout->setRowStretch(RowNum,ColumnNum+1);
    GLayout->setColumnStretch(RowNum,ColumnNum+1);


    groupBox->setLayout(GLayout);                           //将布局放到框中


    return groupBox;
}


//--------------------------------打开并设置串口------------------------------------------
void monitor::on_pushButton_clicked()
{

    if(myserial->open(QIODevice::ReadWrite)==true) //读写方式打开串口
    qDebug()<<"1";
}

//**********************************关闭串口****************************************
void monitor::on_pushButton_2_clicked()
{
     myserial->close();  //关闭串口
}




extern int ViewlogState;
void monitor::openViewlog()   //在mainwindow里面调用此函数用来打开log窗口，监视输入的文本
{
        viewlog = new QDialog;
        Logwindow = new QGridLayout;
        LogTextBrowser = new QTextBrowser;
        buttoncloselog = new QPushButton("Close Log");
        buttonsavelog = new QPushButton("Save Log");

        QObject::connect(LogTextBrowser,SIGNAL(textChanged()),this,SLOT(settextBrowserscroll2()));

        QObject::connect(buttoncloselog,SIGNAL(clicked()),this, SLOT(closeViewlog()) );
        QObject::connect(viewlog,SIGNAL(rejected()),this,SLOT(closeViewlog()));

        QObject::connect(buttonsavelog,SIGNAL(clicked()),this,SLOT(Save_Log_triggered()));


        viewlog -> setLayout(Logwindow);                     //将对话框放入布局
        Logwindow -> addWidget(LogTextBrowser, 0, 0, 1, 2);
        Logwindow -> addWidget(buttoncloselog, 1, 0);
        Logwindow -> addWidget(buttonsavelog, 1, 1);

        viewlog -> setWindowFlags(Qt::WindowStaysOnTopHint);
        viewlog -> show();  //显示窗口

        viewlog -> setWindowTitle("View Log");    //设置窗口标题
        //QObject::connect(myserial,SIGNAL(readyRead()),this,SLOT(showdata(TempByteArry)));


        extern int currentScreenHeight, currentScreenWidth;
        viewlog -> setGeometry(currentScreenWidth/3*2 , currentScreenHeight/20, currentScreenWidth/4, currentScreenHeight/2);  //设置窗口位置

        ViewlogState = 1;         //设置窗口打开标志位
        *sumbuf = "";              //清空文本

}



void monitor::showdata(QByteArray TempByteArry )
{
    LogTextBrowser->insertPlainText(TempByteArry);
    *sumbuf+=TempByteArry;
}

void monitor::settextBrowserscroll2()
{
    LogTextBrowser->moveCursor(QTextCursor::End);
}


void monitor::closeViewlog()
{
  //qDebug() << ViewlogState;
  if(ViewlogState == 1)
    {
      viewlog -> close();
      ViewlogState = 0;
      //qDebug() << ViewlogState;
    }
}

void monitor::Save_Log_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this,"LogTextBrowser","","*.txt");//设置文件
    //filename+=".txt";
    if(!filename.isEmpty())
    {
       file=new QFile(filename);  //为文件取内存
       file->open(QFile::WriteOnly);  //打开文件
       file->write(*sumbuf);   //写文件
       file->close();    //关文件
    }

}
