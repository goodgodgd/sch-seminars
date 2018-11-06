#include "imagemanipulator.h"
#include <map>


void onMouse( int evt, int x, int y, int flags, void* param)
{
    ImageManipulator* imgman = (ImageManipulator *)param;
    imgman->handleEvent(evt, x, y, flags);
}

int ImageManipulator::imID_count = 0;


ImageManipulator::ImageManipulator()
{
    params.color = COLOR::RGB;
    params.effect = EFFECT::VALUE;
    params.boxSize = 10;
    params.floodThresh = 5;
    imID = imID_count++;
}

ImageManipulator::ImageManipulator(const QString& fileName)
    : ImageManipulator()
{
    image = cv::imread(fileName.toStdString());
    imName.sprintf("im%02d", imID);
    cv::imshow(imName.toStdString(), image);
    cv::setMouseCallback(imName.toStdString(), onMouse, this);
}

void ImageManipulator::handleEvent(int evt, int x, int y, int flags)
{
    if(evt == CV_EVENT_LBUTTONDOWN)
    {
        qDebug() << "Mouse event: " << "x=" << x << ", y=" << y;
        qDebug() << "ID" << imID;
        putEffectAndShow(cv::Point(x,y));
    }
}

void ImageManipulator::convertColor(COLOR color)
{
    params.color = color;
    std::vector<cv::Mat> channels = improc.convertAndSplit(image, color);
    concatAndShow(channels);
}

void ImageManipulator::concatAndShow(std::vector<cv::Mat>& images, QString suffix)
{
    cv::Mat outimg;
    cv::hconcat(images, outimg);
    QString cname = imName + suffix;
    cv::imshow(cname.toStdString(), outimg);
}

void ImageManipulator::setEffect(EFFECT effect, int param)
{
    params.effect = effect;
    if(effect == EFFECT::HISTOGRAM)
        params.boxSize = param;
    if(effect == EFFECT::FLOODFILL)
        params.floodThresh = param;
}

void ImageManipulator::putEffectAndShow(cv::Point pt)
{
    cv::Mat timg = improc.markTarget(image, pt, params.boxSize);
    cv::imshow(imName.toStdString(), timg);

    improc.printValues(image, pt, params.color);
    if(params.effect == EFFECT::HISTOGRAM)
    {
        std::vector<cv::Mat> hists =
                improc.drawHists(image, params.color, pt, params.boxSize);
        concatAndShow(hists, "hist");
    }
    else if(params.effect == EFFECT::FLOODFILL)
    {
        std::vector<cv::Mat> chnns =
                improc.floodFill(image, pt, params.color, params.floodThresh);
        concatAndShow(chnns);
    }
}

