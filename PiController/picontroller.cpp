#include "picontroller.h"
#include "ui_picontroller.h"

PiController::PiController(QWidget *parent) : QMainWindow(parent), ui(new Ui::PiController){
    ui->setupUi(this);

    this->setFocusPolicy(Qt::StrongFocus);
    this->grabKeyboard();

    piMap = new PiMap('n',STARTX,STARTY,this);
    piCameraController = new PiCameraController(this);

    ui->stackedWidget->insertWidget(1,piMap);
    ui->stackedWidget->insertWidget(2,piCameraController);
    ui->stackedWidget->setCurrentIndex(0);


    piI2CMultiplexer = new PiTCA9548A(this);


    piMotion = new PiMotion(piI2CMultiplexer);
    piSensing = new PiSensing(150, piI2CMultiplexer);
    piVision = new PiVision(50, piCameraController);
    piPlanner = new PiPlanner(5000, piMap, piMotion);

    threadPiMotion = new QThread(this);
    piMotion->moveToThread(threadPiMotion);

    threadPiSensing = new QThread(this);
    piSensing->moveToThread(threadPiSensing);
    connect(threadPiSensing, &QThread::started, piSensing, &PiSensing::loop);
    connect(piSensing, &PiSensing::updateSensorData, this, &PiController::updateSensorData);
    connect(piSensing, &PiSensing::notifyEmergencyStatus, this, &PiController::handleEmergencyStatus);

    threadPiVision = new QThread(this);
    piVision->moveToThread(threadPiVision);
    connect(threadPiVision, &QThread::started, piVision, &PiVision::loop);

    threadPiPlanner = new QThread(this);
    piPlanner->moveToThread(threadPiPlanner);
    connect(threadPiPlanner, &QThread::started, piPlanner, &PiPlanner::loop);

    threadPiSensing->start();
    threadPiVision->start();
    //threadPiPlanner>start();
}

PiController::~PiController(){
    if(threadPiMotion->isRunning()){
        threadPiMotion->requestInterruption();
        threadPiMotion->quit();
        if(!threadPiMotion->wait(1000))
            threadPiMotion->terminate();
    }

    if(threadPiSensing->isRunning()){
        threadPiSensing->requestInterruption();
        threadPiSensing->quit();
        if(!threadPiSensing->wait(1000))
            threadPiSensing->terminate();
    }

    if(threadPiVision->isRunning()){
        threadPiVision->requestInterruption();
        threadPiVision->quit();
        if(!threadPiVision->wait(1000))
            threadPiVision->terminate();
    }

    if(threadPiPlanner->isRunning()){
        threadPiPlanner->requestInterruption();
        threadPiPlanner->quit();
        if(!threadPiPlanner->wait(1000))
            threadPiPlanner->terminate();
    }

    threadPiSensing->deleteLater();
    threadPiVision->deleteLater();
    threadPiPlanner->deleteLater();

    piMotion->deleteLater();
    piSensing->deleteLater();
    piVision->deleteLater();
    piPlanner->deleteLater();

    piI2CMultiplexer->deleteLater();

    piMap->deleteLater();
    piCameraController->deleteLater();
}

void PiController::updateSensorData(const double &front_laser, const double &left_laser, const double &right_laser, const double &temperature, const int &angle,
                                    const float &accX, const float &accY, const float &accZ, const float &gyroX, const float &gyroY, const float &gyroZ){
    piMap->updateObstaclesOnMap(front_laser,left_laser,right_laser);

    ui->label_frontLaser->setText(QString("%1 cm").arg(front_laser));
    ui->label_leftLaser->setText(QString("%1 cm").arg(left_laser));
    ui->label_rightLaser->setText(QString("%1 cm").arg(right_laser));
    ui->label_temperature->setText(QString("%1 C°").arg(temperature));
    ui->label_angle->setText(QString("%1 °").arg(angle));

    ui->label_accX->setText(QString("%1").arg(accX));
    ui->label_accY->setText(QString("%1").arg(accY));
    ui->label_accZ->setText(QString("%1").arg(accZ));
    ui->label_gyroX->setText(QString("%1").arg(gyroX));
    ui->label_gyroY->setText(QString("%1").arg(gyroY));
    ui->label_gyroZ->setText(QString("%1").arg(gyroZ));
}

void PiController::handleEmergencyStatus(){
    piMotion->setEmergencyStop(true);
    ui->pushButton_toggleSafetyState->setText("Disable Emergency Stop");
    ui->pushButton_toggleSafetyState->setStyleSheet("color: red;");
}

// GUI Events

void PiController::on_action_goToController_triggered(){
    ui->stackedWidget->setCurrentIndex(0);
}
void PiController::on_action_goToMap_triggered(){
    ui->stackedWidget->setCurrentIndex(1);
}
void PiController::on_action_goToCamera_triggered(){
    ui->stackedWidget->setCurrentIndex(2);
}

void PiController::on_stackedWidget_currentChanged(int index)
{
    if(index>-1)
        this->adjustSize();
}

void PiController::on_pushButton_toggleSafetyState_clicked()
{
    bool notSafetoMove = piMotion->getEmergencyStop();
    if(notSafetoMove){
        piMotion->setEmergencyStop(false);
        ui->pushButton_toggleSafetyState->setText("Enable Emergency Stop");
        ui->pushButton_toggleSafetyState->setStyleSheet("color: green;");
    }else{
        piMotion->setEmergencyStop(true);
        ui->pushButton_toggleSafetyState->setText("Disable Emergency Stop");
        ui->pushButton_toggleSafetyState->setStyleSheet("color: red;");
    }
}

void PiController::on_radioButton_fanOn_clicked()
{
    piMotion->toggleFan(true);
}

void PiController::on_radioButton_fanOff_clicked()
{
    piMotion->toggleFan(false);
}

// Key Events

void PiController::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_W){
        piMotion->executeMotion('i');
    }else if(event->key() == Qt::Key_A){
        piMotion->executeMotion('i');
    }else if(event->key() == Qt::Key_S){
        piMotion->executeMotion('i');
    }else if(event->key() == Qt::Key_D){
        piMotion->executeMotion('i');
    }
}

void PiController::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_W){
        piMotion->executeMotion('w');
    }else if(event->key() == Qt::Key_A){
        piMotion->executeMotion('a');
    }else if(event->key() == Qt::Key_S){
        piMotion->executeMotion('s');
    }else if(event->key() == Qt::Key_D){
        piMotion->executeMotion('d');
    }else if(event->key() == Qt::Key_Q){
        piMotion->executeMotion('q');
    }else if(event->key() == Qt::Key_E){
        piMotion->executeMotion('e');
    }else if(event->key() == Qt::Key_Z){
        piMotion->executeMotion('z');
    }else if(event->key() == Qt::Key_X){
        piMotion->executeMotion('x');
    }else if(event->key() == Qt::Key_C){
        piMotion->executeMotion('c');
    }
}
