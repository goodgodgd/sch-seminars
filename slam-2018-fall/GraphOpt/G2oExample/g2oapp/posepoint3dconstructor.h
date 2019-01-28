#ifndef POSEPOINT3DCONSTRUCTOR_H
#define POSEPOINT3DCONSTRUCTOR_H

#include <math.h>
#include "slam3dconstructor.h"

class PosePoint3DConstructor: public Slam3DConstructor
{
public:
    PosePoint3DConstructor();
    virtual void construct(g2o::SparseOptimizer* _optimizer, G2oConfig& _config);

private:
    void createInitPoseVerts();
    void createCirclePoseVerts();
    void setEdgesBtwPoses();
    void createPointVerts();

    double traj_radius;
    Eigen::Vector3d center;

};

#endif // POSEPOINT3DCONSTRUCTOR_H
