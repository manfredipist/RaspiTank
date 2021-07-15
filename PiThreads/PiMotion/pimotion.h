#ifndef PIMOTION_H
#define PIMOTION_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QThread>
#include <QtMath>

#include <pigpiod_if2.h>

#include "PiDevices/PiI2CMultiplexer/pitca9548a.h"

#include "PiDevices/PiMotors/pisteppermotor.h"
#include "PiDevices/PiMotors/piservomotor.h"
#include "PiDevices/PiMotors/pifan.h"

#define CELL_SIZE 10

class PiMotion : public QObject
{
    Q_OBJECT
public:
    explicit PiMotion(PiTCA9548A* piI2CMultiplexer, QObject *parent = nullptr);
    ~PiMotion();
public slots:
    void stepForward();
    void stepBackward();
    void stepLeft();
    void stepRight();

    void moveLeft(const int& degree);
    void moveRight(const int& degree);
    void moveForward(const int &distance);
    void moveBackward(const int &distance);

    void surpassLeft();
    void surpassRight();


    void setEmergencyStop(const bool &enabled);
    bool getEmergencyStop()const{return notSafeToMove;}

    void executeMotion(QChar command);

    void toggleFan(const bool &enabled);

signals:
    void setAngle(int);
    void spanServo();
private slots:
    void goForward();
    void goBackward();
    void goLeft();
    void goSlowLeft();
    void goRight();
    void goSlowRight();
    void stop();

private:
    PiTCA9548A* piI2CMultiplexer;

    int pi;
    bool notSafeToMove = false;

    //Fan
    PiFan* fan;
    const int en_fan = 18;

    //Stepper motors
    PiStepperMotor *leftMotor;
    const int en_a = 21;
    const int in_1 = 16;
    const int in_2 = 20;

    PiStepperMotor *rightMotor;
    const int en_b = 13;
    const int in_3 = 26;
    const int in_4 = 19;

    //Servo motor
    PiServoMotor *servoMotor;
    const int en_servo = 14;

};

#endif // PITRACKS_H
