#ifndef SHAPEDRAWER_H
#define SHAPEDRAWER_H

#include <QString>
#include <QDebug>
#include <opencv2/opencv.hpp>


struct ShapeException
{
    ShapeException(QString msg_) : msg(msg_) {}
    QString msg;
};

class ShapeDrawer
{
public:
    ShapeDrawer(const std::vector<int>& _params) : params(_params) {}
    virtual void draw(cv::Mat& image) = 0;

protected:
    std::vector<int> params;
};


class CircleDrawer : public ShapeDrawer
{
public:
    CircleDrawer(const std::vector<int>& _params)
        : ShapeDrawer(_params)
    {
        if(params.size() != 3)
            throw ShapeException("insuffient params for circle");
        qDebug() << "circle added";
    }

    virtual void draw(cv::Mat& image)
    {
        if(image.empty())
            throw ShapeException("input image is emtpy");
        cv::circle(image, cv::Point(params[0], params[1]), params[2], CV_RGB(0,0,0));
        image.at<cv::Vec3b>(params[0], params[1]) = cv::Vec3b(0,0,0);
    }
};


class RectDrawer : public ShapeDrawer
{
public:
    RectDrawer(const std::vector<int>& _params)
        : ShapeDrawer(_params)
    {
        if(params.size() != 4)
            throw ShapeException("insuffient params for rectangle");
    }

    virtual void draw(cv::Mat& image)
    {
        if(image.empty())
            throw ShapeException("input image is emtpy");
        cv::rectangle(image, cv::Point(params[0], params[1]),
                cv::Point(params[0]+params[2], params[1]+params[3]), CV_RGB(0,0,0));
    }
};

#endif // SHAPEDRAWER_H
