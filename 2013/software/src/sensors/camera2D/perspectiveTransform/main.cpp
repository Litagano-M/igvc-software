#include <opencv2/opencv.hpp>
#include <iostream>
#include <sstream>
#include <sensors/camera3D/Bumblebee2.h>
#include <sensors/lidar/NAV200.h>
#include <boost/thread.hpp>
#include <sensors/camera2D/laneDetection/LidarBasedObstacleHider.hpp>
#include <sensors/camera2D/perspectiveTransform/PerspectiveTransformer.hpp>
#include <sensors/camera3D/StereoImageRepeater.h>

#include "sensors/camera2D/laneDetection/CameraListener.hpp"

using namespace cv;
using namespace std;
using namespace IGVC::Sensors;


Mat gFrame;

int thresh;

void OnMouse(int evtType, int x, int y, int, void*)
{
    if(evtType == EVENT_LBUTTONUP)
    {
        uchar* p = gFrame.ptr<uchar>(y);
        cout << (int)p[x*3] << " " << (int)p[x*3+1] << " " << (int)p[x*3+2] << endl;
    }
}

/*
class CameraListener
{
public:
    CameraListener(StereoSource *cam, Lidar *lidar)
        : _transformer(cam),
          _hider(lidar, &_transformer.OnNewTransformedImage),
          LOnNewFrame(this)
    {
        _cam = cam;
        _lidar = lidar;
        _transformer.OnNewTransformedImage += &LOnNewFrame;
        _hider.OnNewProcessedFrame += &LOnNewFrame;
    }

private:
    Lidar* _lidar;
    StereoSource* _cam;
    PerspectiveTransformer _transformer;
    LidarBasedObstacleHider _hider;
    LISTENER(CameraListener, OnNewFrame, Mat);

    void OnNewFrame(Mat frame)
    {
        if(RUNNING)
        {
            imshow("raw", frame);

            blur(frame, frame, Size(9,9), Point(-1,-1));

            Mat HSV;
            cvtColor(frame, HSV, CV_BGR2HSV);
            vector<Mat> channels;
            split(HSV, channels);
            threshold(channels[0], channels[0], 90, 255, CV_THRESH_BINARY);
            threshold(channels[1], channels[1], 25, 255, CV_THRESH_BINARY);

            Mat output(frame.rows, frame.cols, CV_8UC1);
            bitwise_and(channels[0], channels[1], output);

            Mat intensity;
            {
                vector<Mat> rgbChs;
                split(frame, rgbChs);
                intensity = ((rgbChs[0] + rgbChs[1]) + rgbChs[2])  / 3;
            }
            equalizeHist(intensity, intensity);
            threshold(intensity, intensity, thresh, 255, CV_THRESH_BINARY);
            imshow("Intense", intensity);

            bitwise_and(output, intensity, output);

            /*
            int erosion_size = 6;
            cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS,
                                                        cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
                                                        cv::Point(erosion_size, erosion_size) );

            erode(output, output, element);
            dilate(output, output, element);*/
/*

            imshow("output", output);
            gFrame = frame;
            imshow("Image", frame);
        }
    }
};
*/

int main()
{
    //NAV200 lidar;
    NAV200* lidarPointer = 0;

    //Bumblebee2 camera("/home/robojackets/igvc/2013/software/src/sensors/camera3D/calib/out_camera_data.xml");
    //Bumblebee2 camera;

    //StereoImageRepeater source("/home/robojackets/Desktop/img_right2.jpg", "/home/robojackets/Desktop/img_right2.jpg");
    StereoImageRepeater camera("/home/alex/Desktop/img_left2.jpg","/home/alex/Desktop/img_left2.jpg");
    //while(!camera.frameCount);

    namedWindow("Image");
    setMouseCallback("Image", OnMouse, 0);

    createTrackbar("t1", "Image", &thresh, 255, 0, 0);

    //CameraListener listener(&camera, &lidar);
    CameraListener listener(&camera, lidarPointer);

    waitKey(0);

    return 0;
}
