#include <iostream>

#include "g2o/core/sparse_optimizer.h"
#include "g2o/core/block_solver.h"
#include "g2o/solvers/dense/linear_solver_dense.h"
 #include "g2o/solvers/csparse/linear_solver_csparse.h"
 #include "g2o/solvers/cholmod/linear_solver_cholmod.h"
#include "g2o/core/optimization_algorithm_levenberg.h"
 #include "g2o/core/optimization_algorithm_gauss_newton.h"
#include "g2o/core/factory.h"
#include "g2o/types/slam3d/vertex_se3.h"
#include "g2o/types/slam3d/edge_se3.h"
// #include "g2o/types/sba/types_six_dof_expmap.h"

// G2O_USE_TYPE_GROUP(slam2d);
// G2O_USE_TYPE_GROUP(slam3d);


int getNewID()
{
    static int vertex_id = 0;
    return vertex_id++;
}

void addPoseVertex(g2o::SparseOptimizer* optimizer, g2o::SE3Quat& pose, bool set_fixed)
{
    std::cout << "add pose: t=" << pose.translation().transpose()
              << " r=" << pose.rotation().coeffs().transpose() << std::endl;
    g2o::VertexSE3* v_se3 = new g2o::VertexSE3;
    v_se3->setId(getNewID());
    if(set_fixed)
        v_se3->setEstimate(pose);
    v_se3->setFixed(set_fixed);
    optimizer->addVertex(v_se3);
}

void addEdgePosePose(g2o::SparseOptimizer* optimizer, int id0, int id1, g2o::SE3Quat& relpose)
{
    std::cout << "add edge: id0=" << id0 << ", id1" << id1
              << ", t=" << relpose.translation().transpose()
              << ", r=" << relpose.rotation().coeffs().transpose() << std::endl;

    g2o::EdgeSE3* edge = new g2o::EdgeSE3;
    edge->setVertex(0, optimizer->vertices().find(id0)->second);
    edge->setVertex(1, optimizer->vertices().find(id1)->second);
    edge->setMeasurement(relpose);
    Eigen::MatrixXd info_matrix = Eigen::MatrixXd::Identity(6,6) * 10.;
    edge->setInformation(info_matrix);
    optimizer->addEdge(edge);
}


int main()
{
    std::unique_ptr<g2o::BlockSolverX::LinearSolverType> linear_solver
            = g2o::make_unique<g2o::LinearSolverDense<g2o::BlockSolverX::PoseMatrixType>>();
    std::unique_ptr<g2o::BlockSolverX> block_solver
            = g2o::make_unique<g2o::BlockSolverX>(std::move(linear_solver));
    g2o::OptimizationAlgorithm* algorithm
            = new g2o::OptimizationAlgorithmLevenberg(std::move(block_solver));
    g2o::SparseOptimizer* optimizer = new g2o::SparseOptimizer;
    optimizer->setAlgorithm(algorithm);
    optimizer->setVerbose(true);

    std::vector<g2o::SE3Quat> gt_poses;


    // initialize fixed vertices
    {
        Eigen::Vector3d tran;
        Eigen::Quaterniond quat;

        // first vertex at origin
        tran = Eigen::Vector3d(0,0,0);
        quat.setIdentity();
        g2o::SE3Quat pose0(quat, tran);
        addPoseVertex(optimizer, pose0, true);
        gt_poses.push_back(pose0);

        // second vertex at 1,0,0
        tran = Eigen::Vector3d(1,0,0);
        quat.setIdentity();
        g2o::SE3Quat pose1(quat, tran);
        addPoseVertex(optimizer, pose1, true);
        gt_poses.push_back(pose1);
    }

    // set variable vertices
    {
        const double PI = 3.141592653589793;
        const int CIRCLE_NODES = 8;
        const double CIRCLE_RADIUS = 2;
        double angle = 2.*PI/double(CIRCLE_NODES);
        Eigen::Quaterniond quat;
        quat = Eigen::AngleAxisd(angle, Eigen::Vector3d(0,0,1));
        Eigen::Vector3d tran = Eigen::Vector3d(CIRCLE_RADIUS*sin(angle),
                                CIRCLE_RADIUS - CIRCLE_RADIUS*cos(angle), 0.);
        g2o::SE3Quat relpose(quat, tran);

        // add vertices around a circle centered at "center" with radius
        for(int i=0; i<CIRCLE_NODES; i++)
        {
            g2o::SE3Quat abspose = gt_poses.back() * relpose;
            addPoseVertex(optimizer, abspose, false);
            gt_poses.push_back(abspose);
        }
    }

    // set edges between poses
    {
        g2o::SE3Quat relpose;

        // add edge between poses
        for(size_t i=1; i<gt_poses.size(); i++)
        {
            // relpose: pose[i-1] w.r.t pose[i]
            relpose = gt_poses[i-1].inverse() * gt_poses[i];
            addEdgePosePose(optimizer, i-1, i, relpose);
        }

        // the last pose supposed to be the same as gt_poses[1]
        relpose = gt_poses[1].inverse() * gt_poses.back();
        addEdgePosePose(optimizer, 1, int(gt_poses.size()-1), relpose);
    }

    // output graph files before and after optimization
    {
        optimizer->initializeOptimization();

        std::string filename;
        filename = std::string(PRJ_PATH) + "/../igdata/before_opt.g2o";
        optimizer->save(filename.c_str());

        optimizer->optimize(100);

        filename = std::string(PRJ_PATH) + "/../igdata/after_opt.g2o";
        optimizer->save(filename.c_str());
    }

    return 0;
}
