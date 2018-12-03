#include "imageprocess.h"

#define smax(a,b)       (((a)>(b))?(a):(b))
#define smin(a,b)       (((a)<(b))?(a):(b))
#define sclamp(v,n,x)   smin(smax(v,n),x)

ImageProcess::ImageProcess()
{

}

std::vector<cv::Mat> ImageProcess::convertAndSplit(const cv::Mat rgbimg, COLOR color)
{
    cv::Mat cvtimg = convert(rgbimg, color);
    std::vector<cv::Mat> chnnimgs;
    cv::split(cvtimg, chnnimgs);
    return chnnimgs;
}

cv::Mat ImageProcess::convert(const cv::Mat rgbimg, COLOR color)
{
    cv::Mat cvtimg;
    if(color == COLOR::RGB)
        cvtimg = rgbimg;
    else if(color == COLOR::LAB)
        cv::cvtColor(rgbimg, cvtimg, CV_BGR2Lab);
    else if(color == COLOR::YCrCb)
        cv::cvtColor(rgbimg, cvtimg, CV_BGR2YCrCb);
    else if(color == COLOR::HSV)
        cv::cvtColor(rgbimg, cvtimg, CV_BGR2HSV);

    return cvtimg;
}

cv::Mat ImageProcess::markTarget(cv::Mat srcimg, cv::Point pt, int boxSize)
{
    cv::Mat dstimg = srcimg.clone();
    cv::circle(dstimg, pt, 2, CV_RGB(255,0,0), 2);
    cv::Rect rect = ptnSizeToRect(pt, boxSize, srcimg.size());
    cv::rectangle(dstimg, rect, CV_RGB(255,0,0), 1);
    return dstimg;
}

cv::Rect ImageProcess::ptnSizeToRect(cv::Point center, int rectSize, cv::Size bound)
{
    cv::Rect rect(center.x-rectSize/2, center.y-rectSize/2, rectSize, rectSize);
    rect.x = sclamp(rect.x, 0, bound.width);
    rect.y = sclamp(rect.y, 0, bound.height);
    rect.width = sclamp(rect.width, 0, bound.width - rect.x - 1);
    rect.height = sclamp(rect.height, 0, bound.height - rect.y - 1);
    return rect;
}

void ImageProcess::printValues(cv::Mat image, const cv::Point pt, const COLOR color)
{
    QString colname;
    if(color == COLOR::RGB)
        colname = "r,g,b:";
    else if(color == COLOR::LAB)
        colname = "l,a,b:";
    else if(color == COLOR::YCrCb)
        colname = "Y,Cr,Cb:";
    else if(color == COLOR::HSV)
        colname = "h,s,v:";

    cv::Vec3b pixel = image.at<cv::Vec3b>(pt);
    qDebug() << colname << pixel(0) << pixel(1) << pixel(2);
}

std::vector<cv::Mat> ImageProcess::drawHists(cv::Mat image, COLOR color,
                                             const cv::Point pt, const int boxSize,
                                             const cv::Size histImgSize)
{
    std::vector<cv::Mat> cvtimgs = convertAndSplit(image, color);

    std::vector<cv::Mat> histograms(cvtimgs.size());
    for(size_t i=0; i<cvtimgs.size(); i++)
        histograms[i] = calcHist(cvtimgs[i], pt, boxSize);

    std::vector<cv::Mat> histImgs(cvtimgs.size());
    for(size_t i=0; i<histograms.size(); i++)
        histImgs[i] = drawHist(histograms[i], histImgSize);

    return histImgs;
}

cv::Mat ImageProcess::calcHist(cv::Mat image, const cv::Point pt, const int boxSize)
{
    int histBins[] = {100};
    float range[] = {0, 256};
    const float* ranges[] = {range};
    const int channels[] = {0};

    // prepare mask with the same size with image,
    // and with roi centered at pt and sized by boxSize within image size
    cv::Mat mask = image.clone();
    mask.setTo(0);
    cv::Rect roi = ptnSizeToRect(pt, boxSize, image.size());
    mask(roi).setTo(255);

    cv::Mat histogram;
    cv::calcHist(&image, 1, channels, mask,
                 histogram, 1, histBins, ranges, true, false);

    return histogram;
}

cv::Mat ImageProcess::drawHist(cv::Mat histValue, const cv::Size histImgSize)
{
    const int blank = 1;
    cv::Mat histImg(200, histValue.rows*2 + blank*2, CV_8UC1, cv::Scalar(255));
    histImg.col(0).setTo(0);
    histImg.col(histImg.cols-1).setTo(0);

    cv::Scalar sumval = cv::sum(histValue);
    const float maxval = sumval(0)/4.f;

    for(int i=0; i<histValue.rows; i++)
    {
        int top = histImg.rows - histValue.at<float>(i)/maxval*200;
        top = sclamp(top, 0, histImg.rows-1);
        cv::line(histImg, cv::Point(blank + i*2, histImg.rows-1),
                 cv::Point(blank + i*2, top), cv::Scalar(0));
    }

    if(histImg.size() == histImgSize ||
            histImgSize.width==0 || histImgSize.height==0)
        return histImg;

    cv::Mat histImgRsz;
    cv::resize(histImg, histImgRsz, histImgSize, 0, 0, cv::INTER_NEAREST);
    return histImgRsz;
}

std::vector<cv::Mat> ImageProcess::floodFill(cv::Mat image, const cv::Point pt,
                                             const COLOR color, const int floodThresh)
{
    std::vector<cv::Mat> cvtImgs = convertAndSplit(image, color);
    for(auto& image: cvtImgs)
        image = floodFillImpl(image, pt, floodThresh);
    return cvtImgs;
}

cv::Mat ImageProcess::floodFillImpl(cv::Mat image, const cv::Point pt, const int diff)
{
    cv::Mat floodImg;
    cv::cvtColor(image, floodImg, CV_GRAY2BGR);
    cv::Scalar fldiff(diff, diff, diff);
    cv::floodFill(floodImg, cv::Mat(), pt, CV_RGB(255,255,0), 0, fldiff, fldiff);
    return floodImg;
}
