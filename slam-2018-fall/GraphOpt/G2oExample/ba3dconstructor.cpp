#include "ba3dconstructor.h"

BA3dConstructor::BA3dConstructor()
    : ExampleConstructor()
{
    center = Eigen::Vector3d(1,2,0);
}

void BA3dConstructor::construct()
{
    addPoseVertices();
    addPointVertices();
    addEdgePosePose();
    addEdgePosePoint();

    // applyNoise
}

void BA3dConstructor::addPoseVertices()
{
    // add pose vertices at (0,0,0) and (1,0,0)
    addFixedPoseVertices();
    // add pose vertices along circle on x-y plane
    addVariablePoseVertices();
}

void BA3dConstructor::addFixedPoseVertices()
{
    Eigen::Vector3d trans(0,0,0);
    Eigen::Quaterniond q;
    q.setIdentity();
    g2o::SE3Quat pose(q,trans);

    // first vertex at origin
    g2o::VertexSE3Expmap* vtx1 = createPoseVertex(pose);
    pose->setFixed(true);
    optimizer.addVertex(vtx1);
    gt_poses.push_back(pose);

    // second vertex at 1,0,0
    pose.setTranslation(Eigen::Vector3d(center(0),0,0));
    g2o::VertexSE3Expmap* vtx2 = createPoseVertex(pose);
    vtx2->setFixed(true);
    optimizer.addVertex(vtx2);
    gt_poses.push_back(pose);
}

void BA3dConstructor::addVariablePoseVertices()
{
    const double PI = 3.141592653589793;
    const int N_NODES = 10;
    double angle = PI/double(N_NODES);
    Eigen::Quaterniond q = Eigen::AngleAxisd(angle, Eigen::Vector(0,0,1));
    Eigen::Vector3d trans;
    g2o::SE3Quat relpose(q, trans);

    for(int i=0; i<10; i++)
    {
        // last pose *= relpose
        // add vertex
    }
}

g2o::VertexSE3Expmap* BA3dConstructor::createPoseVertex(g2o::SE3Quat& pose)
{
    g2o::VertexSE3Expmap* v_se3 = new g2o::VertexSE3Expmap();
    v_se3->setId(getNewID());
    v_se3->setEstimate(pose);
    return v_se3;
}

void BA3dConstructor::addPointVertices()
{

}

void BA3dConstructor::addEdgePosePose()
{

}

void BA3dConstructor::addEdgePosePoint()
{

}
