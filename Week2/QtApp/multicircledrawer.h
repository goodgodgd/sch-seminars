#ifndef MULTICIRCLEDRAWER_H
#define MULTICIRCLEDRAWER_H

// study point: c++ functional, please read
// https://en.cppreference.com/w/cpp/utility/functional/function
#include <functional>
#include "paramparser.h"
#include "circledrawer.h"

class MultiCircleDrawer
{
public:
    MultiCircleDrawer() {}

    void initImage(QString params)
    {
        try {
            image = createImage(params);
            cv::imshow("image", image);
        }
        catch (ParsingException& exception) {
            qDebug() << "parsing exception" << exception.msg;
        }
    }

    void pushCircle(QString params)
    {
        auto func = std::bind(&MultiCircleDrawer::addCircle, this, params);
        drawCircle(func);
    }

    void popCircle()
    {
        auto func = [&](){ if(circles.size()==0) throw CircleException("no circle to draw");
                          circles.pop_back(); };
        drawCircle(func);
    }

private:
    cv::Mat createImage(const QString& paramString)
    {
        ParamParser parser(paramString);
        // study point: auto??
        auto& params = parser.getParams();
        if(params.size() != 2)
            throw ParsingException("reset image needs two parameters");

        cv::Mat image = cv::Mat(params[1], params[0], CV_8UC3);
        image.setTo(cv::Scalar(255,255,255));
        return image;
    }

    void drawCircle(auto& func)
    {
        try {
            func();
            qDebug() << "now there are" << circles.size() << "circles";

            image.setTo(cv::Scalar(255,255,255));
            // study point: c++11 new for loop style
            for(auto& circle: circles)
                circle.draw(image);
            cv::imshow("image", image);
        }
        catch (ParsingException& exception) {
            qDebug() << "parsing exception" << exception.msg;
        }
        catch (CircleException& exception) {
            qDebug() << "circle exception" << exception.msg;
        }
    }

    void addCircle(QString params)
    {
        ParamParser parser(params);
        CircleDrawer circle(parser.getParams());
        circles.push_back(circle);
    }

    cv::Mat image;
    std::list<CircleDrawer> circles;

};



#endif // MULTICIRCLEDRAWER_H
