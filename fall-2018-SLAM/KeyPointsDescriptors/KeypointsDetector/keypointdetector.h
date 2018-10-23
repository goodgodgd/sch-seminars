#ifndef KEYPOINTDETECTOR_H
#define KEYPOINTDETECTOR_H

#include <opencv2/opencv.hpp>

class KeypointDetector
{
public:
    KeypointDetector() {}
    KeypointDetector(cv::Ptr<cv::Feature2D> _detector, cv::String _name)
    {
        detector = _detector;
        name = _name;
    }

    void detect(cv::Mat _image)
    {
        image = _image;
        detector->detect(image, keypoints);
    }

    void drawAndAppend()
    {
        cv::Mat keyptsimg;
        cv::drawKeypoints(image, keypoints, keyptsimg, cv::Scalar::all(-1),
                          cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
        cv::putText(keyptsimg, name, cv::Point(10,30),
                    cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar::all(0), 2);
        appendResult(keyptsimg);
    }

    static cv::Mat getResultingImg(int maxwidth = 0)
    {
        cv::Mat resimg = result.clone();
        result.release();
        if(maxwidth > 100 && maxwidth < resimg.cols)
        {
            float scale = float(maxwidth) / float(resimg.cols);
            cv::Size neosize(int(resimg.cols * scale), int(resimg.rows * scale));
            cv::resize(resimg, resimg, neosize);
        }
        return resimg;
    }

private:
    void appendResult(cv::Mat keyptsimg)
    {
        if(result.empty())
            result = keyptsimg;
        else
        {
            std::vector<cv::Mat> himgs = {result, keyptsimg};
            cv::hconcat(himgs, result);
        }
    }

    cv::Ptr<cv::Feature2D> detector;
    cv::String name;
    cv::Mat image;
    std::vector<cv::KeyPoint> keypoints;
    static cv::Mat result;
};

#endif // KEYPOINTDETECTOR_H
