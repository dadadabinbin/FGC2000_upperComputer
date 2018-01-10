#include "ShowCurve.h"
#include<QDebug>
#include<QFile>
#include<QTextStream>
#include<QDir>
#include<QFileInfoList>
#include<QFileInfo>
#include<QFile>
#include <QDialog>

extern int XwindowState;
extern int YwindowState;
extern int ZwindowState;
extern int XYZwindowState;

/* 类名：ShowCurve
 * 功能：控制绘图功能的参数，并且实例化绘图类Drawer,从addwigets接收数据放入X/Y/Z_DATA中
 *
 */


ShowCurve::ShowCurve(QWidget *parent) : QDialog(parent)
{

    //设置程序图标
    QPixmap mypixmap("Program_Group_128px_585002_easyicon.net.ico");
    QIcon myicon(mypixmap);
    this->setWindowIcon(myicon);

    /************************ShowCurve部分*****************************/
     Vlayout=new QVBoxLayout;
     formlayout=new QFormLayout;
     Hlayout=new QHBoxLayout;
     Hlayouttime=new QHBoxLayout;
     timelineedit=new QLineEdit;
     intervallineedit=new QLineEdit;
     timeproportionlineedit=new QLineEdit;
     dirlineedit=new QLineEdit;
     combobox=new QComboBox;
     combobox->addItem("s");
     combobox->addItem("min");
     combobox->addItem("h");
     buttonok=new QPushButton("OK");
     buttoncancel=new QPushButton("CANCEL");
     Hlayouttime->addWidget(combobox);
     Hlayouttime->addWidget(timelineedit);

     formlayout->addRow(QStringLiteral("sample time(number)："),Hlayouttime);
     formlayout->addRow(QStringLiteral("sample interval(ms)："), intervallineedit);
     formlayout->addRow(QStringLiteral("amplify proportion："), timeproportionlineedit);
     formlayout->addRow(QStringLiteral("filesavepath："), dirlineedit);
     Hlayout->addWidget(buttonok);
     Hlayout->addWidget(buttoncancel);
     Vlayout->addLayout(formlayout);
     Vlayout->addLayout(Hlayout);
     formlayout->setSpacing(10);
     Vlayout->setSpacing(100);
     Vlayout->setMargin(20);

     this->setLayout(Vlayout);
     this->setWindowTitle(tr("photo"));

     str1=new QByteArray;
     str2=new QByteArray;
     str3=new QByteArray;
     str4=new QByteArray;


     //读取配置文件
     QDir dir("../");
     dir.setFilter(QDir::Files);
     QFileInfoList list = dir.entryInfoList();
     for(int i=0;i<list.size();i++)
     {
          QFileInfo fileinfo=list.at(i);
         if(fileinfo.fileName()=="ini.ini")
         {
             //找到文件
             QString dir=fileinfo.path();
             dir+="/ini.ini";

             QFile file(dir);
             if(file.open(QIODevice::ReadOnly | QIODevice::Text))
             {
                QByteArray filecontent=file.readAll();
               // j=filecontent;
               // qDebug()<<filecontent<<endl;
                //搜索对应数字
                j=searchnum(filecontent,0,str1);
                k=searchnum(filecontent,j,str2);
                m=searchnum(filecontent,k,str3);
                searchnum(filecontent,m,str4);

                timelineedit->setText(*str1);
                intervallineedit->setText(*str2);
                timeproportionlineedit->setText(*str3);
                dirlineedit->setText(*str4);
             }
         }
     }



     QObject::connect(buttonok,SIGNAL(clicked()),this,SLOT(onbuttonokslot()));          //OK按钮触发打开操作
     QObject::connect(buttoncancel,SIGNAL(clicked()),this,SLOT(onbuttoncancelslot()));  //cancel按钮





     X_Data = new QLineEdit;
     Y_Data = new QLineEdit;
     Z_Data = new QLineEdit;

     ShowXcurve = new ShowCurveWindow;
     ShowYcurve = new ShowCurveWindow;
     ShowZcurve = new ShowCurveWindow;
     ThreeAxleWindow = new ThreeAxleCurve;

     //ShowXcurve -> LeftLayout -> addWidget(X_Data);

     /*
     ThreeAxleWindow -> MainLayout -> addWidget(X_Data, 4, 1);
     ThreeAxleWindow -> MainLayout -> addWidget(Y_Data, 6, 1);
     ThreeAxleWindow -> MainLayout -> addWidget(Z_Data, 8, 1);
     */

     //ThreeAxleWindow -> RightLayout -> addWidget(X_Data, 4);
     //ThreeAxleWindow -> RightLayout -> addWidget(Y_Data, 6);
     //ThreeAxleWindow -> RightLayout -> addWidget(Z_Data, 8);


     //formlayout->addWidget(X_Data);
     //formlayout->addWidget(Y_Data);
     //formlayout->addWidget(Z_Data);
     QObject::connect(X_Data,SIGNAL(textChanged(const QString &)),this,SLOT(LineEditorChange()));  //三轴数据是同步更新的，因此不需要多个信号
     //QObject::connect(Y_Data,SIGNAL(textChanged(const QString &)),this,SLOT(LineEditorChange()));
     //QObject::connect(Z_Data,SIGNAL(textChanged(const QString &)),this,SLOT(LineEditorChange()));


}

