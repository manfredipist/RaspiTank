#ifndef PISENSING_H
#define PISENSING_H

#include <QObject>
#include <QCoreApplication>
#include <QDebug>
#include <QString>
#include <QThread>

#include "PiDevices/PiI2CMultiplexer/pitca9548a.h"

#define SAFETY_THRESHOLD 5

class PiSensing : public QObject
{
    Q_OBJECT
public:
    explicit PiSensing(int delay, PiTCA9548A* piI2CMultiplexer, QObject *parent = nullptr);
    ~PiSensing();
public slots:
    void loop();
signals:
    void updateSensorData(double front_laser, double left_laser, double right_laser, double temperature, int angle, float accX, float accY, float accZ, float gyroX, float gyroY, float gyroZ);
    void notifyEmergencyStatus();
private slots:

private:
    const int delay;
    PiTCA9548A *piI2CMultiplexer;

    bool isCalibrating = false;
};

#endif // PISENSING_H
