#include "motiontracker.h"

MotionTracker::MotionTracker()
{
    cap.open(0);
    if(!cap.isOpened())
        throw std::string("video capture is not open");

    keyfDesc = DescHandler::factory("orb", "flann");
    curfDesc = DescHandler::factory("orb", "flann");
    DescHandler::enableDrawMatches(false);

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

bool MotionTracker::setup()
{
    // set keyframe first
    if(keyframe.empty())
    {
        keyframe = curframe.clone();
        keyfDesc->detectAndCompute(keyframe);
        std::cout << "keyframe was set" << std::endl;
        return false;
    }

    // in the second, compute 3D points from essential matrix
    curPose = computePoseAndPoints(curframe, points3D);
    qDebug() << "setup pose" << curPose << "points" << points3D.size();
    draw();
    return true;
}

Pose6DofQt MotionTracker::computePoseAndPoints(cv::Mat curframe, std::vector<cv::Point3f>& recon3Dpts)
{
    std::vector<cv::DMatch> matches;
    std::vector<cv::Point2f> keyfpoints;
    std::vector<cv::Point2f> curfpoints;

    // find keypoints in current frame, match them with keyframe's keypoints and align them with matches
    matchPoints(curframe, matches, keyfpoints, curfpoints);

    // compute essential matrix and extract inliers from keyfpoints, curfpoints, and matches
    cv::Mat E = findEssentialMatrix(keyfpoints, curfpoints, matches);

    // reconstruct pose and 3d points and extract inliers from keyfpoints, curfpoints, and matches
    Pose6DofQt pose = reconstruct(E, keyfpoints, curfpoints, matches, recon3Dpts);

    cv::Mat matchres = DescHandler::getResultingImg(600);
//    cv::imshow("match result", matchres);
//    cv::waitKey(10);

    // extract descriptors of inlier points
    std::vector<int> reconInds;
    for(auto& match: matches)
        reconInds.push_back(match.trainIdx);
    keyfDesc->extractByIndices(reconInds);

    return pose;
}

void MotionTracker::matchPoints(cv::Mat curframe, std::vector<cv::DMatch>& matches,
                 std::vector<cv::Point2f>& keyfpoints, std::vector<cv::Point2f>& curfpoints)
{
    curfDesc->detectAndCompute(curframe);
    matches = curfDesc->match(keyfDesc->getDescriptors(), 0.8f);

    std::vector<cv::KeyPoint> keyfKeypoints = keyfDesc->getKeypoints();
    std::vector<cv::KeyPoint> curfKeypoints = curfDesc->getKeypoints();
    keyfpoints.clear();
    curfpoints.clear();

    // align keypoints from both images
    for(auto& match: matches)
    {
        keyfpoints.push_back(keyfKeypoints.at(match.trainIdx).pt);
        curfpoints.push_back(curfKeypoints.at(match.queryIdx).pt);
    }
    DescHandler::drawAndAppendResult(curfDesc, keyfDesc, matches);
}

cv::Mat MotionTracker::findEssentialMatrix(std::vector<cv::Point2f>& keyfpoints,
                std::vector<cv::Point2f>& curfpoints, std::vector<cv::DMatch>& matches)
{
    // compute essential matrix
    cv::Mat mask;
    cv::Mat E = cv::findEssentialMat(keyfpoints, curfpoints, camK,
                                     cv::RANSAC, 0.99, 2.0, mask);
    std::cout << "essential" << std::endl << E << std::endl;

    // extract inlier points
    std::vector<cv::Point2f> keyfpts_inlier, curfpts_inlier;
    std::vector<cv::DMatch> matches_inlier;
    for(int i=0; i<mask.rows; i++)
    {
        if(int(mask.at<uchar>(i))>0)
        {
            keyfpts_inlier.push_back(keyfpoints[i]);
            curfpts_inlier.push_back(curfpoints[i]);
            matches_inlier.push_back(matches[i]);
        }
    }
    mask.release();
    std::cout << "inliers by findEssentialMat: " << keyfpts_inlier.size()
              << " from " << keyfpoints.size() << std::endl;
    DescHandler::drawAndAppendResult(curfDesc, keyfDesc, matches_inlier);

    keyfpoints = keyfpts_inlier;
    curfpoints = curfpts_inlier;
    matches = matches_inlier;
    return E;
}

Pose6DofQt MotionTracker::reconstruct(cv::Mat E, std::vector<cv::Point2f>& keyfpoints,
                       std::vector<cv::Point2f>& curfpoints,
                       std::vector<cv::DMatch>& matches,
                       std::vector<cv::Point3f>& recon3Dpts)
{
    // recover pose
    cv::Mat R, t, mask;
    cv::Mat triangPoints;
    cv::recoverPose(E, keyfpoints, curfpoints, camK, R, t, 1000.f, mask,
                    triangPoints);

    std::cout << "recover pose: " << std::endl << "R= " << R << std::endl
              << "t= " << t.t() << std::endl;
    std::cout << "triangulated points " << ", " << triangPoints.cols
              << " type:" << triangPoints.type() << std::endl
              << triangPoints.colRange(0, 3) << std::endl;

    // set transformation output
    Pose6DofQt outPose = toPoseVector(R, t);

    // normalize scales of points to 1
    float scale;
    std::vector<cv::Point2f> keyfpts_inlier, curfpts_inlier;
    std::vector<cv::DMatch> matches_inlier;
    triangPoints.convertTo(triangPoints, CV_32F);
    recon3Dpts.clear();

    for(int i=0; i<triangPoints.cols; i++)
    {
        if(int(mask.at<uchar>(i))>0)
        {
            scale = triangPoints.at<float>(3,i);
            recon3Dpts.emplace_back(triangPoints.at<float>(0,i)/scale,
                                    triangPoints.at<float>(1,i)/scale,
                                    triangPoints.at<float>(2,i)/scale);
            keyfpts_inlier.push_back(keyfpoints[i]);
            curfpts_inlier.push_back(curfpoints[i]);
            matches_inlier.push_back(matches[i]);
        }
    }
    std::cout << "inliers by recoverPose: " << recon3Dpts.size() << " in "
              << triangPoints.cols << std::endl;
    DescHandler::drawAndAppendResult(curfDesc, keyfDesc, matches_inlier);

    keyfpoints = keyfpts_inlier;
    curfpoints = curfpts_inlier;
    matches = matches_inlier;
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
        draw();
        return;
    }

    curPose = computePoseByProjection(curframe, points3D);
    qDebug() << "current pose" << curPose;
    draw();
}

