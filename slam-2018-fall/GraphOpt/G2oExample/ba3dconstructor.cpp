#include "ba3dconstructor.h"

BA3dConstructor::BA3dConstructor()
    : ExampleConstructor()
{
    traj_radius = 2.;
    center = Eigen::Vector3d(1., traj_radius, 0.);
}

void BA3dConstructor::construct()
{
    setPoseVertices();
    addPointVertices();
    // add edges only from points
    addEdgePosePoint();

    // applyNoise
}

void BA3dConstructor::setPoseVertices()
{
    // add pose vertices at (0,0,0) and (1,0,0)
    setFixedPoseVertices();
    // add pose vertices along circle on x-y plane
    addVariablePoseVertices();
}

void BA3dConstructor::setFixedPoseVertices()
{
    // first vertex at origin
    Eigen::Vector3d trans(0,0,0);
    Eigen::Quaterniond q;
    q.setIdentity();
    g2o::SE3Quat pose(q,trans);
    addPoseVertex(pose, true);

    // second vertex at 1,0,0
    pose.setTranslation(Eigen::Vector3d(center(0),0,0));
    addPoseVertex(pose, true);

    // add edge between 0 and 1

}

void BA3dConstructor::addVariablePoseVertices()
{
    const double PI = 3.141592653589793;
    const int N_NODES = 10;
    double angle = PI/double(N_NODES);
    Eigen::Quaterniond q;
    q = Eigen::AngleAxisd(angle, Eigen::Vector3d(0,0,1));
    Eigen::Vector3d trans(traj_radius*sin(angle), traj_radius - traj_radius*cos(angle), 0.);
    g2o::SE3Quat relpose(q, trans);

    for(int i=0; i<N_NODES; i++)
    {
        g2o::SE3Quat pose = gt_poses.back() * relpose;
        addPoseVertex(pose);
        // add edge between poses
    }
    // the last pose supposed to be the same as gt_poses[1]
}

void BA3dConstructor::addPoseVertex(g2o::SE3Quat& pose, bool set_fixed)
{
//    g2o::VertexSE3Expmap* vtx = createPoseVertex(pose);
//    vtx->setFixed(set_fixed);
//    optimizer->addVertex(vtx);
//    gt_poses.push_back(pose);
}

g2o::VertexSE3Expmap* BA3dConstructor::createPoseVertex(g2o::SE3Quat& pose)
{
//    g2o::VertexSE3Expmap* v_se3 = new g2o::VertexSE3Expmap;
//    v_se3->setId(getNewID());
//    v_se3->setEstimate(pose);
//    return v_se3;
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
