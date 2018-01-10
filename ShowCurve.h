#ifndef ShowCurve_H
#define ShowCurve_H

#include <QWidget>
#include<QPushButton>
#include<QVBoxLayout>
#include<QFormLayout>
#include<QHBoxLayout>
#include<QLineEdit>
#include<QDir>
#include<QComboBox>
//#include<QMainWindow>
#include<QCloseEvent>
#include"Drawer.h"
#include"ShowCurveWindow.h"
#include"ThreeAxleCurve.h"

class ShowCurve : public QDialog
{
    Q_OBJECT
public:
    explicit ShowCurve(QWidget *parent = 0);
    Drawer* xdrawer;
    Drawer* ydrawer;
    Drawer* zdrawer;

    QVBoxLayout* Vlayout;
    QFormLayout* formlayout;
    QHBoxLayout* Hlayout;
    QHBoxLayout* Hlayouttime;
    QLineEdit* timelineedit;
    QLineEdit* intervallineedit;
    QLineEdit* timeproportionlineedit;
    //设置文件保存路径
    QLineEdit* dirlineedit;
    QPushButton* buttonok;
    QPushButton* buttoncancel;
    QComboBox* combobox;

    int continuetime;
    int interval;
    float proportion;
    QString mydir;
    //这个变量用于判断时间的单位
    int timeselect;


    QByteArray* str1;//最上面空格中的量
    QByteArray* str2;
    QByteArray* str3;
    QByteArray* str4;
    int j;//第二循环的循环控制变量
    int k;
    int m;


    int searchnum(QByteArray filecontent,int j,QByteArray* str);
    void clearfile(QDir dir);
    void savefile();

    //按下enter就直接运行图像
    void keyPressEvent(QKeyEvent *ev);
    QLineEdit* X_Data;
    QLineEdit* Y_Data;
    QLineEdit* Z_Data;
    float TempXData;
    QString TempXString;

    QWidget* DawerWindow;
    QPushButton* RestartSimple;

    ShowCurveWindow* ShowXcurve;
    ShowCurveWindow* ShowYcurve;
    ShowCurveWindow* ShowZcurve;
    ThreeAxleCurve* ThreeAxleWindow;
   void closeEvent(QCloseEvent *event );

signals:



public slots:
   void  onbuttonokslot();
   void  onbuttoncancelslot();
   void  LineEditorChange();
};

#endif // UI_H
