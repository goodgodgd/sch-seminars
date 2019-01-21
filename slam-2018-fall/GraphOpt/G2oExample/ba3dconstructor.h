#ifndef BA3DCONSTRUCTOR_H
#define BA3DCONSTRUCTOR_H

#include <exampleconstructor.h>
#include <math.h>
#include <map>

class BA3dConstructor : public ExampleConstructor
{
public:
    BA3dConstructor();
    virtual void construct();

private:
    void setPoseVertices();
    void addPointVertices();
    void addEdgePosePose();
    void addEdgePosePoint();

    void setFixedPoseVertices();
    void addVariablePoseVertices();
    void addPoseVertex(g2o::SE3Quat& pose, bool set_fixed=false);
    g2o::VertexSE3Expmap* createPoseVertex(g2o::SE3Quat& pose);

    std::vector<g2o::SE3Quat> gt_poses;
    double traj_radius;
    Eigen::Vector3d center;
};

#endif // BA3DCONSTRUCTOR_H