int ShowCurve::searchnum(QByteArray filecontent,int j,QByteArray* str)//找到所选数字
{
    for(int i=j;i<filecontent.size();i++)
    {
        if(filecontent.at(i)=='=')
        {
            do
            {
                 i=i+1;
                if(filecontent.at(i)=='\n')
                   break;
                str->push_back(filecontent.at(i));
               //qDebug()<<"str:"<<str<<"i:"<<i<<endl;
            }

            while(filecontent.at(i)!='\n');
          //qDebug()<<"str:"<<*str<<"i:"<<i<<endl;
            return i;
        }
    }
}

void ShowCurve:: onbuttonokslot()                           //判断配置文件内容是否符合规范，并且保存配置文件
{
     continuetime=timelineedit->text().toInt();
     interval=intervallineedit->text().toInt();
     proportion=timeproportionlineedit->text().toFloat();
     mydir=dirlineedit->text();
     timeselect=combobox->currentIndex();
     //这里设置正则表达式，检查输入的是否都是数字如果不是数字，则会报错
     //输入的数字最多不能超过9位
     QRegExp rx("[1-9]\\d{0,8}");
     if(rx.indexIn(timelineedit->text())==-1)
     {
        QMessageBox::warning(this,"warning!","the continuetime input is not correct! ",QMessageBox::Yes);
     }
     if(rx.indexIn(intervallineedit->text())==-1)
     {
        QMessageBox::warning(this,"warning!","the interval input is not correct! ",QMessageBox::Yes);
     }
     if(rx.indexIn(timeproportionlineedit->text())==-1)
     {
        QMessageBox::warning(this,"warning!","the proportion input is not correct! ",QMessageBox::Yes);
     }

    //先清除之前存在的配置文件，保持配置文件始终只有一份
    QDir dir("../");
    clearfile(dir);

    savefile();
    if(rx.indexIn(timelineedit->text())!=-1&&rx.indexIn(intervallineedit->text())!=-1&&rx.indexIn(timeproportionlineedit->text())!=-1)
    {

        if(XwindowState == 1)
        {
            ShowXcurve -> Single_axis_Drawer -> openDraw(interval,continuetime,proportion,timeselect,mydir,(X_Data -> text()).toFloat());
            ShowXcurve -> show();
            XwindowState = 0;   //防止多次开关同一个显示曲线
            ShowXcurve -> setWindowTitle("X-axle Data and Curve");
            ShowXcurve -> Single_axis_Drawer -> DataName = "/X-Data-";
            ShowXcurve -> Single_axis_Drawer -> PicName = "/X-Pic-";

        }
        if(YwindowState == 1)
        {
            ShowYcurve -> Single_axis_Drawer -> openDraw(interval,continuetime,proportion,timeselect,mydir,(Y_Data -> text()).toFloat());
            ShowYcurve -> show();
            ShowYcurve -> setWindowTitle("Y-axle Data and Curve");
            YwindowState = 0;
            ShowYcurve -> Single_axis_Drawer -> DataName = "/Y-Data-";
            ShowYcurve -> Single_axis_Drawer -> PicName = "/Y-Pic-";
        }
        if(ZwindowState == 1)
        {
            ShowZcurve -> Single_axis_Drawer -> openDraw(interval,continuetime,proportion,timeselect,mydir,(Z_Data -> text()).toFloat());
            ShowZcurve -> show();
            ShowZcurve -> setWindowTitle("Z-axle Data and Curve");
            ZwindowState = 0;
            ShowZcurve -> Single_axis_Drawer -> DataName = "/Z-Data-";
            ShowZcurve -> Single_axis_Drawer -> PicName = "/Z-Pic-";
        }
        if(XYZwindowState == 1)
        {
            ThreeAxleWindow -> X_axis_Drawer -> openDraw(interval,continuetime,proportion,timeselect,mydir,(X_Data -> text()).toFloat());
            ThreeAxleWindow -> Y_axis_Drawer -> openDraw(interval,continuetime,proportion,timeselect,mydir,(Y_Data -> text()).toFloat());
            ThreeAxleWindow -> Z_axis_Drawer -> openDraw(interval,continuetime,proportion,timeselect,mydir,(Z_Data -> text()).toFloat());
            ThreeAxleWindow -> show();
            ThreeAxleWindow -> setWindowTitle("XYZ-axle Data and Curve");
            XYZwindowState = 0;
            ThreeAxleWindow -> X_axis_Drawer -> DataName = "/X-Data-";
            ThreeAxleWindow -> X_axis_Drawer -> PicName = "/X-Pic-";
            ThreeAxleWindow -> Y_axis_Drawer -> DataName = "/Y-Data-";
            ThreeAxleWindow -> Y_axis_Drawer -> PicName = "/Y-Pic-";
            ThreeAxleWindow -> Z_axis_Drawer -> DataName = "/Z-Data-";
            ThreeAxleWindow -> Z_axis_Drawer -> PicName = "/Z-Pic-";




        }
        this ->close();

    }
}

