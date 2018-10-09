#ifndef DESCRIPTORMATCHER_H
#define DESCRIPTORMATCHER_H

#include <iostream>
#include <opencv2/opencv.hpp>


class DescriptorMatcher
{
public:
    DescriptorMatcher() {}
    DescriptorMatcher(cv::Ptr<cv::Feature2D> _featMngr, cv::String _name)
    {
        featMngr = _featMngr;
        name = _name;
    }

    DescriptorMatcher(const DescriptorMatcher& other)
    {
        featMngr = other.getFeatMngr();
        name = other.getName();
    }


    void detectAndCompute(cv::Mat _image)
    {
        image = _image;
        featMngr->detectAndCompute(image, cv::Mat(), keypoints, descriptors);
        // ORB outputs 8bit descriptor but FLANN matcher accept only 32f
        if(descriptors.type() == CV_8U)
            descriptors.convertTo(descriptors, CV_32F);
    }

    void matchAndDraw(const DescriptorMatcher& other, float acceptRatio = 0.5f)
    {
        // match descriptors
        cv::FlannBasedMatcher matcher;
        matcher.match(this->getDescriptors(), other.getDescriptors(), matches);

        // sort matches by score and remove not so good matches
        std::sort(matches.begin(), matches.end());
        const int numGoodMatches = matches.size() * acceptRatio;
        matches.erase(matches.begin()+numGoodMatches, matches.end());

        // draw matches
        cv::Mat matchimg;
        cv::drawMatches(this->getImage(), this->getKeypoints(),
                        other.getImage(), other.getKeypoints(),
                        matches, matchimg);
        cv::putText(matchimg, name, cv::Point(10,30),
                    cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar::all(0), 2);
        appendResult(matchimg);
    }

    cv::Mat getDescriptors() const { return descriptors; }
    cv::Mat getImage() const { return image; }
    const std::vector<cv::KeyPoint>& getKeypoints() const { return keypoints; }
    const cv::Ptr<cv::Feature2D>& getFeatMngr() const { return featMngr; }
    const cv::String& getName() const { return name; }

    static cv::Mat getResultingImg(int maxheight = 0)
    {
        cv::Mat resimg = result.clone();
        result.release();
        if(maxheight > 100 && maxheight < resimg.rows)
        {
            float scale = float(maxheight) / float(resimg.rows);
            cv::Size neosize(int(resimg.cols * scale), int(resimg.rows * scale));
            cv::resize(resimg, resimg, neosize);
        }
        return resimg;
    }

private:
    static void appendResult(cv::Mat matchimg)
    {
        if(result.empty())
            result = matchimg;
        else
        {
            std::vector<cv::Mat> himgs = {result, matchimg};
            cv::vconcat(himgs, result);
        }
    }

    cv::Ptr<cv::Feature2D> featMngr;
    cv::String name;
    cv::Mat image;
    std::vector<cv::KeyPoint> keypoints;
    cv::Mat descriptors;
    std::vector<cv::DMatch> matches;
    static cv::Mat result;
};

#endif // DESCRIPTORMATCHER_H
