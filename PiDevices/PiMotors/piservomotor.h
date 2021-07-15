#ifndef PISERVOMOTOR_H
#define PISERVOMOTOR_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QThread>

#include <pigpiod_if2.h>

class PiServoMotor : public QObject
{
    Q_OBJECT
public:
    explicit PiServoMotor(const int &pi, const int &en, QObject *parent = nullptr);
    ~PiServoMotor();
public slots:
    void setAngle(int angle);
    int getAngle();
    void span();
signals:
private slots:

private:
    const int pi;

    const int en;
    const int minPulseWidth = 1000;
    const int maxPulseWidth = 2000;
    const int pulseWidth = 1500;
    const int pulseWidth_inc = 25;

};

#endif // PISERVOMOTOR_H
