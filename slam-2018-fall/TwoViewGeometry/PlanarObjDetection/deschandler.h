#ifndef DESCRIPTORMATCHER_H
#define DESCHANDLER_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>


class DescHandler
{
public:
    // DescHandler factory function
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

    //---------- 일반적인 멤버 함수 ----------//
    void detectAndCompute(cv::Mat _image)
    {
        image = _image;
        descMaker->detectAndCompute(image, cv::Mat(), keypoints, descriptors);
    }

    std::vector<cv::DMatch> match(const cv::Mat trainDescriptors, float acceptRatio = 0.5f)
    {
        std::vector<cv::DMatch> matches;
        matcher->match(descriptors, trainDescriptors, matches);

        // sort matches by score and remove not so good matches
        std::sort(matches.begin(), matches.end());
        const int numGoodMatches = matches.size() * acceptRatio;
        matches.erase(matches.begin()+numGoodMatches, matches.end());
        return matches;
    }

    static void drawAndAppendResult(const DescHandler* query, const DescHandler* train,
                                    const std::vector<cv::DMatch>& matches)
    {
        // draw matches
        cv::Mat matchimg;
        cv::drawMatches(query->getImage(), query->getKeypoints(),
                        train->getImage(), train->getKeypoints(),
                        matches, matchimg);
        std::cout << "drawmatches query: "
                  << query->getImage().rows << " " << query->getImage().cols << ", train: "
                  << train->getImage().rows << " " << train->getImage().cols << ", result: "
                  << matchimg.rows << " " << matchimg.cols << std::endl;
        cv::putText(matchimg, query->getName(), cv::Point(10,30),
                    cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar::all(0), 2);

        // stack resulting images
        if(result.empty())
            result = matchimg;
        else
        {
            std::vector<cv::Mat> himgs = {result, matchimg};
            cv::vconcat(himgs, result);
        }
    }

    const cv::Mat getDescriptors() const { return descriptors; }
    const cv::Mat getImage() const { return image; }
    const cv::String getName() const { return name; }
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
    // 일단 cv::Ptr로 생성하는 방법이 어렵고
    // Feature2D와 DescriptorMatcher의 조합이 맞춰야 하기 때문에
    // 일반 생성자는 공개하지 않고 오직 factory로만 생성하게 한다.
    // 잘 모르는 사용자도 오류 없이 쉽게 사용
    DescHandler(cv::String _name, cv::Ptr<cv::Feature2D> _descMaker,
                      cv::Ptr<cv::DescriptorMatcher> _matcher)
    {
        name = _name;
        descMaker = _descMaker;
        matcher = _matcher;
    }

    cv::Ptr<cv::Feature2D> descMaker;
    cv::Ptr<cv::DescriptorMatcher> matcher;
    cv::String name;
    cv::Mat image;
    std::vector<cv::KeyPoint> keypoints;
    cv::Mat descriptors;
    static cv::Mat result;
};

#endif // DESCRIPTORMATCHER_H
