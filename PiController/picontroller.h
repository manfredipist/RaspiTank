#ifndef PICONTROLLER_H
#define PICONTROLLER_H

#include <QMainWindow>
#include <QDebug>
#include <QKeyEvent>
#include <QMetaType>
#include <QThread>
#include <QTimer>

#include <pigpiod_if2.h>

#include "PiWidgets/PiMap/pimap.h"
#include "PiWidgets/PiCameraController/picameracontroller.h"

#include "PiDevices/PiI2CMultiplexer/pitca9548a.h"

#include "PiThreads/PiMotion/pimotion.h"
#include "PiThreads/PiVision/pivision.h"
#include "PiThreads/PiSensing/pisensing.h"
#include "PiThreads/PiPlanner/piplanner.h"

namespace Ui {
class PiController;
}

class PiController : public QMainWindow
{
    Q_OBJECT
public:
    explicit PiController(QWidget *parent = nullptr);
    ~PiController();
protected:
    void keyReleaseEvent(QKeyEvent *event);
    void keyPressEvent(QKeyEvent *event);
signals:
    void calibrateMagnetometer();
    void calibrateGyroAccelerometer();
private slots:
    void updateSensorData(const double &front_laser, const double &left_laser, const double &right_laser, const double &temperature, const int &angle, const float &accX, const float &accY, const float &accZ, const float &gyroX, const float &gyroY, const float &gyroZ);

    void handleEmergencyStatus();

    void on_action_goToController_triggered();
    void on_action_goToMap_triggered();
    void on_action_goToCamera_triggered();
    void on_stackedWidget_currentChanged(int index);

    void on_pushButton_toggleSafetyState_clicked();

    void on_radioButton_fanOn_clicked();
    void on_radioButton_fanOff_clicked();
private:
    Ui::PiController *ui;

    PiMap* piMap;
    PiCameraController *piCameraController;

    PiTCA9548A* piI2CMultiplexer;

    QThread* threadPiMotion;
    QThread* threadPiSensing;
    QThread* threadPiVision;
    QThread* threadPiPlanner;

    PiMotion* piMotion;
    PiSensing* piSensing;
    PiVision* piVision;
    PiPlanner* piPlanner;
};

#endif // PICONTROLLER_H
