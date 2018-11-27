#include "deschandler.h"

cv::Mat DescHandler::resultimg;
bool DescHandler::enableDraw = false;

// DescHandler factory function
DescHandler* DescHandler::factory(std::string descType, std::string matcherType)
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

//---------- normal member functions ----------//
DescHandler::DescHandler(cv::String _name, cv::Ptr<cv::Feature2D> _descMaker,
                  cv::Ptr<cv::DescriptorMatcher> _matcher)
{
    name = _name;
    descMaker = _descMaker;
    matcher = _matcher;
}

void DescHandler::detectAndCompute(cv::Mat _image)
{
    image = _image.clone();
    descMaker->detectAndCompute(image, cv::Mat(), keypoints, descriptors);
}

std::vector<cv::DMatch> DescHandler::match(const cv::Mat trainDescriptors, float acceptRatio)
{
    std::vector<cv::DMatch> matches;
    matcher->match(descriptors, trainDescriptors, matches);

    // sort matches by score and remove not so good matches
    std::sort(matches.begin(), matches.end());
    const int numGoodMatches = matches.size() * acceptRatio;
    matches.erase(matches.begin()+numGoodMatches, matches.end());
    return matches;
}

void DescHandler::extractByIndices(std::vector<int>& indices)
{
    std::vector<cv::KeyPoint> neoKeypoints;
    cv::Mat neoDescriptors = cv::Mat(0, descriptors.cols, descriptors.type());

    for(int i: indices)
    {
        neoKeypoints.push_back(keypoints[i]);
        neoDescriptors.push_back(descriptors.row(i));
    }

    keypoints = neoKeypoints;
    descriptors = neoDescriptors;
    std::cout << "extract by indices: " << indices.size() << " " << keypoints.size()
              << " " << descriptors.rows << std::endl;
}

//---------- static member functions ----------//
void DescHandler::drawAndAppendResult(const DescHandler* query, const DescHandler* train,
                                const std::vector<cv::DMatch>& matches)
{
    if(enableDraw==false)   return;

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
    if(resultimg.empty())
        resultimg = matchimg;
    else
    {
        std::vector<cv::Mat> himgs = {resultimg, matchimg};
        cv::vconcat(himgs, resultimg);
    }
}

cv::Mat DescHandler::getResultingImg(int maxheight)
{
    cv::Mat resimg = resultimg.clone();
    resultimg.release();
    if(maxheight > 100 && maxheight < resimg.rows)
    {
        float scale = float(maxheight) / float(resimg.rows);
        cv::Size neosize(int(resimg.cols * scale), int(resimg.rows * scale));
        cv::resize(resimg, resimg, neosize);
    }
    return resimg;
}
