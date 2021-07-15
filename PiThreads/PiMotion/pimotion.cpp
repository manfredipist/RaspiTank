#include "pimotion.h"

PiMotion::PiMotion(PiTCA9548A* piI2CMultiplexer, QObject *parent) : QObject(parent), piI2CMultiplexer(piI2CMultiplexer)
{
    if((pi=pigpio_start(NULL,NULL)<0))
        qDebug()<<"Something went wrong while initializing GPIO";

    fan = new PiFan(pi,en_fan,this);

    leftMotor = new PiStepperMotor(pi, en_a,in_1,in_2,this);
    rightMotor = new PiStepperMotor(pi, en_b,in_3,in_4,this);

    servoMotor = new PiServoMotor(pi, en_servo);
}

PiMotion::~PiMotion(){
    fan->toggleFan(false);
    rightMotor->stop();
    leftMotor->stop();

    fan->deleteLater();
    rightMotor->deleteLater();
    leftMotor->deleteLater();
    servoMotor->deleteLater();

    pigpio_stop(pi);
}

void PiMotion::stepForward(){
    qDebug()<<"Stepping forward..";
    moveForward(CELL_SIZE);
}

void PiMotion::stepBackward(){
    qDebug()<<"Stepping backward..";
    moveForward(CELL_SIZE);
}

void PiMotion::stepLeft(){
    qDebug()<<"Stepping left..";
    moveLeft(90);
    moveForward(CELL_SIZE);
}

void PiMotion::stepRight(){
    qDebug()<<"Stepping right..";
    moveRight(90);
    moveForward(CELL_SIZE);
}

void PiMotion::moveLeft(const int &degree){
    int starting_angle = piI2CMultiplexer->getAngle();
    int distance = 0;
    while(distance < degree){
        rightMotor->backward();
        leftMotor->forward();
        QThread::msleep(40);
        rightMotor->stop();
        leftMotor->stop();
        distance = qAbs(starting_angle-piI2CMultiplexer->getAngle());
        distance = qMin(distance, 360-distance);
        QThread::msleep(60);
    }
}

void PiMotion::moveRight(const int &degree){
    int starting_angle = piI2CMultiplexer->getAngle();
    int distance = 0;
    while(distance < degree){
        rightMotor->forward();
        leftMotor->backward();
        QThread::msleep(40);
        rightMotor->stop();
        leftMotor->stop();
        distance = qAbs(piI2CMultiplexer->getAngle() - starting_angle);
        distance = qMin(distance, 360-distance);
        QThread::msleep(60);
    }
}

void PiMotion::moveForward(const int &distance){
    int ms = 240/10 * distance;
    rightMotor->forward();
    leftMotor->forward();
    QThread::msleep(ms);
    rightMotor->stop();
    leftMotor->stop();
}

void PiMotion::moveBackward(const int &distance){
    int ms = 240/10 * distance;
    leftMotor->backward();
    rightMotor->backward();
    QThread::msleep(ms);
    rightMotor->stop();
    leftMotor->stop();
}

void PiMotion::surpassLeft(){
    stepLeft();
    stepRight();
}

void PiMotion::surpassRight(){
    stepRight();
    stepLeft();
}

void PiMotion::goForward(){
    if(notSafeToMove)
        qInfo()<<"Not safe to move!";
    else{
        leftMotor->forward();
        rightMotor->forward();
    }
}
void PiMotion::goBackward(){
    if(notSafeToMove)
        qInfo()<<"Not safe to move!";
    else{
        leftMotor->backward();
        rightMotor->backward();
    }
}
void PiMotion::goLeft(){
    if(notSafeToMove)
        qInfo()<<"Not safe to move!";
    else{
        rightMotor->forward();
        leftMotor->backward();
    }
}
void PiMotion::goSlowLeft(){
    if(notSafeToMove)
        qInfo()<<"Not safe to move!";
    else{
        rightMotor->forward();
        leftMotor->backward();
        QThread::msleep(40);
        rightMotor->stop();
        leftMotor->stop();
        QThread::msleep(60);
    }
}
void PiMotion::goRight(){
    if(notSafeToMove)
        qInfo()<<"Not safe to move!";
    else{
        rightMotor->backward();
        leftMotor->forward();
    }
}
void PiMotion::goSlowRight(){
    if(notSafeToMove)
        qInfo()<<"Not safe to move!";
    else{
        rightMotor->backward();
        leftMotor->forward();
        QThread::msleep(40);
        rightMotor->stop();
        leftMotor->stop();
        QThread::msleep(60);
    }
}
void PiMotion::stop(){
    rightMotor->stop();
    leftMotor->stop();
}

void PiMotion::setEmergencyStop(const bool &enabled){
    enabled ? notSafeToMove = true : notSafeToMove = false;
}

void PiMotion::executeMotion(QChar command){
    qDebug()<<"Executing motion:"<<command;
    switch(command.unicode()){
    case 'i':
        stop();
        break;
    case 'w':
        goForward();
        break;
    case 'a':
        goLeft();
        break;
    case 's':
        goBackward();
        break;
    case 'd':
        goRight();
        break; 
    case 'q':
        goSlowLeft();
        break;
    case 'e':
        goSlowRight();
        break;
//    case 'z':
//        setServoMotor(-45);
//        break;
//    case 'x':
//        setServoMotor(0);
//        break;
//    case 'c':
//        setServoMotor(45);
//        break;
    }
}

void PiMotion::toggleFan(const bool &enabled){
    fan->toggleFan(enabled);
}
