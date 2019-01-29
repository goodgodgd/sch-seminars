#ifndef STREAM_OPERATORS_H
#define STREAM_OPERATORS_H

#include <iostream>
#include <g2o/types/slam3d/types_slam3d.h>
#include <eigen3/Eigen/Eigen>

void print_se3(const g2o::SE3Quat pose, const std::string heading)
{
    Eigen::IOFormat OctaveFmt(4, 0, ", ", ";\n", "", "", "[", "]");
    std::cout << heading << "t=" << pose.translation().transpose().format(OctaveFmt)
        << ", r=" << pose.rotation().coeffs().transpose().format(OctaveFmt)
        << std::endl;
}

void print_vec3(const Eigen::Vector3d vec, const std::string heading, bool linebreak)
{
    Eigen::IOFormat OctaveFmt(4, 0, ", ", ";\n", "", "", "[", "]");
    std::cout << heading << "t=" << vec.transpose().format(OctaveFmt);
    if(linebreak)
        std::cout << std::endl;
}

#endif // STREAM_OPERATORS_H
