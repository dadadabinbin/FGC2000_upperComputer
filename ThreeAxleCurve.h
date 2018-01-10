#ifndef THREEAXLECURVE_H
#define THREEAXLECURVE_H

#include <QWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QCloseEvent>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include "Drawer.h"


class ThreeAxleCurve : public QWidget
{
    Q_OBJECT
public:
    explicit ThreeAxleCurve(QWidget *parent = 0);
    void openDraw(int interval, int continuetime, float proportion,int timeselect, QString mydir, float currentNum);
    QGridLayout* MainLayout;
    QVBoxLayout* LeftLayout;
    QVBoxLayout* RightLayout;

    Drawer* X_axis_Drawer;
    Drawer* Y_axis_Drawer;
    Drawer* Z_axis_Drawer;

    QLineEdit* XData;
    QLineEdit* YData;
    QLineEdit* ZData;



private:
    QPushButton* RestartButton;
    QLabel* XMagNum;
    QLabel* YMagNum;
    QLabel* ZMagNum;
    QSize sizeHint() const;
    void closeEvent(QCloseEvent *event);
    int currentScreenWid;
    int currentScreenHei;


signals:


public slots:
    void Restart();
};

#endif // THREEAXLECURVE_H
