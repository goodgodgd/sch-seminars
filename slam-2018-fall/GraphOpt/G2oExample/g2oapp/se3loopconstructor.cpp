#include "se3loopconstructor.h"

SE3LoopConstructor::SE3LoopConstructor()
    : GraphConstructor()
{
    traj_radius = 2.;
    center = Eigen::Vector3d(1., traj_radius, 0.);
}

void SE3LoopConstructor::construct()
{
    // add pose vertices at (0,0,0) and (1,0,0)
    setInitPoseVertices();
    // add pose vertices around circle
    setCirclePoseVertices();
    // add edges between pose edges
    setEdgesBtwPoses();

    addPointVertices();
    addEdgePosePoint();

    // applyNoise

}

void SE3LoopConstructor::setInitPoseVertices()
{
    Eigen::Vector3d tran;
    Eigen::Quaterniond quat;

    // first vertex at origin
    tran = Eigen::Vector3d(0,0,0);
    quat.setIdentity();
    addPoseVertex(quat, tran, true);

    // second vertex at 1,0,0
    tran = Eigen::Vector3d(center(0),0,0);
    quat.setIdentity();
    addPoseVertex(quat, tran, true);
}

void SE3LoopConstructor::setCirclePoseVertices()
{
    const double PI = 3.141592653589793;
    const int CIRCLE_NODES = 10;
    double angle = 2.*PI/double(CIRCLE_NODES);
    Eigen::Quaterniond quat;
    quat = Eigen::AngleAxisd(angle, Eigen::Vector3d(0,0,1));
    Eigen::Vector3d tran = Eigen::Vector3d(traj_radius*sin(angle), traj_radius - traj_radius*cos(angle), 0.);
    g2o::SE3Quat relpose(quat, tran);

    // add vertices around a circle centered at "center" with radius
    for(int i=0; i<CIRCLE_NODES; i++)
    {
        g2o::SE3Quat abspose = gt_poses.back() * relpose;
        addPoseVertex(abspose);
    }
}

void SE3LoopConstructor::setEdgesBtwPoses()
{
    g2o::SE3Quat edge;

    // add edge between poses
    for(size_t i=1; i<gt_poses.size(); i++)
    {
        // edge: pose[i-1] w.r.t pose[i]
        edge = gt_poses[i-1].inverse() * gt_poses[i];
        addEdgePosePose(i-1, i, edge);
    }

    // the last pose supposed to be the same as gt_poses[1]
    edge = gt_poses[1].inverse() * gt_poses.back();
    std::cout << "relpose between 0 and last:" << std::endl
              << edge.to_homogeneous_matrix() << std::endl;
    addEdgePosePose(1, int(gt_poses.size()-1), edge);
}


void SE3LoopConstructor::addPoseVertex(Eigen::Quaterniond quat, Eigen::Vector3d tran, bool set_fixed)
{
    g2o::SE3Quat pose(quat, tran);
    addPoseVertex(pose, set_fixed);
}

void SE3LoopConstructor::addPoseVertex(g2o::SE3Quat& pose, bool set_fixed)
{
    std::cout << "add pose: t=" << pose.translation().transpose()
              << " r=" << pose.rotation().coeffs().transpose() << std::endl;
    g2o::VertexSE3* v_se3 = new g2o::VertexSE3;
    v_se3->setId(getNewID());
    v_se3->setEstimate(pose);
    v_se3->setFixed(set_fixed);
    optimizer->addVertex(v_se3);
    gt_poses.push_back(pose);
}

void SE3LoopConstructor::addEdgePosePose(int id0, int id1, g2o::SE3Quat& relpose)
{
    std::cout << "add edge: id0=" << id0 << ", id1" << id1
              << ", t=" << relpose.translation().transpose()
              << ", r=" << relpose.rotation().coeffs().transpose() << std::endl;
    g2o::EdgeSE3* edge = new g2o::EdgeSE3;
    edge->setVertex(0, optimizer->vertices().find(id0)->second);
    edge->setVertex(1, optimizer->vertices().find(id1)->second);
    // !! Note: edge measurement use inverse of id0 ->
    edge->setMeasurement(relpose);
    Eigen::MatrixXd info_matrix = Eigen::MatrixXd::Identity(6,6) * 10.;
    edge->setInformation(info_matrix);
    optimizer->addEdge(edge);
}

void SE3LoopConstructor::addPointVertices()
{

}


void SE3LoopConstructor::addEdgePosePoint()
{

}
