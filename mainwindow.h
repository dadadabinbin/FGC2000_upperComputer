
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QFileDialog>
#include <QMenuBar>
#include <QGridLayout>
#include "serialsetting.h"
#include "addwidgets.h"
//#include "monitor.h"

extern QSerialPort* myserial;


//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private slots:
   // void updateActions(const QItemSelection &selection);
    //void openFile();
   // void saveFile();
    //void SerialSetting();
    //void openViewlog();
    //void closeViewlog();


private:
    void createMenus();

  // AddressWidget *addressWidget;
    QMenu *fileMenu;
    QMenu *pictureMenu;
    QMenu *toolMenu;
    QAction *openAct;
    QAction *saveAct;
    QAction *exitAct;
    QAction *addAct;
    QAction *editAct;
    QAction *removeAct;
    QAction *openLog;
    QAction *openMagCurve;
    QAction *openXaxis;
    QAction *openYaxis;
    QAction *openZaxis;
    QAction *displaySettings;
    QAction *compass;
    QAction *pitch;

    //QAction *openSerialSetting;
    AddWidgets *addwidgets;
    //serialsetting SetSerial;

    //int* Desktopsize();
//    QByteArray* buf;
//    QByteArray* sumbuf;

};
//! [0]

#endif // MAINWINDOW_H
