#ifndef SE3POINTCONSTRUCTOR_H
#define SE3POINTCONSTRUCTOR_H

#include <math.h>
#include "se3loopconstructor.h"

class Se3PointConstructor: public SE3LoopConstructor
{
public:
    Se3PointConstructor();
    virtual void construct(g2o::SparseOptimizer* _optimizer, G2oConfig& _config);

private:
    void createPointVerts();
    void setEdgesBtwPosePoint();

    std::vector<Eigen::Vector3d> gt_points;
};

#endif // SE3POINTCONSTRUCTOR_H
