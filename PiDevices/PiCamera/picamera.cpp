#include "picamera.h"

PiCamera::PiCamera(QObject *parent) : QObject(parent)
{

    raspiCam.set ( cv::CAP_PROP_FRAME_WIDTH, 640);
    raspiCam.set ( cv::CAP_PROP_FRAME_HEIGHT, 480);
//    raspiCam.set ( cv::CAP_PROP_ISO_SPEED, 100);
//    raspiCam.set ( cv::CAP_PROP_BRIGHTNESS, 55);
//    raspiCam.set ( cv::CAP_PROP_EXPOSURE, 0);//0 default
//    raspiCam.set ( cv::CAP_PROP_CONTRAST, 50);
//    raspiCam.set ( cv::CAP_PROP_SHARPNESS, 0);
//    raspiCam.set ( cv::CAP_PROP_SATURATION, 50);
//    raspiCam.set ( cv::CAP_PROP_GAIN, 50);
    raspiCam.set ( cv::CAP_PROP_FPS, 30);
    raspiCam.set ( cv::CAP_PROP_FORMAT, CV_8UC3);

    if (!raspiCam.open())
        qDebug()<<"Error opening the camera";
    else
        qDebug()<<"Connected to camera ="<<QString::fromStdString(raspiCam.getId());
    /*
    if(!cap.isOpened())  // check if we succeeded
        qDebug()<<"Error opening the camera";

    qDebug() << "Camera Init Successful\n";
    qDebug() << "Setting parameters..\n";


    qDebug() <<"[PARAM_FRAME_WIDTH] ";
    if(cap.set(CAP_PROP_FRAME_WIDTH,640))
         qDebug() <<"SUCCESS\n";
    else
         qDebug() <<"FAIL\n";


    qDebug() <<"[PARAM_FRAME_HEIGHT] ";
    if(cap.set(CAP_PROP_FRAME_HEIGHT,480))
         qDebug() <<"SUCCESS\n";
    else
         qDebug() <<"FAIL\n";


    qDebug() <<"[PARAM_FPS] ";
    if(cap.set(CAP_PROP_FPS,60))
         qDebug() <<"SUCCESS\n";
    else
         qDebug() <<"FAIL\n";

    qDebug() <<"[BUFFERSIZE] ";
    if(cap.set(CAP_PROP_BUFFERSIZE, 10))
         qDebug() <<"SUCCESS\n";
    else
         qDebug() <<"FAIL\n";

    int nFPS = cap.get(CAP_PROP_FPS);
    qDebug() << "Loaded FPS : " << nFPS << "\n";
    */

}

PiCamera::~PiCamera(){
    /*
    if (cap.isOpened())
        cap.release();
    */
    if (raspiCam.open())
        raspiCam.release();
}

Mat PiCamera::getVideoCaptureCV(){
    Mat image;
    raspiCam.grab();
    raspiCam.retrieve(image);

    //cap.read(image);

    return image;
}
