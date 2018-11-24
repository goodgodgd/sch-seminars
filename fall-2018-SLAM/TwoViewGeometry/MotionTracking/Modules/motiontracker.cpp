#include "motiontracker.h"

MotionTracker::MotionTracker()
{
    cap.open(0);
    if(!cap.isOpened())
        throw std::string("video capture is not open");

    keyfDesc = DescHandler::factory("orb", "flann");
    curfDesc = DescHandler::factory("orb", "flann");

    // camera matrix K
    cv::Mat frame;
    cap >> frame;
    double intrinsics[9] = {300., 0., double(frame.cols/2),
                            0., 300., double(frame.rows/2),
                            0., 0., 1.};
    camK = cv::Mat(3, 3, CV_64F, intrinsics);
    /// IMPORTANT!!
    /// when raw data is desctroyed at end of this function,
    /// the matrix's data is also destroyed
    camK = camK.clone();
    std::cout << "camera matrix: " << std::endl << camK << std::endl;
}

void MotionTracker::setup()
{
    // set keyframe first
    if(keyframe.empty())
    {
        keyframe = curframe.clone();
        keyfDesc->detectAndCompute(keyframe);
        std::cout << "keyframe was set" << std::endl;
        return;
    }

    // in the second, compute 3D points from essential matrix
    curTfm = computePoseAndPoints(curframe, points3D);
}

cv::Mat MotionTracker::computePoseAndPoints(
        cv::Mat curframe, std::vector<cv::Point3f>& recon3Dpts)
{
    // detect keypoints, compute descriptors, match current descriptors with keyframe's
    curfDesc->detectAndCompute(curframe);
    std::vector<cv::DMatch> matches = curfDesc->match(keyfDesc->getDescriptors(), 0.8f);

    // align keypoints from both images
    std::vector<cv::KeyPoint> keyfpoints = keyfDesc->getKeypoints();
    std::vector<cv::KeyPoint> curfpoints = curfDesc->getKeypoints();
    std::vector<cv::Point2f> keyfpts_aligned, curfpts_aligned;
    for(auto& match: matches)
    {
        keyfpts_aligned.push_back(keyfpoints.at(match.trainIdx).pt);
        curfpts_aligned.push_back(curfpoints.at(match.queryIdx).pt);
    }
    DescHandler::drawAndAppendResult(curfDesc, keyfDesc, matches);


    // compute essential matrix
    cv::Mat mask;
    cv::Mat E = cv::findEssentialMat(keyfpts_aligned, curfpts_aligned, camK,
                                     cv::RANSAC, 0.99, 2.0, mask);
    std::cout << "essential" << std::endl << E << std::endl;

    // extract inlier points
    std::vector<cv::Point2f> keyfpts_inlier, curfpts_inlier;
    std::vector<cv::DMatch> matches_inlier;
    for(int i=0; i<mask.rows; i++)
    {
        if(mask.at<uchar>(i)==1)
        {
            keyfpts_inlier.push_back(keyfpts_aligned[i]);
            curfpts_inlier.push_back(curfpts_aligned[i]);
            matches_inlier.push_back(matches[i]);
        }
    }
    mask.release();
    std::cout << "inliers from findEssentialMat: " << keyfpts_inlier.size()
              << " from " << keyfpts_aligned.size() << std::endl;
    DescHandler::drawAndAppendResult(curfDesc, keyfDesc, matches_inlier);


    // recover pose
    cv::Mat R, t;
    cv::Mat triangulatedPts;
    cv::recoverPose(E, keyfpts_inlier, curfpts_inlier, camK, R, t, 1000.f, mask,
                    triangulatedPts);

    std::cout << "recover pose: " << std::endl << "R= " << R << std::endl
              << "t= " << t.t() << std::endl;
    std::cout << "triangulated points " << triangulatedPts.rows
              << ", " << triangulatedPts.cols
              << " type:" << triangulatedPts.type() << std::endl
              << triangulatedPts.colRange(0, 3) << std::endl;

    // set transformation output
    cv::Mat outTransform;
    cv::Mat Rt = cv::Mat::eye(4, 4, CV_64F);
    R.copyTo(Rt(cv::Rect(0,0,3,3)));
    t.copyTo(Rt(cv::Rect(3,0,1,3)));
    Rt.convertTo(outTransform, CV_32F);
    std::cout << "transformation type=" << outTransform.type() << std::endl
              << outTransform << std::endl;

    // normalize scales of points to 1
    float scale;
    std::vector<cv::DMatch> matches_pose_inlier;
    triangulatedPts.convertTo(triangulatedPts, CV_32F);
    recon3Dpts.clear();
    for(int i=0; i<triangulatedPts.cols; i++)
    {
        if(mask.at<uchar>(i)==1)
        {
            scale = triangulatedPts.at<float>(3,i);
            recon3Dpts.emplace_back(triangulatedPts.at<float>(0,i)/scale,
                               triangulatedPts.at<float>(1,i)/scale,
                               triangulatedPts.at<float>(2,i)/scale);
            matches_pose_inlier.push_back(matches_inlier[i]);
        }
    }
    std::cout << "recoverpose inliers: " << recon3Dpts.size() << " in "
              << triangulatedPts.cols << std::endl;

//    std::cout << "inlier points" << std::endl;
//    for(auto& pt: pts3d)
//        std::cout << pt << std::endl;

    DescHandler::drawAndAppendResult(curfDesc, keyfDesc, matches_pose_inlier);
    cv::Mat matchres = DescHandler::getResultingImg(1000);
    cv::imshow("matchres", matchres);
    cv::waitKey(10);

    return outTransform;
}

void MotionTracker::isSetupFinished()
{

}

void MotionTracker::run()
{
    cap >> curframe;
    cv::imshow("curframe", curframe);
    cv::waitKey(10);

//    if(isSetupFinished()==false)
//        return;


}
