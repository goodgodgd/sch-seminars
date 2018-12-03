#ifndef DESCRIPTORMATCHER_H
#define DESCHANDLER_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>


class DescHandler
{
public:
    // DescHandler factory function
    static DescHandler* factory(std::string descType, std::string matcherType);

    //---------- normal member functions ----------//
    void detectAndCompute(cv::Mat _image);
    std::vector<cv::DMatch> match(const cv::Mat trainDescriptors, float acceptRatio = 0.5f);
    void extractByIndices(std::vector<int>& indices);

    const cv::Mat getDescriptors() const { return descriptors; }
    const cv::Mat getImage() const { return image; }
    const cv::String getName() const { return name; }
    const std::vector<cv::KeyPoint>& getKeypoints() const { return keypoints; }

    //---------- static member functions ----------//
    static void drawAndAppendResult(const DescHandler* query, const DescHandler* train,
                                    const std::vector<cv::DMatch>& matches);
    static cv::Mat getResultingImg(int maxheight = 0);
    static void enableDrawMatches(bool enable) { enableDraw = enable; }

private:
    // 일단 cv::Ptr로 생성하는 방법이 어렵고
    // Feature2D와 DescriptorMatcher의 조합이 맞춰야 하기 때문에
    // 일반 생성자는 공개하지 않고 오직 factory로만 생성하게 한다.
    // 잘 모르는 사용자도 오류 없이 쉽게 사용
    DescHandler(cv::String _name, cv::Ptr<cv::Feature2D> _descMaker,
                      cv::Ptr<cv::DescriptorMatcher> _matcher);

    static cv::Mat resultimg;
    static bool enableDraw;

    cv::Ptr<cv::Feature2D> descMaker;
    cv::Ptr<cv::DescriptorMatcher> matcher;
    cv::String name;
    cv::Mat image;
    std::vector<cv::KeyPoint> keypoints;
    cv::Mat descriptors;

};

#endif // DESCRIPTORMATCHER_H
