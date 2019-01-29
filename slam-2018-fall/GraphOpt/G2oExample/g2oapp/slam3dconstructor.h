#ifndef SLAM3DCONSTRUCTOR_H
#define SLAM3DCONSTRUCTOR_H

#include "graphconstructor.h"
#include "g2o/types/slam3d/types_slam3d.h"

class Slam3DConstructor: public GraphConstructor
{
public:
    Slam3DConstructor() : GraphConstructor() {}
    virtual void construct(g2o::SparseOptimizer* _optimizer, G2oConfig& _config)=0;

protected:
    void addPoseVertex(g2o::SE3Quat* pose=nullptr, bool set_fixed=false);
    g2o::SE3Quat addNoisePoseMeasurement(const g2o::SE3Quat& srcpose);
    void addEdgePosePose(int id0, int id1, const g2o::SE3Quat &relpose);

    void setParameter();
    void addPoint3DVertex(Eigen::Vector3d* pt=nullptr, bool set_fixed=false);
    Eigen::Vector3d addNoisePointMeasurement(const Eigen::Vector3d& srcpt);
    void addEdgePosePoint(int poseid, int ptid, const Eigen::Vector3d& relpt);

    void print_se3(const g2o::SE3Quat pose, const std::string heading)
    {
        if(!config.verbosity) return;
        Eigen::IOFormat OctaveFmt(4, 0, ", ", ";\n", "", "", "[", "]");
        std::cout << heading << "t=" << pose.translation().transpose().format(OctaveFmt)
            << ", r=" << pose.rotation().coeffs().transpose().format(OctaveFmt)
            << std::endl;
    }

    void print_vec3(const Eigen::Vector3d vec, const std::string heading, bool linebreak)
    {
        if(!config.verbosity) return;
        Eigen::IOFormat OctaveFmt(4, 0, ", ", ";\n", "", "", "[", "]");
        std::cout << heading << "t=" << vec.transpose().format(OctaveFmt);
        if(linebreak)
            std::cout << std::endl;
    }
};

#endif // SLAM3DCONSTRUCTOR_H
