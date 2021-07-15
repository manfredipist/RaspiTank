#ifndef PICAMERA_H
#define PICAMERA_H

#include <QObject>
#include <QDebug>

#include <opencv2/opencv.hpp>
#include <raspicam/raspicam_cv.h>

using namespace cv;
using namespace raspicam;

class PiCamera : public QObject
{
    Q_OBJECT
public:
    explicit PiCamera(QObject *parent = nullptr);
    ~PiCamera();
public slots:
    Mat getVideoCaptureCV();
signals:

private slots:
private:
    RaspiCam_Cv raspiCam;
    //VideoCapture cap = VideoCapture(0, CAP_V4L2); // open the default camera
};

#endif // PICAMERA_H
