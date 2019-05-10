#ifndef SE3LOOPCONSTRUCTOR_H
#define SE3LOOPCONSTRUCTOR_H

#include <math.h>
#include "slam3dconstructor.h"

class SE3LoopConstructor: public Slam3DConstructor
{
public:
    SE3LoopConstructor();

    virtual void construct(g2o::SparseOptimizer* _optimizer, G2oConfig& _config);

protected:
    void createInitPoseVerts();
    void createCirclePoseVerts();
    void setEdgesBtwPoses();

    double traj_radius;
    Eigen::Vector3d center;
    std::vector<g2o::SE3Quat> gt_poses;
};

#endif // SE3LOOPCONSTRUCTOR_H