Pose6DofQt MotionTracker::computePoseByProjection(cv::Mat curframe, const std::vector<cv::Point3f>& points3D)
{
    std::vector<cv::DMatch> matches;
    std::vector<cv::Point2f> keyfpoints;
    std::vector<cv::Point2f> curfpoints;

    // find keypoints in current frame, match them with keyframe's keypoints and align them with matches
    matchPoints(curframe, matches, keyfpoints, curfpoints);

    // inliers from points3D
    std::vector<cv::Point3f> pointsInlier;
    for(auto& match: matches)
        pointsInlier.push_back(points3D[match.trainIdx]);

    std::cout << "matches keyframe " << matches.size() << " " << keyfDesc->getKeypoints().size()
              << " points " << pointsInlier.size() << " " << points3D.size() << std::endl;

    cv::Mat dist_coeff = (cv::Mat_<double>(5, 1) << 0., 0., 0., 0., 0.);
    cv::Mat rvec, rmat, tvec;
    cv::solvePnPRansac(pointsInlier, curfpoints, camK, dist_coeff, rvec, tvec);
    cv::Rodrigues(rvec, rmat);
    return toPoseVector(rmat, tvec);

    // draw matches
    cv::Mat matchres = DescHandler::getResultingImg(600);
//    cv::imshow("match result", matchres);
//    cv::waitKey(10);
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

void MotionTracker::draw()
{
    drawPose(curPose);
    drawPoints(points3D);
}

void MotionTracker::drawPose(const Pose6DofQt& pose)
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
        globalVerts.push_back( pose * lvert );

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


}

void MotionTracker::drawPoints(const std::vector<cv::Point3f>& points)
{
    QVector3D color(0,1,1);
    QVector3D normal(0,0,1);
    for(const cv::Point3f& pt: points)
    {
        QVector3D vert(pt.x, pt.y, pt.z);
        gvm::AddVertex(VertexType::point, vert, color, normal, 2);
    }
}
