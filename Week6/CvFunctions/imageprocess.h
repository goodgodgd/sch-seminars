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
    void printValues(cv::Mat image, const cv::Point pt, COLOR color);
    std::vector<cv::Mat> drawHist(cv::Mat image, const cv::Point pt,
                                  COLOR color, int boxSize);
    cv::Mat calcHist(cv::Mat image);
    cv::Mat drawHist(cv::Mat histValue);

    std::vector<cv::Mat> floodFill(cv::Mat image, const cv::Point pt,
                                   COLOR color, int floodThresh);
    cv::Mat floodFillImpl(cv::Mat image, const cv::Point pt, int floodThresh);

};

#endif // COLORCONVERTER_H
