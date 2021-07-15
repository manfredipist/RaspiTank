#include "piservomotor.h"

PiServoMotor::PiServoMotor(const int &pi, const int &en, QObject *parent) : QObject(parent), pi(pi), en(en)
{
    setAngle(0);
}

PiServoMotor::~PiServoMotor()
{
    set_servo_pulsewidth(pi, en, 0);
}

void PiServoMotor::setAngle(int angle){
    switch(angle){
        case 45:
            set_servo_pulsewidth(pi, en, 1000);
            break;
        case 30:
            set_servo_pulsewidth(pi, en, 1150);
            break;
        case 0:
            set_servo_pulsewidth(pi, en, 1400);
            break;
        case -30:
            set_servo_pulsewidth(pi, en, 1750);
            break;
        case -45:
            set_servo_pulsewidth(pi, en, 2000);
            break;
        default:
            set_servo_pulsewidth(pi, en, 1500);
            break;
    }
    QThread::msleep(100);
    set_servo_pulsewidth(pi, en, 0);
}

int PiServoMotor::getAngle(){
    int pw = get_servo_pulsewidth(pi, en);
    int angle = -1;
    switch(pw){
        case 1000:
            angle=45;
            break;
        case 1150:
            angle=30;
            break;
        case 1400:
            angle=0;
            break;
        case 1750:
            angle=-30;
            break;
        case 2000:
            angle=-45;
            break;
        case 1500:
            angle=-1;
            break;
    }
    return angle;
}

void PiServoMotor::span(){
    for(int i=minPulseWidth;i<maxPulseWidth;i+=pulseWidth_inc){
        set_servo_pulsewidth(pi, en, i);
        QThread::msleep(100);
    }
    QThread::msleep(1000);
    for(int i=maxPulseWidth;i>minPulseWidth-pulseWidth_inc;i-=pulseWidth_inc){
        set_servo_pulsewidth(pi, en, i);
        QThread::msleep(100);
    }
}
