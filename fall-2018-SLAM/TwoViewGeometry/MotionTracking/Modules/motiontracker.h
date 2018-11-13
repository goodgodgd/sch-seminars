#ifndef MOTIONTRACKER_H
#define MOTIONTRACKER_H

#include <opencv2/opencv.hpp>
#include <vector>
#include "deschandler.h"

class MotionTracker
{
public:
    MotionTracker();
    void setup();
    void run();

private:
    std::vector<cv::Point3f> computePoseAndPoints(cv::Mat curframe, cv::Mat& outTransform);
    void isSetupFinished();

    cv::VideoCapture cap;
    cv::Mat keyframe;
    cv::Mat curframe;
    DescHandler* keyfDesc;
    DescHandler* curfDesc;
    std::vector<cv::Point3f> points3D;
    cv::Mat camK;
    cv::Mat curTfm;
};

#endif // MOTIONTRACKER_H
