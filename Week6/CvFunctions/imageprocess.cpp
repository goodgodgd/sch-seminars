#include "imageprocess.h"

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
    cv::Rect rect(pt.x-boxSize/2, pt.y-boxSize/2, boxSize, boxSize);
    cv::rectangle(dstimg, rect, CV_RGB(255,0,0), 1);
    return dstimg;
}

void ImageProcess::printValues(cv::Mat image, const cv::Point pt, COLOR color)
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

std::vector<cv::Mat> ImageProcess::drawHist(cv::Mat image, const cv::Point pt,
                                            COLOR color, int boxSize)
{
    std::vector<cv::Mat> cvtimgs = convertAndSplit(image, color);

    std::vector<cv::Mat> histograms(cvtimgs.size());
    for(size_t i=0; i<cvtimgs.size(); i++)
        histograms[i] = calcHist(cvtimgs[i]);

    std::vector<cv::Mat> histImgs(cvtimgs.size());
    for(size_t i=0; i<histograms.size(); i++)
        histImgs[i] = drawHist(histograms[i]);

    return histImgs;



//    std::vector<cv::Mat> images = {cvtimg};

//    std::vector<int> numChnns = {0, 1, 2};
//    cv::Mat mask;
//    cv::Mat histogram;
//    std::vector<int> histSizes = {100, 100, 100};
//    std::vector<float> histRanges = {0.f, 255.f, 0.f, 255.f, 0.f, 255.f};


//    //TODO: 여기서 다시시작
//    cv::calcHist(images, numChnns, cv::Mat(), histogram, histSizes, histRanges);
//    qDebug() << "calchist" << histogram.channels() << histogram.rows << histogram.cols;
}

cv::Mat ImageProcess::calcHist(cv::Mat image)
{
    int histBins[] = {100};
    float range[] = {0, 256};
    const float* ranges[] = {range};
    const int channels[] = {0};

    cv::Mat histogram;
    cv::calcHist(&image, 1, channels, cv::Mat(),
                 histogram, 1, histBins, ranges, true, false);

    qDebug() << "calchist" << histogram.channels()
             << histogram.rows << histogram.cols
             << histogram.type() << CV_32FC1;

    return histogram;
}

cv::Mat ImageProcess::drawHist(cv::Mat histValue)
{
    cv::Mat histImg(200, histValue.rows*2, CV_8UC1, cv::Scalar(255));
    cv::Scalar sumval = cv::sum(histValue);
    const float maxval = sumval(0)/4.f;

    for(int i=0; i<histValue.rows; i++)
    {
        int top = histImg.rows - histValue.at<float>(i)/maxval*200;
        top = std::max(std::min(top, histImg.rows-1), 0);
        cv::line(histImg, cv::Point(i*2, histImg.rows-1),
                 cv::Point(i*2, top), cv::Scalar(0));
    }
    return histImg;
}

std::vector<cv::Mat> ImageProcess::floodFill(cv::Mat image, const cv::Point pt,
                                             COLOR color, int floodThresh)
{
    std::vector<cv::Mat> channels = convertAndSplit(image, color);
    for(auto& chnn: channels)
        chnn = floodFillImpl(chnn, pt, floodThresh);
    return channels;
}

cv::Mat ImageProcess::floodFillImpl(cv::Mat image, const cv::Point pt, int floodThresh)
{

}
