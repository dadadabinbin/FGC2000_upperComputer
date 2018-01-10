#ifndef MONITOR_H
#define MONITOR_H

#include <QWidget>
#include<QDialog>
#include<QTextBrowser>
#include<QHBoxLayout>
#include<QLineEdit>
#include<QSerialPort>
#include<QSerialPortInfo>
#include<QByteArray>
#include<QFile>
#include<QIODevice>
#include<QFileDialog>
#include <QRadioButton>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMenu>
#include <QGridLayout>
#include <QLabel>
#include <QDebug>
#include <QGroupBox>
#include  <QCloseEvent>


//namespace Ui {
//class monitor;
//}
extern  QSerialPort* myserial;

class monitor : public QWidget
{
    Q_OBJECT

public:
    explicit monitor(QWidget *parent = 0);
    //~monitor();

    QPushButton* button1;
    QPushButton* button2;


    QLineEdit *LineEditHDG[5], *LineEditHDT[1], *LineEditRCD[7], *LineEditHPR[7], *LineEditASCII[1],
              *LineEditCCD[8], *LineEditLog;

    QDialog *viewlog;
    QGridLayout *Logwindow;
    QTextBrowser *LogTextBrowser;
    //int* Desktopsize();
    //QByteArray* buf;
    QByteArray* sumbuf;
    QPushButton* buttoncloselog;
    QPushButton* buttonsavelog;
    QFile* file;


public slots:
    void openViewlog();
  //void readmyserial();
    void showdata(QByteArray TempByteArry);
    void settextBrowserscroll2();
    void closeViewlog();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void Save_Log_triggered();

private:
    //Ui::monitor *ui;
    QGroupBox *createExclusiveGroup(int NumOfLine, QByteArray Labes[], QByteArray Title, QLineEdit *LineEdit[]);



};

#endif
