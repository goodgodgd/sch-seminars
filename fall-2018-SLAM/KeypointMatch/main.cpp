#include <iostream>
#include <opencv2/opencv.hpp>

int main()
{
    cv::VideoCapture cap(0);
    if(!cap.isOpened())
        return -1;

    cv::Mat fixed;
    cv::Mat frame;
    int i=0;
    while(++i < 500)
    {
        cap >> frame;
        int key = cv::waitKey(10);
        if(key==int('f') || key==int('F'))
        {
            std::cout << "set fixed reference frame" << std::endl;
            fixed = frame.clone();
        }
        else if(key==int('q') || key==int('Q'))
            break;

        if(fixed.empty())
            cv::imshow("frame", frame);
        else
        {
            cv::Mat concImg;
            std::vector<cv::Mat> imgs = {fixed, frame};
            cv::hconcat(imgs, concImg);
            cv::imshow("frame", concImg);
        }
    }

    return 0;
}
