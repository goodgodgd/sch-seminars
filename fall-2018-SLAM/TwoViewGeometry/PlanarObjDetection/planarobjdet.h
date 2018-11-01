#ifndef PLANAROBJDET_H
#define PLANAROBJDET_H

#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <eigen3/Eigen/Eigen>
#include <opencv2/core/eigen.hpp>
#include "deschandler.h"


class PlanarObjDet
{
public:
    PlanarObjDet(cv::Mat _trainimg)
    {
        trainimg = _trainimg.clone();
        trainHdl = DescHandler::factory("orb", "flann");
        trainHdl->detectAndCompute(trainimg);
    }

    void detect(DescHandler* queryHdl, const float inlierThresh = 0.3f)
    {
        // compute and match descriptors
        std::vector<cv::DMatch> matches = queryHdl->match(trainHdl->getDescriptors(), 0.8f);

        // compute homography
        cv::Mat H = computeHomography(queryHdl->getKeypoints(), trainHdl->getKeypoints(), matches);

        // extract inlier matches
        matches = extractInliers(H, matches, inlierThresh);

        // draw inlier matches
        DescHandler::drawAndAppendResult(queryHdl, trainHdl, matches);
    }

    static cv::Mat resultingImg(const int maxheight = 0)
    {
        return DescHandler::getResultingImg(maxheight);
    }

private:
    cv::Mat computeHomography(const std::vector<cv::KeyPoint>& queryKeypts,
                              const std::vector<cv::KeyPoint>& trainKeypts,
                              std::vector<cv::DMatch>& matches)
    {
        queryPts.clear();
        trainPts.clear();
        for (auto& match: matches)
        {
            queryPts.push_back(queryKeypts.at(match.queryIdx).pt);
            trainPts.push_back(trainKeypts.at(match.trainIdx).pt);
        }
        cv::Mat H = cv::findHomography(queryPts, trainPts, cv::RANSAC);
        std::cout << "homography" << std::endl << H << std::endl;
        return H;
    }

    std::vector<cv::DMatch> extractInliers(cv::Mat cvH, std::vector<cv::DMatch>& matches,
                                           const float inlierThresh)
    {
        std::vector<cv::DMatch> inliers;

        // create homogeneous coordinates for keypoints
        int npts = queryPts.size();
        Eigen::Matrix3Xf querypt = Eigen::Matrix3Xf::Constant(3,npts, 1.f);
        Eigen::Matrix3Xf trainpt = Eigen::Matrix3Xf::Constant(3,npts, 1.f);
        for(int i=0; i<queryPts.size(); i++)
        {
            querypt(0,i) = queryPts[i].x;
            querypt(1,i) = queryPts[i].y;
            trainpt(0,i) = trainPts[i].x;
            trainpt(1,i) = trainPts[i].y;
        }

        // copy homography to eigen matrix
        Eigen::Matrix3f H;

        cv::cv2eigen(cvH, H);

        // reproject querypt onto image1 frame
        Eigen::Matrix3Xf rpjqpt = H*querypt;
        rpjqpt.row(0) = rpjqpt.row(0).array() / rpjqpt.row(2).array();
        rpjqpt.row(1) = rpjqpt.row(1).array() / rpjqpt.row(2).array();
        rpjqpt.row(2) = rpjqpt.row(2).array() / rpjqpt.row(2).array();

        // compute error
        Eigen::Matrix3Xf errmat = trainpt - rpjqpt;
        errmat = errmat.array() * errmat.array();
        Eigen::RowVectorXf errvec;
        errvec =  errmat.row(0) + errmat.row(1);
        std::cout << "errors" << std::endl << errvec.head(20) << std::endl;

        // count inliers and outliers
        const float thresh = 2.f;
        std::vector<cv::DMatch> inlierMatches;
        for(int i=0; i<errmat.cols(); i++)
        {
            if(errvec(i) < thresh)
                inlierMatches.push_back(matches.at(i));
        }
        float inlierRatio = float(inlierMatches.size()) / float(matches.size());
        std::cout << "### inliers: " << inlierMatches.size() << ", ratio: " << inlierRatio << std::endl;

        // if inliers are not sufficient, clear inliers
        if(inlierRatio < inlierThresh)
            inlierMatches.clear();
        // to make inlierMatches to be non-empty, push one
        inlierMatches.push_back(matches.at(0));

        return inlierMatches;
    }

    cv::Mat trainimg;
    DescHandler* trainHdl;
    std::vector<cv::Point2f> queryPts;
    std::vector<cv::Point2f> trainPts;
    float inlierThresh;
};

#endif // PLANAROBJDET_H
