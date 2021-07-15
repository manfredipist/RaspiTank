#ifndef PISTEPPERMOTOR_H
#define PISTEPPERMOTOR_H

#include <QObject>
#include <QDebug>
#include <QString>

#include <pigpiod_if2.h>

class PiStepperMotor : public QObject
{
    Q_OBJECT
public:
    explicit PiStepperMotor(const int &pi, const int &en, const int &in_1, const int &in_2, QObject *parent = nullptr);
    ~PiStepperMotor();
public slots:
    void forward();
    void backward();
    void stop();
signals:
private:
    const int pi;

    const int en;
    const int in_1;
    const int in_2;
};

#endif // PISTEPPERMOTOR_H
