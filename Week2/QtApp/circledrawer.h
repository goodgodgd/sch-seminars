#ifndef CIRCLEDRAWER_H
#define CIRCLEDRAWER_H

#include <QString>
#include <QDebug>
#include <opencv2/opencv.hpp>


struct CircleException
{
    CircleException(QString msg_) : msg(msg_) {}
    QString msg;
};


class CircleDrawer
{
public:
    CircleDrawer(const std::vector<int>& _params)
        : params(_params)
    {
        if(params.size() != 3)
            throw CircleException("insuffient params");
    }

    void draw(cv::Mat& image)
    {
        if(image.empty())
            throw CircleException("input image is emtpy");
        cv::circle(image, cv::Point(params[0], params[1]), params[2], CV_RGB(0,0,0));
        image.at<cv::Vec3b>(params[0], params[1]) = cv::Vec3b(0,0,0);
        cv::imshow("image", image);
    }

private:
    std::vector<int> params;
};

#endif // CIRCLEDRAWER_H
