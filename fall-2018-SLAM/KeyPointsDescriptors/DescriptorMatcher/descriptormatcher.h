#ifndef DESCRIPTORMATCHER_H
#define DESCRIPTORMATCHER_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>


class DescHandler
{
public:
    static DescHandler* factory(std::string descType, std::string matcherType)
    {
        cv::Ptr<cv::Feature2D> desc;
        cv::Ptr<cv::DescriptorMatcher> matcher;
        if(descType=="sift")
            desc = cv::xfeatures2d::SIFT::create();
        else if(descType=="surf")
            desc = cv::xfeatures2d::SURF::create();
        else if(descType=="orb")
            desc = cv::ORB::create();
        else
            throw std::string("error");

        if(matcherType=="flann")
        {
            // ORB outputs integer descriptor
            if(descType=="orb")
                matcher = new cv::FlannBasedMatcher(new cv::flann::LshIndexParams(12, 20, 2));
            else
                matcher = cv::FlannBasedMatcher::create();
        }
        else if(matcherType=="bf")
        {
            if(descType=="orb")
                matcher = cv::BFMatcher::create(cv::NORM_HAMMING);
            else
                matcher = cv::BFMatcher::create(cv::NORM_L1);
        }
        else
            throw std::string("error");

        return new DescHandler(descType, desc, matcher);
    }

    void detectAndCompute(cv::Mat _image)
    {
        image = _image;
        descMaker->detectAndCompute(image, cv::Mat(), keypoints, descriptors);
    }

    void matchAndDraw(const DescHandler* other, float acceptRatio = 0.5f)
    {
        matcher->match(this->getDescriptors(), other->getDescriptors(), matches);

        // sort matches by score and remove not so good matches
        std::sort(matches.begin(), matches.end());
        const int numGoodMatches = matches.size() * acceptRatio;
        matches.erase(matches.begin()+numGoodMatches, matches.end());

        // draw matches
        cv::Mat matchimg;
        cv::drawMatches(this->getImage(), this->getKeypoints(),
                        other->getImage(), other->getKeypoints(),
                        matches, matchimg);
        cv::putText(matchimg, name, cv::Point(10,30),
                    cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar::all(0), 2);
        appendResult(matchimg);
    }

    const cv::Mat getDescriptors() const { return descriptors; }
    const cv::Mat getImage() const { return image; }
    const std::vector<cv::KeyPoint>& getKeypoints() const { return keypoints; }

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
    DescHandler(cv::String _name, cv::Ptr<cv::Feature2D> _descMaker,
                      cv::Ptr<cv::DescriptorMatcher> _matcher)
    {
        name = _name;
        descMaker = _descMaker;
        matcher = _matcher;
    }

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

    cv::Ptr<cv::Feature2D> descMaker;
    cv::Ptr<cv::DescriptorMatcher> matcher;
    cv::String name;
    cv::Mat image;
    std::vector<cv::KeyPoint> keypoints;
    cv::Mat descriptors;
    std::vector<cv::DMatch> matches;
    static cv::Mat result;
};

#endif // DESCRIPTORMATCHER_H
