// This source code is from https://github.com/sunglok/3dv_tutorial
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include "reprojerror.h"

int main()
{
    // Load two images (c.f. We assume that two images have the same size and type)
    cv::Mat image1 = cv::imread(std::string(PRJ_PATH) + "/../data/hill01.jpg");
    cv::Mat image2 = cv::imread(std::string(PRJ_PATH) + "/../data/hill02.jpg");
    if (image1.empty() || image2.empty())
    {
        std::cout << "image NOT found" << std::endl;
        return -1;
    }

    // Retrieve matching points
    cv::Mat gray1, gray2;
    if (image1.channels() > 1)
    {
        cv::cvtColor(image1, gray1, CV_RGB2GRAY);
        cv::cvtColor(image2, gray2, CV_RGB2GRAY);
    }
    else
    {
        gray1 = image1.clone();
        gray2 = image2.clone();
    }
    cv::Ptr<cv::FeatureDetector> detector = cv::xfeatures2d::SURF::create();  // SURF features
    std::vector<cv::KeyPoint> keypoint1, keypoint2;
    detector->detect(gray1, keypoint1);
    detector->detect(gray2, keypoint2);
    std::cout << "num of key points: " << keypoint1.size() << ", " << keypoint2.size() << std::endl;

    cv::Ptr<cv::FeatureDetector> extractor = cv::xfeatures2d::SURF::create(); // SURF descriptors
    cv::Mat descriptor1, descriptor2;
    extractor->compute(gray1, keypoint1, descriptor1);
    extractor->compute(gray2, keypoint2, descriptor2);
    cv::FlannBasedMatcher matcher; // Approximate Nearest Neighbors (ANN) matcher
    std::vector<cv::DMatch> match;
    matcher.match(descriptor1, descriptor2, match);
    std::cout << "num of matchings: " << match.size() << std::endl;

    // compute planar homography and merge two images
    std::vector<cv::Point2f> points1, points2;
    for (size_t i = 0; i < match.size(); i++)
    {
        points1.push_back(keypoint1.at(match.at(i).queryIdx).pt);
        points2.push_back(keypoint2.at(match.at(i).trainIdx).pt);
    }
    cv::Mat H = cv::findHomography(points2, points1, cv::RANSAC);
    std::cout << "homography" << std::endl << H << std::endl;
    cv::Mat merge;
    cv::warpPerspective(image2, merge, H, cv::Size(image1.cols * 2, image1.rows));
    merge.colRange(0, image1.cols) = image1 * 1; // Copy

    // decompose homography into rotation(R), translation(t), and normal(n)
    float intrinsic[] = {300.f, 0.f, image1.cols/2.f,
                         0.f, 300.f, image1.rows/2.f,
                         0.f, 0.f, 1.f};
    cv::Mat K = cv::Mat(3, 3, CV_32FC1, intrinsic);
    std::vector<cv::Mat> R, t, n;
    std::cout << "arbitrary K= " << K << std::endl;
    cv::decomposeHomographyMat(H, K, R, t, n);
    std::cout << "\nhomography decomposition: transformation from src(2, right) to dst(1, left)\n";
    for(int i=0; i<4; i++)
    {
        std::cout << "possible decomposition " << i << std::endl;
        std::cout << "R= " << R[i] << "\nt= " << t[i].t() << "\nn= " << n[i].t() << std::endl;
    }

    // find inlier matches from homography reprojection error
    std::vector<int> inlierIndices = reprojectionError(points2, points1, H);
    std::vector<cv::DMatch> inlierMatch;
    for(int ind: inlierIndices)
        inlierMatch.push_back(match.at(ind));

    // draw matches: all matches + only inlier matches
    cv::Mat inlierMatchImg, matchImg, viewImg;
    cv::drawMatches(image1, keypoint1, image2, keypoint2, inlierMatch, inlierMatchImg);
    cv::drawMatches(image1, keypoint1, image2, keypoint2, match, matchImg);
    std::vector<cv::Mat> images = {merge, matchImg, inlierMatchImg};
    cv::vconcat(images, viewImg);

    // Show the merged image
    cv::imshow("3DV Tutorial: Image Stitching", viewImg);
    printf("Press any key to terminate tihs program!\n");
    cv::waitKey(0);
    return 0;
}
