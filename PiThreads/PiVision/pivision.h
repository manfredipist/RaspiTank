#ifndef PIVISION_H
#define PIVISION_H

#include <QObject>
#include <QDebug>
#include <QThread>

#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>

#include "PiDevices/PiCamera/picamera.h"
#include "PiWidgets/PiCameraController/picameracontroller.h"

using namespace std;
using namespace cv;

class PiVision : public QObject
{
    Q_OBJECT
public:
    explicit PiVision(int delay, PiCameraController* piCameraController, QObject *parent = nullptr);
    ~PiVision();
signals:
public slots:
    void loop();
private slots:
    void setupCVWindow();

    void drawFPS(Mat &color_image, double fps);
    void drawQRCodeContour(Mat &color_image, const vector<Point>& corners);
    void drawQRCodeResults(Mat& frame, const vector<Point>& corners, double fps);
    double processQRCodeDetection(QRCodeDetector& qrcode, const Mat& input, Mat& result, vector<Point>& corners);
private:
    const int delay;
    PiCameraController* piCameraController;
    PiCamera* piCamera;

    int iLowH = 0;
    int iHighH = 179;

    int iLowS = 0;
    int iHighS = 255;

    int iLowV = 0;
    int iHighV = 255;

    int thresh = 100;

    const  int gLHue = 65;
    const  int gHHue = 85;
    const  int gLSaturation = 80;
    const  int gHSaturation = 180;
    const  int gLValue = 40;
    const  int gHValue = 180;
};

#endif // PIVISION_H
