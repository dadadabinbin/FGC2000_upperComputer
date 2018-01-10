#include "ShowCurveWindow.h"
#include <QWidget>
#include <QGridLayout>
#include <QCloseEvent>
#include <QPushButton>
#include <QLabel>
#include "Drawer.h"

ShowCurveWindow::ShowCurveWindow(QWidget *parent) : QWidget (parent)
{
    QDesktopWidget* desktopWidget = new QDesktopWidget;

    QRect screenRect = desktopWidget->screenGeometry();

    currentScreenWid = screenRect.width();

    currentScreenHei = screenRect.height();

    RestartButton = new QPushButton("Restart");
    MainLayout = new QGridLayout;
    Single_axis_Drawer = new Drawer;
    MagNum = new QLabel(" Magnetic "
                        "Field Data");
    LeftLayout = new QVBoxLayout;
    DataBox = new QLineEdit;
    setLayout(MainLayout);

    LeftLayout -> addStretch(2);
    LeftLayout -> addWidget(RestartButton);
    LeftLayout -> addWidget(MagNum);
    LeftLayout -> addWidget(DataBox);
    LeftLayout -> addStretch(2);

    MainLayout -> addWidget(Single_axis_Drawer, 0, 0, 4, 1);
    MainLayout -> addLayout(LeftLayout, 0, 1, 4, 1);
    //MainLayout -> addWidget(RestartButton, 0, 1, 1, 1);
    //MainLayout -> addWidget(MagNum, 1, 1, 1, 1);

    //RestartButton -> setGeometry(QRect(currentScreenWid/10, currentScreenHei/30, 300, 200));
    MainLayout -> setColumnStretch( 0, 10);
    MainLayout -> setColumnStretch( 1, 1);

    connect(RestartButton, &QPushButton::clicked, this, &ShowCurveWindow::Restart);

}

void ShowCurveWindow::openDraw(int interval, int continuetime, float proportion, int timeselect, QString mydir, float currentNum)
{
      Single_axis_Drawer -> openDraw(interval,continuetime,proportion,timeselect,mydir,currentNum);
}

void ShowCurveWindow::closeEvent(QCloseEvent *event)
{
    Single_axis_Drawer -> Trigger = 0;
    Single_axis_Drawer -> flag=false;            //关闭时钟标志位
    Single_axis_Drawer -> sampletime->stop();    //采样停止
    Single_axis_Drawer -> killTimer(Single_axis_Drawer->myTimerId);  //关闭当前时钟
    Single_axis_Drawer -> restart();

}

QSize ShowCurveWindow::sizeHint() const            //关键字const，自动运行
 {
     return QSize( currentScreenWid/1.5, currentScreenHei/3 );
 }

void ShowCurveWindow::Restart()
{
    //Single_axis_Drawer -> timecount = 0;
    Single_axis_Drawer -> Trigger = 0;
    Single_axis_Drawer -> CurrentNum = Single_axis_Drawer -> displayDataprevious;
    Single_axis_Drawer -> restart();
    Single_axis_Drawer -> openDraw(Single_axis_Drawer -> myinterval,Single_axis_Drawer -> mycontinuetime/Single_axis_Drawer -> timemultiple,Single_axis_Drawer -> myproportion,Single_axis_Drawer -> mytimeselect,Single_axis_Drawer -> savedir, Single_axis_Drawer -> CurrentNum);
}
