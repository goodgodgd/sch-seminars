#include "se3loopconstructor.h"

SE3LoopConstructor::SE3LoopConstructor()
    : Slam3DConstructor()
{
    traj_radius = 2.;
    center = Eigen::Vector3d(1., traj_radius, 0.);
}

void SE3LoopConstructor::construct(g2o::SparseOptimizer* _optimizer, G2oConfig& _config)
{
    optimizer = _optimizer;
    config = _config;

    // add pose vertices at (0,0,0) and (1,0,0)
    createInitPoseVerts();
    // add pose vertices around circle
    createCirclePoseVerts();
    // add edges between pose edges
    setEdgesBtwPoses();
}

void SE3LoopConstructor::createInitPoseVerts()
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

void SE3LoopConstructor::createCirclePoseVerts()
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
    g2o::SE3Quat relpose;

    // add edge between poses
    for(size_t i=1; i<gt_poses.size(); i++)
    {
        // relpose: pose[i-1] w.r.t pose[i]
        relpose = gt_poses[i-1].inverse() * gt_poses[i];
        if(config.edge_noise)
            relpose = addNoisePoseMeasurement(relpose);
        addEdgePosePose(i-1, i, relpose);
    }

    // the last pose supposed to be the same as gt_poses[1]
    relpose = gt_poses[1].inverse() * gt_poses.back();
    std::cout << "relpose between 0 and last:" << std::endl
              << relpose.to_homogeneous_matrix() << std::endl;
    if(config.edge_noise)
        relpose = addNoisePoseMeasurement(relpose);
    addEdgePosePose(1, int(gt_poses.size()-1), relpose);
}
