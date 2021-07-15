#include "pisensing.h"

PiSensing::PiSensing(int delay, PiTCA9548A *piI2CMultiplexer, QObject *parent) : QObject(parent), delay(delay), piI2CMultiplexer(piI2CMultiplexer)
{

}


PiSensing::~PiSensing(){
    piI2CMultiplexer->deleteLater();
}

void PiSensing::loop(){
    forever
    {
        //qInfo()<<"Running piSensing thread: "<<QThread::currentThread();

        if(QThread::currentThread()->isInterruptionRequested())
            return;

        double front_laser = piI2CMultiplexer->getFrontLaser();
        double left_laser = piI2CMultiplexer->getLeftLaser();
        double right_laser = piI2CMultiplexer->getRightLaser();

        int angle = piI2CMultiplexer->getAngle();

        double temperature = piI2CMultiplexer->getTemperature();
        float accX, accY, accZ, gyroX, gyroY, gyroZ;
        piI2CMultiplexer->getMotion6(&accX, &accY, &accZ, &gyroX, &gyroY, &gyroZ);

        emit updateSensorData(front_laser, left_laser, right_laser, temperature, angle, accX, accY, accZ, gyroX, gyroY, gyroZ);

       if( (front_laser>0 && front_laser<SAFETY_THRESHOLD) || (left_laser>0 && left_laser<SAFETY_THRESHOLD) || (right_laser>0 && right_laser<SAFETY_THRESHOLD))
           emit notifyEmergencyStatus();

        QThread::msleep(delay);
    }
}


//NB: as a bad practise, you could force signals processing by invoking the following..
//QCoreApplication::processEvents();
