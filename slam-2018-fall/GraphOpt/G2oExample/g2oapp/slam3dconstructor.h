#ifndef SLAM3DCONSTRUCTOR_H
#define SLAM3DCONSTRUCTOR_H

#include "graphconstructor.h"
#include "g2o/types/slam3d/types_slam3d.h"

class Slam3DConstructor: public GraphConstructor
{
public:
    Slam3DConstructor() : GraphConstructor() {}

protected:
    g2o::SE3Quat addNoisePoseMeasurement(const g2o::SE3Quat& srcpose);
    void addEdgePosePose(int id0, int id1, g2o::SE3Quat &relpose);
    void addPoseVertex(Eigen::Quaterniond quat, Eigen::Vector3d tran,
                       bool set_fixed=false);
    void addPoseVertex(g2o::SE3Quat& pose, bool set_fixed=false);

    std::vector<g2o::SE3Quat> gt_poses;
};

#endif // SLAM3DCONSTRUCTOR_H
