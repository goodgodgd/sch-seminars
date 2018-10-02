#ifndef COLORCONVERTER_H
#define COLORCONVERTER_H

#include <QString>
#include <QDebug>
#include <opencv2/opencv.hpp>
#include <vector>

enum class COLOR {
    RGB,
    YCrCb,
    LAB,
    HSV,
};

enum class EFFECT {
    VALUE,
    HISTOGRAM,
    FLOODFILL,
};

struct IPParams
{
    COLOR color;
    EFFECT effect;
    int boxSize;
    int floodThresh;
};


class ImageProcess
{
public:
    ImageProcess();

    std::vector<cv::Mat> convertAndSplit(const cv::Mat rgbimg, COLOR color);
    cv::Mat convert(const cv::Mat rgbimg, COLOR color);
    cv::Mat markTarget(cv::Mat srcimg, cv::Point pt, int boxSize);
    cv::Rect ptnSizeToRect(cv::Point center, int rectSize, cv::Size bound);
    void printValues(cv::Mat image, const cv::Point pt, const COLOR color);
    std::vector<cv::Mat> drawHists(cv::Mat image, COLOR color,
                                   const cv::Point pt, const int boxSize,
                                   const cv::Size histImgSize={0,0});
    cv::Mat calcHist(cv::Mat image, const cv::Point pt, const int boxSize);
    cv::Mat drawHist(cv::Mat histValue, const cv::Size histImgSize);

    std::vector<cv::Mat> floodFill(cv::Mat image, const cv::Point pt,
                                   const COLOR color, const int floodThresh);
    cv::Mat floodFillImpl(cv::Mat image, const cv::Point pt, const int diff);

};

#endif // COLORCONVERTER_H