void ShowCurve::clearfile(QDir dir)             //清除原有配置文件
{
    dir.setFilter(QDir::Files);
    QFileInfoList list = dir.entryInfoList();
    for(int i=0;i<list.size();i++)
    {
        QFileInfo fileinfo=list.at(i);
        if(fileinfo.fileName()=="ini.ini")
        {
              fileinfo.dir().remove(fileinfo.fileName());//删除指定文件
        }
        i++;
    }
}
void ShowCurve::savefile()                               //保存配置文件
{
    //保存配置文件
    QFile file("../ini.ini");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out<<"timelineedit="<<QString::number(continuetime)<<endl;
        out<<"intervallineedit="<<QString::number(interval)<<endl;
        out<<"timeproportionlineedit="<<QString::number(proportion)<<endl;
        out<<"dirlineedit="<<mydir<<endl;
    }
    file.close();
}
void ShowCurve:: onbuttoncancelslot()
{
    //xdrawer->closeDraw();
    this->close();
}



void ShowCurve::keyPressEvent(QKeyEvent *ev)
{
    //注意回车键是Key_Return,而不是Key_enter！！
    if(ev->key()==Qt::Key_Return)
    {
        //如果多次按下设置成无效
       if(ev->isAutoRepeat())
            return;
       emit buttonok->click();

    }
}

void ShowCurve::LineEditorChange()
{

     ShowXcurve-> Single_axis_Drawer -> displayDataprevious = (X_Data -> text()).toFloat();
     ShowYcurve-> Single_axis_Drawer -> displayDataprevious = (Y_Data -> text()).toFloat();
     ShowZcurve-> Single_axis_Drawer -> displayDataprevious = (Z_Data -> text()).toFloat();

     ShowXcurve-> DataBox -> setText(X_Data -> text());
     ShowYcurve-> DataBox -> setText(Y_Data -> text());
     ShowZcurve-> DataBox -> setText(Z_Data -> text());

     ThreeAxleWindow -> XData -> setText(X_Data -> text());
     ThreeAxleWindow -> YData -> setText(Y_Data -> text());
     ThreeAxleWindow -> ZData -> setText(Z_Data -> text());

     ThreeAxleWindow-> X_axis_Drawer -> displayDataprevious = (X_Data -> text()).toFloat();
     ThreeAxleWindow-> Y_axis_Drawer -> displayDataprevious = (Y_Data -> text()).toFloat();
     ThreeAxleWindow-> Z_axis_Drawer -> displayDataprevious = (Z_Data -> text()).toFloat();



    //qDebug()<< xdrawer ->displayDataprevious << X_Data -> text();
}


void ShowCurve::closeEvent(QCloseEvent *event)
{
    XwindowState = 0;
    YwindowState = 0;
    ZwindowState = 0;
    XYZwindowState = 0;

}

