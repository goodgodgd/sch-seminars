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
    curPose = computePoseAndPoints(curframe, points3D);
    std::cout << "points: " << points3D.size() << std::endl;
}

Pose6DofQt MotionTracker::computePoseAndPoints(
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
        if(int(mask.at<uchar>(i))>0)
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
    std::cout << "triangulated points " << ", " << triangulatedPts.cols
              << " type:" << triangulatedPts.type() << std::endl
              << triangulatedPts.colRange(0, 3) << std::endl;
//    std::cout << "mask" << mask.t() << std::endl;

    // set transformation output
    Pose6DofQt outPose = toPoseVector(R, t);

    // normalize scales of points to 1
    float scale;
    std::vector<cv::DMatch> matches_pose_inlier;
    triangulatedPts.convertTo(triangulatedPts, CV_32F);
    recon3Dpts.clear();
    for(int i=0; i<triangulatedPts.cols; i++)
    {
        if(int(mask.at<uchar>(i))>0)
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
    std::cout << "matchres size=" << matchres.rows << ", " << matchres.cols << std::endl;
    cv::imshow("match result", matchres);
    cv::waitKey(10);
    std::cout << "draw matchs" << std::endl;

    return outPose;
}

Pose6DofQt MotionTracker::toPoseVector(cv::Mat R, cv::Mat t)
{
    Eigen::Matrix3f Re;
    Eigen::Vector3f te;
    cv::cv2eigen(R, Re);
    cv::cv2eigen(t, te);
    Pose6DofQt pose(te, Re);
    return pose;
}

bool MotionTracker::beforeSetup()
{
    return points3D.empty();
}

void MotionTracker::run()
{
    cap >> curframe;
    showImage(curframe, "curframe", 300);

    if(beforeSetup())
    {
        draw3DPose(curPose);
        return;
    }

//    curPose = computePoseByProjection(curframe, points3D);
    draw3DPose(curPose);
    qDebug() << "current pose" << curPose;
}

Pose6DofQt MotionTracker::computePoseByProjection(cv::Mat curframe,
        const std::vector<cv::Point3f>& points3D)
{
    // TODO: 3D point와 이미지 점을 매칭해서 solvePnP에 입력해야함

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

    cv::Mat dist_coeff = (cv::Mat_<double>(5, 1) << 0., 0., 0., 0., 0.);
    cv::Mat rvec, rmat, tvec;
    cv::solvePnP(points3D, curfpts_aligned, camK, dist_coeff, rvec, tvec);
    cv::Rodrigues(rvec, rmat);
    return toPoseVector(rmat, tvec);
}

void MotionTracker::showImage(cv::Mat image, std::string wndname, int height)
{
    cv::Mat resimg = image.clone();
    if(resimg.rows != height)
    {
        float scale = float(height) / float(resimg.rows);
        cv::Size neosize(int(resimg.cols * scale), int(resimg.rows * scale));
        cv::resize(resimg, resimg, neosize);
    }
    cv::imshow(wndname, resimg);
    cv::waitKey(10);
}

void MotionTracker::draw3DPose(const Pose6DofQt& curPose)
{
    static std::vector<QVector3D> localVerts;
    // initialze localVerts in local frame
    if(localVerts.empty())
    {
        localVerts.emplace_back(0, 0, 0);
        localVerts.emplace_back(0.1f, 0.1f, 0.1f);
        localVerts.emplace_back(0.1f, -0.1f, 0.1f);
        localVerts.emplace_back(-0.1f, -0.1f, 0.1f);
        localVerts.emplace_back(-0.1f, 0.1f, 0.1f);
    }

    std::vector<QVector3D> globalVerts;
    for(auto& lvert : localVerts)
        globalVerts.push_back( curPose * lvert );

    QVector3D color(1,1,0);
    QVector3D normal(0,0,1);
    // from camera origin to image vertices
    gvm::AddVertex(VertexType::line, globalVerts[0], color, normal, 1);
    gvm::AddVertex(VertexType::line, globalVerts[1], color, normal, 1, true);
    gvm::AddVertex(VertexType::line, globalVerts[0], color, normal, 1);
    gvm::AddVertex(VertexType::line, globalVerts[2], color, normal, 1, true);
    gvm::AddVertex(VertexType::line, globalVerts[0], color, normal, 1);
    gvm::AddVertex(VertexType::line, globalVerts[3], color, normal, 1, true);
    gvm::AddVertex(VertexType::line, globalVerts[0], color, normal, 1);
    gvm::AddVertex(VertexType::line, globalVerts[4], color, normal, 1, true);
    // between image vertices
    gvm::AddVertex(VertexType::line, globalVerts[1], color, normal, 1);
    gvm::AddVertex(VertexType::line, globalVerts[2], color, normal, 1, true);
    gvm::AddVertex(VertexType::line, globalVerts[2], color, normal, 1);
    gvm::AddVertex(VertexType::line, globalVerts[3], color, normal, 1, true);
    gvm::AddVertex(VertexType::line, globalVerts[3], color, normal, 1);
    gvm::AddVertex(VertexType::line, globalVerts[4], color, normal, 1, true);
    gvm::AddVertex(VertexType::line, globalVerts[4], color, normal, 1);
    gvm::AddVertex(VertexType::line, globalVerts[1], color, normal, 1, true);

    // TODO: pose뿐만 아니라 3차원 점도 그려보자
}
