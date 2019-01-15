#ifndef BA3DCONSTRUCTOR_H
#define BA3DCONSTRUCTOR_H

#include <exampleconstructor.h>

class BA3dConstructor : public ExampleConstructor
{
public:
    BA3dConstructor();
    virtual void construct();

private:
    void addPoseVertices();
    void addPointVertices();
    void addEdgePosePose();
    void addEdgePosePoint();

    void addFixedPoseVertices();
    void addVariablePoseVertices();
    g2o::VertexSE3Expmap* createPoseVertex(g2o::SE3Quat& pose);

    std::vector<g2o::SE3Quat> gt_poses;
    Eigen::Vector3d center;
};

#endif // BA3DCONSTRUCTOR_H
