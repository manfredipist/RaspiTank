#include "pivision.h"

PiVision::PiVision(int delay, PiCameraController *piCameraController, QObject *parent) : QObject(parent), delay(delay), piCameraController(piCameraController)
{
    piCamera = new PiCamera(this);
    //setupCVWindow()
}

PiVision::~PiVision(){
    piCamera->deleteLater();
}

void PiVision::setupCVWindow(){
    namedWindow("Control", WINDOW_AUTOSIZE); //create a window called "Control"

    //Create trackbars in "Control" window
    createTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
    createTrackbar("HighH", "Control", &iHighH, 179);

    createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
    createTrackbar("HighS", "Control", &iHighS, 255);

    createTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
    createTrackbar("HighV", "Control", &iHighV, 255);
}

void PiVision::drawQRCodeContour(Mat &color_image, const vector<Point>& corners)
{
    if (!corners.empty() && corners.size() % 4 == 0)
    {
        double show_radius = (color_image.rows  > color_image.cols)
                   ? (2.813 * color_image.rows) / color_image.cols
                   : (2.813 * color_image.cols) / color_image.rows;
        double contour_radius = show_radius * 0.4;

        vector< vector<Point> > contours;
        contours.push_back(corners);
        drawContours(color_image, contours, 0, Scalar(211, 0, 148), cvRound(contour_radius));

        RNG rng(1000);
        for (size_t i = 0; i < 4; i++)
        {
            Scalar color = Scalar(rng.uniform(0,255), rng.uniform(0, 255), rng.uniform(0, 255));
            circle(color_image, corners[i], cvRound(show_radius), color, -1);
        }
    }
}

void PiVision::drawFPS(Mat &color_image, double fps)
{
    ostringstream convert;
    convert << cv::format("%.2f", fps) << " FPS";
    putText(color_image, convert.str(), Point(25, 25), FONT_HERSHEY_DUPLEX, 1, Scalar(0, 255, 0), 2);
}

void PiVision::drawQRCodeResults(Mat& frame, const vector<Point>& corners, double fps)
{
    if (!corners.empty() && corners.size() % 4 == 0)
    {
        //vector<Point> qrcode_contour(corners.begin(), corners.begin()+4);

        drawQRCodeContour(frame, corners);

        //cout << "QR @ " << Mat(qrcode_contour).reshape(2, 1);

        Moments mu = cv::moments(corners);

        Point2f mc = Point2f(static_cast<float>(mu.m10 / (mu.m00 + 1e-5)),
                             static_cast<float>(mu.m01 / (mu.m00 + 1e-5)));

        qInfo() << "mc = [" << mc.x << "," << mc.y << "]";

        double half_width = frame.cols / 2.0;
        double half_height= frame.rows / 2.0;

        double x_offset = half_width-mc.x;
        double y_offset = half_height-mc.y;

        qInfo() << "offsets = [" << x_offset << "," << y_offset << "]";

        if(x_offset>20)
            qInfo()<<"MOVE LEFT!";
        else if(x_offset<-20)
            qInfo()<<"MOVE RIGHT!";
        else
            qInfo()<<"ALIGNED";
    }

    drawFPS(frame, fps);
}

double PiVision::processQRCodeDetection(QRCodeDetector& qrcode, const Mat& input, Mat& result, vector<Point>& corners)
{
    if (input.channels() == 1)
        cvtColor(input, result, COLOR_GRAY2BGR);
    else
        input.copyTo(result);

    TickMeter timer;

    timer.start();
    std::string decode_info = qrcode.detectAndDecode(input, corners);

//    if (!decode_info.empty())
//        qInfo() << QString::fromStdString(decode_info);
//    else
//        qInfo() << "Can't decode QR code";

    timer.stop();

    double fps = 1 / timer.getTimeSec();
    drawQRCodeResults(result, corners, fps);

    return fps;
}

void PiVision::loop(){
    QRCodeDetector qrcode;

    forever{
        //qInfo()<<"Running piVision thread: "<<QThread::currentThread();

        if(QThread::currentThread()->isInterruptionRequested())
            return;

        Mat capture = piCamera->getVideoCaptureCV();

        vector<Point> corners;
        if(piCameraController->isQRDetectionEnabled()) //must add check on corners, contourArea(src_points)' is 0 must be greater than '0.0' is 0
            processQRCodeDetection(qrcode, capture, capture, corners);

        piCameraController->refreshFrame(capture);

        QThread::msleep(delay);
    }
}

/*
Mat processed_capture;
cvtColor(capture,processed_capture,COLOR_BGR2GRAY);
threshold(processed_capture, processed_capture, 120, 255, THRESH_BINARY_INV + THRESH_OTSU);
dilate(processed_capture, processed_capture, getStructuringElement(MORPH_RECT, Size(13, 13)));

vector<vector<Point> > contours;
vector<Vec4i> hierarchy;
findContours(processed_capture, contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE );

Mat drawing = Mat::zeros( processed_capture.size(), CV_8UC3 );
RNG rng(1000);
for( size_t i = 0; i< contours.size(); i++ )
{
    Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
    drawContours( drawing, contours, (int)i, color, 2, LINE_8, hierarchy, 0 );
}
*/
