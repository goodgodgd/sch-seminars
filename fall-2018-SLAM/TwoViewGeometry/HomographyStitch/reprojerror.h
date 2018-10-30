#ifndef REPROJERROR_H
#define REPROJERROR_H

#include <opencv2/opencv.hpp>
#include <eigen3/Eigen/Eigen>
#include <opencv2/core/eigen.hpp>

std::vector<int> reprojectionError(std::vector<cv::Point2f>& srcpts,
                                   std::vector<cv::Point2f>& dstpts, cv::Mat cvH)
{
    // create homogeneous coordinates for keypoints
    int npts = srcpts.size();
    Eigen::Matrix3Xf spt = Eigen::Matrix3Xf::Constant(3,npts, 1.f);
    Eigen::Matrix3Xf dpt = Eigen::Matrix3Xf::Constant(3,npts, 1.f);
    for(int i=0; i<srcpts.size(); i++)
    {
        spt(0,i) = srcpts[i].x;
        spt(1,i) = srcpts[i].y;
        dpt(0,i) = dstpts[i].x;
        dpt(1,i) = dstpts[i].y;
    }
    std::cout << "eigen pts: " << std::endl << dpt.block<3,5>(0,0) << std::endl;

    // copy homography to eigen matrix
    Eigen::Matrix3f H;
    cv::cv2eigen(cvH, H);
    std::cout << "eigen H: " << std::endl << H << std::endl;

    // reproject spt onto image1 frame
    Eigen::Matrix3Xf rpjspt = H*spt;
    std::cout << "reprojected spt: " << std::endl << rpjspt.block<3,5>(0,0) << std::endl;
    rpjspt.row(0) = rpjspt.row(0).array() / rpjspt.row(2).array();
    rpjspt.row(1) = rpjspt.row(1).array() / rpjspt.row(2).array();
    rpjspt.row(2) = rpjspt.row(2).array() / rpjspt.row(2).array();
    std::cout << "normalied spt: " << std::endl << rpjspt.block<3,5>(0,0) << std::endl;

    // compute error
    Eigen::Matrix3Xf errmat = dpt - rpjspt;
    errmat = errmat.array() * errmat.array();
    std::cout << "error mat: " << std::endl << errmat.block<3,5>(0,0) << std::endl;
    Eigen::RowVectorXf errvec;
    errvec =  errmat.row(0) + errmat.row(1);
    std::cout << "errors" << std::endl << errvec.head(20) << std::endl;

    // count inliers and outliers
    const float thresh = 2.f;
    int inliers=0, outliers=0;
    std::vector<int> inlierIndices;
    for(int i=0; i<errmat.cols(); i++)
    {
        if(errvec(i) < thresh)
        {
            inlierIndices.push_back(i);
            inliers++;
        }
        else
            outliers++;
    }
    std::cout << "inliers: " << inliers << ", ratio: "
              << float(inliers)/float(errmat.cols()) << std::endl;
    std::cout << "outliers: " << outliers << ", ratio: "
              << float(outliers)/float(errmat.cols()) << std::endl;

    return inlierIndices;
}

#endif // REPROJERROR_H
