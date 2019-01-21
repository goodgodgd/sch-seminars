#ifndef SE3LOOPCONSTRUCTOR_H
#define SE3LOOPCONSTRUCTOR_H

#include <math.h>
#include <map>
#include "graphconstructor.h"

class SE3LoopConstructor: public GraphConstructor
{
public:
    SE3LoopConstructor();

    virtual void construct();

private:
    void setInitPoseVertices();
    void setCirclePoseVertices();
    void setEdgesBtwPoses();

    void addPoseVertex(Eigen::Quaterniond quat, Eigen::Vector3d tran, bool set_fixed=false);
    void addPoseVertex(g2o::SE3Quat& pose, bool set_fixed=false);
    void addEdgePosePose(int id0, int id1, g2o::SE3Quat &relpose);

    void addPointVertices();
    void addEdgePosePoint();


    std::vector<g2o::SE3Quat> gt_poses;
    double traj_radius;
    Eigen::Vector3d center;
};

#endif // SE3LOOPCONSTRUCTOR_H
