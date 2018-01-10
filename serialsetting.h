#ifndef SERIALSETTING_H
#define SERIALSETTING_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QGridLayout>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QIODevice>
#include <QRect>
#include <QDesktopWidget>
#include <QString>



extern  QSerialPort* myserial;

class serialsetting : public QDialog
{
    Q_OBJECT


public:
    serialsetting(QWidget *parent = 0);



private slots:
    void SetSerial();

    /*
    void transaction();
    void showResponse(const QString &s);
    void processError(const QString &s);
    void processTimeout(const QString &s);

*/
private:
    void setControlsEnabled(bool enable);

private:

    QLabel *otherSettings;
    QLabel *serialPortLabel;
    QComboBox *Baudrates;
    QComboBox *serialPortComboBox;
    QLabel *baudrateLabel;
    QPushButton *runButton;
    QPushButton *closeButton;

    QSize sizeHint() const;

};

#endif // SERIALSETTING_H
