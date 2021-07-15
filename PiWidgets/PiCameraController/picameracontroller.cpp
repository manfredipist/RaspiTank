#include "picameracontroller.h"
#include "ui_picameracontroller.h"

PiCameraController::PiCameraController(QWidget *parent) : QWidget(parent), ui(new Ui::PiCameraController){
    ui->setupUi(this);
}

PiCameraController::~PiCameraController(){

}

QImage PiCameraController::matToQImage(const cv::Mat &mat)
{
    if(mat.channels() == 1) { // if grayscale image
        return QImage((uchar*)mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_Grayscale8).copy();
    }
    if(mat.channels() == 3) { // if 3 channel color image
        cv::Mat rgbMat;
        cv::cvtColor(mat, rgbMat, cv::COLOR_BGR2RGB); // invert BGR to RGB
        return QImage((uchar*)rgbMat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_RGB888).copy();
    }
    return QImage();
}

void PiCameraController::refreshFrame(cv::Mat &frame){
    QImage qFrame = matToQImage(frame);
    ui->label->setPixmap(QPixmap::fromImage(qFrame.copy()));
}

//GUI EVENTS

void PiCameraController::on_pushButton_spanCamera_clicked()
{
    emit spanCamera();
}

void PiCameraController::on_pushButton_centerCamera_clicked()
{
    emit setAngleCamera(0);
}

void PiCameraController::on_pushButton_turnRCamera_clicked()
{
    emit setAngleCamera(45);
}

void PiCameraController::on_pushButton_turnLCamera_clicked()
{
    emit setAngleCamera(-45);
}

void PiCameraController::on_radioButton_qrDetectionOn_clicked()
{
    qrDetectionEnabled = true;
}

void PiCameraController::on_radioButton_qrDetectionOff_clicked()
{
    qrDetectionEnabled = false;
}
