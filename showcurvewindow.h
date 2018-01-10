#ifndef SHOWCURVEWINDOW_H
#define SHOWCURVEWINDOW_H

#include <QWidget>
#include <QGridLayout>
#include <QCloseEvent>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include "Drawer.h"

class ShowCurveWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ShowCurveWindow(QWidget *parent = 0);
    void openDraw(int interval, int continuetime, float proportion,int timeselect, QString mydir, float currentNum);
    QGridLayout* MainLayout;
    QVBoxLayout* LeftLayout;
    Drawer* Single_axis_Drawer;
    QLineEdit* DataBox;

private:
    QPushButton* RestartButton;
    QLabel* MagNum;
    QSize sizeHint() const;
    void closeEvent(QCloseEvent *event);
    int currentScreenWid;
    int currentScreenHei;

signals:


public slots:
    void Restart();

};

#endif // SHOWCURVEWINDOW_H
