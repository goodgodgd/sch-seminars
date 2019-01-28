#ifndef POSEPOINT3DCONSTRUCTOR_H
#define POSEPOINT3DCONSTRUCTOR_H

#include <math.h>
#include "se3loopconstructor.h"
#include "g2o/types/sba/types_six_dof_expmap.h"

class PosePoint3DConstructor: public SE3LoopConstructor
{
public:
    PosePoint3DConstructor();
    virtual void construct(g2o::SparseOptimizer* _optimizer, G2oConfig& _config);

private:
    void createPointVerts();
    void setEdgesBtwPosePoint();

    std::vector<Eigen::Vector3d> gt_points;
};

#endif // POSEPOINT3DCONSTRUCTOR_H
