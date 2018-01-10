#include "ThreeAxleCurve.h"
#include <QGroupBox>

ThreeAxleCurve::ThreeAxleCurve(QWidget *parent) : QWidget(parent)
{

    QDesktopWidget* desktopWidget = new QDesktopWidget;
    QRect screenRect = desktopWidget->screenGeometry();
    currentScreenWid = screenRect.width();
    currentScreenHei = screenRect.height();

    RestartButton = new QPushButton("Restart");

    MainLayout = new QGridLayout;
    LeftLayout = new QVBoxLayout;
    RightLayout = new QVBoxLayout;

    X_axis_Drawer = new Drawer;
    Z_axis_Drawer = new Drawer;
    Y_axis_Drawer = new Drawer;

    XData = new QLineEdit;
    YData = new QLineEdit;
    ZData = new QLineEdit;


    XMagNum = new QLabel(" X-axie Data");
    YMagNum = new QLabel(" Y-axie Data");
    ZMagNum = new QLabel(" Z-axie Data");


    MainLayout -> addWidget(X_axis_Drawer, 0, 0, 1, 1);
    MainLayout -> addWidget(Y_axis_Drawer, 1, 0, 1, 1);
    MainLayout -> addWidget(Z_axis_Drawer, 2, 0, 1, 1);

    QGroupBox* XBox = new QGroupBox;
    QGroupBox* YBox = new QGroupBox;
    QGroupBox* ZBox = new QGroupBox;


    QGridLayout* XLayout =new QGridLayout;
    QGridLayout* YLayout =new QGridLayout;
    QGridLayout* ZLayout =new QGridLayout;

    XLayout -> setRowStretch(0, 1);
    XLayout -> addWidget(RestartButton, 1, 0);
    XLayout -> setRowStretch(2, 1);
    XLayout -> addWidget(XMagNum, 3, 0);
    XLayout -> addWidget(XData, 4, 0);
    XLayout -> setRowStretch(5, 1);

    YLayout -> setRowStretch(0, 1);
    YLayout -> addWidget(YMagNum, 1, 0);
    YLayout -> addWidget(YData, 2, 0);
    YLayout -> setRowStretch(3, 1);

    ZLayout -> setRowStretch(0, 1);
    ZLayout -> addWidget(ZMagNum, 1, 0);
    ZLayout -> addWidget(ZData, 2, 0);
    ZLayout -> setRowStretch(3, 0);
    ZLayout -> setRowStretch(4, 1);

    XBox -> setLayout(XLayout);
    YBox -> setLayout(YLayout);
    ZBox -> setLayout(ZLayout);

    MainLayout -> addWidget(XBox, 0, 1);
    MainLayout -> addWidget(YBox, 1, 1);
    MainLayout -> addWidget(ZBox, 2, 1);



    MainLayout -> setColumnStretch( 0, 10);
    MainLayout -> setColumnStretch( 1, 1);

    MainLayout -> setRowStretch( 0, 1);
    MainLayout -> setRowStretch( 1, 1);
    MainLayout -> setRowStretch( 2, 1);



    setLayout(MainLayout);
    setWindowTitle("3-Axis Curve");


    connect(RestartButton, &QPushButton::clicked, this, &ThreeAxleCurve::Restart);

}

void ThreeAxleCurve::openDraw(int interval, int continuetime, float proportion, int timeselect, QString mydir, float currentNum)
{
      X_axis_Drawer -> openDraw(interval,continuetime,proportion,timeselect,mydir,currentNum);
      Y_axis_Drawer -> openDraw(interval,continuetime,proportion,timeselect,mydir,currentNum);
      Z_axis_Drawer -> openDraw(interval,continuetime,proportion,timeselect,mydir,currentNum);
}

void ThreeAxleCurve::closeEvent(QCloseEvent *event)
{
      X_axis_Drawer -> Trigger = 0;
      X_axis_Drawer -> flag=false;
      X_axis_Drawer -> sampletime->stop();
      X_axis_Drawer -> killTimer(X_axis_Drawer->myTimerId);
      X_axis_Drawer -> restart();

      Y_axis_Drawer -> Trigger = 0;
      Y_axis_Drawer -> flag=false;
      Y_axis_Drawer -> sampletime->stop();
      Y_axis_Drawer -> killTimer(Y_axis_Drawer->myTimerId);
      Y_axis_Drawer -> restart();

      Z_axis_Drawer -> Trigger = 0;
      Z_axis_Drawer -> flag=false;
      Z_axis_Drawer -> sampletime->stop();
      Z_axis_Drawer -> killTimer(Z_axis_Drawer->myTimerId);
      Z_axis_Drawer -> restart();


}

QSize ThreeAxleCurve::sizeHint() const            //关键字const，自动运行
 {
     return QSize(currentScreenWid*0.9, currentScreenHei*0.9);
 }

void ThreeAxleCurve::Restart()
{

     X_axis_Drawer -> Trigger = 0;
     X_axis_Drawer -> CurrentNum = X_axis_Drawer -> displayDataprevious;
     X_axis_Drawer -> restart();
     X_axis_Drawer -> openDraw(X_axis_Drawer -> myinterval,X_axis_Drawer -> mycontinuetime/X_axis_Drawer -> timemultiple,X_axis_Drawer -> myproportion,X_axis_Drawer -> mytimeselect,X_axis_Drawer -> savedir, X_axis_Drawer -> CurrentNum);

     Y_axis_Drawer -> Trigger = 0;
     Y_axis_Drawer -> CurrentNum = Y_axis_Drawer -> displayDataprevious;
     Y_axis_Drawer -> restart();
     Y_axis_Drawer -> openDraw(Y_axis_Drawer -> myinterval,Y_axis_Drawer -> mycontinuetime/Y_axis_Drawer -> timemultiple,Y_axis_Drawer -> myproportion,Y_axis_Drawer -> mytimeselect,Y_axis_Drawer -> savedir, Y_axis_Drawer -> CurrentNum);

     Z_axis_Drawer -> Trigger = 0;
     Z_axis_Drawer -> CurrentNum = Z_axis_Drawer -> displayDataprevious;
     Z_axis_Drawer -> restart();
     Z_axis_Drawer -> openDraw(Z_axis_Drawer -> myinterval,Z_axis_Drawer -> mycontinuetime/Z_axis_Drawer -> timemultiple,Z_axis_Drawer -> myproportion,Z_axis_Drawer -> mytimeselect,Z_axis_Drawer -> savedir, Z_axis_Drawer -> CurrentNum);
}
