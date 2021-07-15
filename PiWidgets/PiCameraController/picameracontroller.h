#ifndef PICAMERACONTROLLER_H
#define PICAMERACONTROLLER_H

#include <QWidget>
#include <QDebug>
#include <QMetaType>
#include <QThread>

#include <opencv2/imgproc.hpp>

namespace Ui {
    class PiCameraController;
}

class PiCameraController : public QWidget
{
    Q_OBJECT
public:
    explicit PiCameraController(QWidget *parent = nullptr);
    ~PiCameraController();
public slots:
    QImage matToQImage(const cv::Mat &mat);
    void refreshFrame(cv::Mat &frame);
    bool isQRDetectionEnabled()const{return qrDetectionEnabled;}
signals:
    void spanCamera();
    void setAngleCamera(int);
private slots:
    void on_pushButton_spanCamera_clicked();
    void on_pushButton_centerCamera_clicked();
    void on_pushButton_turnRCamera_clicked();
    void on_pushButton_turnLCamera_clicked();
    void on_radioButton_qrDetectionOn_clicked();
    void on_radioButton_qrDetectionOff_clicked();
private:
    Ui::PiCameraController *ui;
    bool qrDetectionEnabled = false;

};

#endif // PICAMERACONTROLLER_H
