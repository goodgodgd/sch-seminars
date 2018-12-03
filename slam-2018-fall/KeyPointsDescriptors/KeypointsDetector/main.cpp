#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

#include "keypointdetector.h"
cv::Mat KeypointDetector::result;


int main()
{
    std::cout << "Press 'f' to show fixed snapshot, and 'q' to quit." << std::endl;
    cv::VideoCapture cap(0);
    if(!cap.isOpened())
        return -1;

    KeypointDetector sift(cv::xfeatures2d::SIFT::create(), "sift");
    KeypointDetector surf(cv::xfeatures2d::SURF::create(), "surf");
    KeypointDetector fast(cv::FastFeatureDetector::create(), "fast");
    std::vector<KeypointDetector> detectors = {sift, surf, fast};

    cv::Mat frame;
    while(1)
    {
        cap >> frame;
        for(auto& det: detectors)
        {
            det.detect(frame);
            det.drawAndAppend();
        }
        cv::Mat result = KeypointDetector::getResultingImg(1500);

        int key = cv::waitKey(10);
        if(key==int('f') || key==int('F'))
        {
            std::cout << "set fixed reference result" << std::endl;
            cv::imshow("snapshot", result);
        }
        else if(key==int('q') || key==int('Q'))
            break;

        cv::imshow("keypoints", result);
    }

    return 0;
}
