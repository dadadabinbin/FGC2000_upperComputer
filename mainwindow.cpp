#include "mainwindow.h"

/* 类名：MainWindow
 * 功能：控制所有工具条以及主界面的内容和触发事件
 * 在此处创建：AddWidgets类，用于添加选择页面和信息处理
 */

//! [0]
MainWindow::MainWindow()     //
{
    this->setWindowState( Qt::WindowMaximized );
    addwidgets = new AddWidgets;      //添加部件addressWidget部件
    setCentralWidget(addwidgets);
    createMenus();
    setWindowTitle(tr("MiniCompass Demo"));
}

void MainWindow::createMenus()
{
    /*
    fileMenu = menuBar()->addMenu(tr("&File"));

    openAct = new QAction(tr("&Open..."), this);
    fileMenu->addAction(openAct);
 //   connect(openAct, &QAction::triggered, this, &MainWindow::openFile);
//! [1a]

    saveAct = new QAction(tr("&Save As..."), this);
    fileMenu->addAction(saveAct);
 //   connect(saveAct, &QAction::triggered, this, &MainWindow::saveFile);

    fileMenu->addSeparator();

    exitAct = new QAction(tr("E&xit"), this);
    fileMenu->addAction(exitAct);
    connect(exitAct, &QAction::triggered, this, &QWidget::close);

    toolMenu = menuBar()->addMenu(tr("&Tools"));

    addAct = new QAction(tr("&Add Entry..."), this);
    toolMenu->addAction(addAct);
//    connect(addAct, &QAction::triggered, addressWidget, &AddressWidget::showAddEntryDialog);

//! [1b]
    editAct = new QAction(tr("&Edit Entry..."), this);
    editAct->setEnabled(false);
    toolMenu->addAction(editAct);
 //   connect(editAct, &QAction::triggered, addressWidget, &AddressWidget::editEntry);

    toolMenu->addSeparator();

    removeAct = new QAction(tr("&Remove Entry"), this);
    removeAct->setEnabled(false);
    toolMenu->addAction(removeAct);
 //   connect(removeAct, &QAction::triggered, addressWidget, &AddressWidget::removeEntry);

 //   connect(addressWidget, &AddressWidget::selectionChanged,
  //      this, &MainWindow::updateActions);
*/
    fileMenu = menuBar()->addMenu(tr("&View Log"));                            //显示串口监视窗口
    openLog = new QAction(tr("&View Log"), this);
    fileMenu->addAction(openLog);
    connect(openLog, &QAction::triggered, addwidgets, &AddWidgets::MonitorLogopen);


    fileMenu = menuBar()->addMenu(tr("&Display Magnetic Field Curve "));                          //显示磁场曲线
    openXaxis = new QAction(tr("&X-axis Curve"), this);
    openYaxis = new QAction(tr("&Y-axis Curve"), this);
    openZaxis = new QAction(tr("&Z-axis Curve"), this);
    openMagCurve = new QAction(tr("&All 3-axes Curves"), this);
    displaySettings = new QAction(tr("&Display Settings"), this);

    pictureMenu=menuBar()->addMenu(tr("&Display compass"));
    compass=new QAction(tr("compass"),this);
    pitch=new QAction(tr("pitch"),this);
    pictureMenu->addAction(compass);
    pictureMenu->addAction(pitch);
    connect(compass, &QAction::triggered, addwidgets, &AddWidgets::CompassShow);
    connect(pitch, &QAction::triggered, addwidgets, &AddWidgets::PitchShow);



    fileMenu->addAction(openMagCurve);
    fileMenu->addAction(openXaxis);
    fileMenu->addAction(openYaxis);
    fileMenu->addAction(openZaxis);

    fileMenu->addSeparator();

    fileMenu->addAction(displaySettings);


    //connect(openXaxis, &QAction::triggered, addwidgets, SLOT(AddWidgets::ShowXCurve(1);));
    connect(openXaxis, &QAction::triggered, addwidgets, &AddWidgets::ShowXCurve);
    connect(openYaxis, &QAction::triggered, addwidgets, &AddWidgets::ShowYCurve);
    connect(openZaxis, &QAction::triggered, addwidgets, &AddWidgets::ShowZCurve);
    connect(openMagCurve, &QAction::triggered, addwidgets, &AddWidgets::ShowXYZCurve);

   // connect(openMagCurve, &QAction::triggered, this, &MainWindow::SerialSetting);

}





