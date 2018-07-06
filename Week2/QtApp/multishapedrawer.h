#ifndef MULTISHAPEDRAWER_H
#define MULTISHAPEDRAWER_H

// study point: c++ functional, please read
// https://en.cppreference.com/w/cpp/utility/functional/function
#include <functional>
#include "paramparser.h"
#include "shapedrawer.h"

class MultiShapeDrawer
{
public:
    MultiShapeDrawer() {}

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
        auto func = std::bind(&MultiShapeDrawer::addCircle, this, params);
        drawShapes(func);
    }

    void pushRect(QString params)
    {
        auto func = std::bind(&MultiShapeDrawer::addRect, this, params);
        drawShapes(func);
    }

    void popShape()
    {
        auto func = [&](){ if(shapes.size()==0) throw ShapeException("no circle to draw");
                          shapes.pop_back(); };
        drawShapes(func);
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

    void drawShapes(auto& func)
    {
        try {
            func();
            qDebug() << "now there are" << shapes.size() << "shapes";

            image.setTo(cv::Scalar(255,255,255));
            // study point: c++11 new for loop style
            for(ShapeDrawer* shape: shapes)
                shape->draw(image);
            cv::imshow("image", image);
        }
        catch (ParsingException& exception) {
            qDebug() << "parsing exception" << exception.msg;
        }
        catch (ShapeException& exception) {
            qDebug() << "circle exception" << exception.msg;
        }
    }

    void addCircle(QString params)
    {
        ParamParser parser(params);
        ShapeDrawer* circle = new CircleDrawer(parser.getParams());
        shapes.push_back(circle);
    }

    void addRect(QString params)
    {
        ParamParser parser(params);
        ShapeDrawer* rect = new RectDrawer(parser.getParams());
        shapes.push_back(rect);
    }


    cv::Mat image;
    std::list<ShapeDrawer*> shapes;
};



#endif // MULTISHAPEDRAWER_H
