#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>


std::vector<cv::Point3f> generate3dPoints(int npts)
{
    cv::Mat rnd(3, npts, CV_32F);
    cv::randu(rnd.rowRange(0, 2), cv::Scalar::all(-1.f), cv::Scalar::all(1.f));
    cv::randu(rnd.row(2), cv::Scalar::all(1.f), cv::Scalar::all(2.f));
    std::vector<cv::Point3f> points;
    for(int i=0; i<npts; i++)
        points.emplace_back(rnd.at<float>(0,i), rnd.at<float>(1,i), rnd.at<float>(2,i));
    return points;
}

// Calculates rotation matrix given euler angles.
cv::Mat eulerAnglesToRotationVector(cv::Vec3f eulers)
{
    // Calculate rotation about x axis
    cv::Mat R_x = (cv::Mat_<float>(3,3) <<
               1,       0,               0,
               0,       cos(eulers[0]),  -sin(eulers[0]),
               0,       sin(eulers[0]),  cos(eulers[0])  );
    // Calculate rotation about y axis
    cv::Mat R_y = (cv::Mat_<float>(3,3) <<
               cos(eulers[1]),    0,  sin(eulers[1]),
               0,                 1,  0,
               -sin(eulers[1]),   0,  cos(eulers[1])  );
    // Calculate rotation about z axis
    cv::Mat R_z = (cv::Mat_<float>(3,3) <<
               cos(eulers[2]),  -sin(eulers[2]),  0,
               sin(eulers[2]),  cos(eulers[2]),   0,
               0,               0,                1  );

    // Combined rotation matrix
    cv::Mat R = R_z * R_y * R_x;
    cv::Mat rvec;
    cv::Rodrigues(R, rvec);
    return rvec;
}

void testPoseEstimation(cv::Vec3f trans, cv::Vec3f rotat)
{
    cv::Mat camK = (cv::Mat_<float>(3, 3)
                    << 300.f, 0.f, 100.f,   0.f, 100.f, 100.f,   0.f, 0.f, 1.f);
    std::cout << "camera matrix: " << std::endl << camK << std::endl;

    // 3 x N
    int npts = 100;
    std::vector<cv::Point3f> points3d = generate3dPoints(npts);
    for(int i=0; i<10; i++)
        std::cout << "3d point " << points3d[i] << std::endl;

    std::vector<cv::Point2f> imgpts1, imgpts2;
    cv::Mat rvec = eulerAnglesToRotationVector(cv::Vec3f(0.f, 0.f, 0.f));
    cv::Mat tvec = cv::Mat::zeros(3, 1, CV_32F);
    cv::Mat dist = cv::Mat::zeros(4, 1, CV_32F);
    cv::projectPoints(points3d, rvec, tvec, camK, dist, imgpts1);

    rvec = eulerAnglesToRotationVector(rotat);
    tvec = (cv::Mat_<float>(3,1) << trans(0), trans(1), trans(2));
    std::cout << "rvec " << rvec.t() << std::endl;
    std::cout << "tvec " << tvec.t() << std::endl;
    cv::projectPoints(points3d, rvec, tvec, camK, dist, imgpts2);

    cv::Mat mask, R, t, triangulated;
    cv::Mat E = cv::findEssentialMat(imgpts1, imgpts2, camK, cv::RANSAC, 0.9, 1.0, mask);
    int incounts=0;
    for(int i=0; i<mask.rows; i++)
        incounts++;
    std::cout << "findEssentialMat inliers" << incounts  << std::endl;

    cv::recoverPose(E, imgpts1, imgpts2, camK, R, t, 1000.f, mask, triangulated);
    incounts=0;
    for(int i=0; i<mask.rows; i++)
        incounts++;
    std::cout << "recoverPose inliers" << incounts  << std::endl;

    cv::Mat rvecout;
    cv::Rodrigues(R, rvecout);
    std::cout << "rotat " << rvecout.t() << std::endl;
    std::cout << "trans " << t.t() << std::endl;
}


int main()
{
    cv::Vec3f trans(1.f, 0, 0);
    cv::Vec3f rotat(0, 0.5f, 0);
    testPoseEstimation(trans, rotat);

    return 0;
}
