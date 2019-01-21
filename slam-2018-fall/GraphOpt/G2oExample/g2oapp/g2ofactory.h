#ifndef G2OFACTORY_H
#define G2OFACTORY_H

#include <iostream>
#include "g2o/core/sparse_optimizer.h"
#include "g2o/core/block_solver.h"
#include "g2o/core/optimization_algorithm_gauss_newton.h"
#include "g2o/core/optimization_algorithm_levenberg.h"
#include "g2o/solvers/dense/linear_solver_dense.h"
#include "g2o/solvers/csparse/linear_solver_csparse.h"
#include "g2o/solvers/cholmod/linear_solver_cholmod.h"
#include "g2o/core/factory.h"
#include "g2o/types/slam3d/vertex_se3.h"
#include "g2o/types/slam3d/edge_se3.h"
#include "g2o/types/sba/types_six_dof_expmap.h"

G2O_USE_TYPE_GROUP(slam2d);
G2O_USE_TYPE_GROUP(slam3d);


enum class SolverType
{
    DENSE,
    CHOLMOD,
    CSPARSE,
};

enum class BlockType
{
    VAR,
    SE3,
    Sim3,
    SE2,
};

enum class OptimAlgorithm
{
    Levenberg,
    GaussNewton,
};


struct G2oOptions
{
    SolverType sovler_type;
    BlockType block_type;
    OptimAlgorithm algorithm;
    bool verbosity;
    Eigen::Vector3d tran_noise;
    Eigen::Vector3d quat_noise;
    Eigen::Vector3d point_noise;
    Eigen::Vector3d pixel_noise;
};

class G2oFactory
{
    typedef g2o::LinearSolverDense                  SolverDense;
    typedef g2o::LinearSolverCholmod                SolverCholmod;
    typedef g2o::LinearSolverCSparse                SolverCSparse;
    typedef g2o::BlockSolverX::PoseMatrixType       BlockVar;
    typedef g2o::BlockSolver_6_3                    BlockSE3;
    typedef g2o::BlockSolver_7_3                    BlockSim3;
    typedef g2o::BlockSolver_3_2                    BlockSE2;
    typedef g2o::OptimizationAlgorithmLevenberg     Levenberg;
    typedef g2o::OptimizationAlgorithmGaussNewton   GaussNewton;

public:
    static optimizerFactory(G2oOptions& options)
    {
        std::unique_ptr<g2o::BlockSolver> solver
                = solverFactory(options.sovler_type, options.block_type);

        g2o::OptimizationAlgorithm* algorithm;
        if(options.algorithm == OptimAlgorithm::Levenberg)
            algorithm = new Levenberg(std::move(solver));
        else if(options.algorithm == OptimAlgorithm::GaussNewton)
            algorithm = new GaussNewton(std::move(solver));

        g2o::SparseOptimizer optimizer;
        optimizer.setAlgorithm(solver);
        optimizer.setVerbose(options.verbosity);
    }

private:
    static std::unique_ptr<g2o::BlockSolver> solverFactory
                (SolverType solver_type, BlockType block_type)
    {
        std::unique_ptr<g2o::Solver> linearSolver;

        if(block_type == BlockType::VAR)
        {
            if(solver_type == SolverType::DENSE)
                linearSolver = g2o::make_unique<SolverDense<BlockVar::PoseMatrixType>>();
            else if(solver_type == SolverType::CHOLMOD)
                linearSolver = g2o::make_unique<SolverCholmod<BlockVar::PoseMatrixType>>();
            else if(solver_type == SolverType::CSPARSE)
                linearSolver = g2o::make_unique<SolverCSparse<BlockVar::PoseMatrixType>>();

            return g2o::make_unique<BlockVar>(std::move(linearSolver));
        }
        else if(block_type == BlockType::SE3)
        {
            if(solver_type == SolverType::DENSE)
                linearSolver = g2o::make_unique<SolverDense<BlockSE3::PoseMatrixType>>();
            else if(solver_type == SolverType::CHOLMOD)
                linearSolver = g2o::make_unique<SolverCholmod<BlockSE3::PoseMatrixType>>();
            else if(solver_type == SolverType::CSPARSE)
                linearSolver = g2o::make_unique<SolverCSparse<BlockSE3::PoseMatrixType>>();

            return g2o::make_unique<BlockSE3>(std::move(linearSolver));
        }
        else if(block_type == BlockType::Sim3)
        {
            if(solver_type == SolverType::DENSE)
                linearSolver = g2o::make_unique<SolverDense<BlockSim3::PoseMatrixType>>();
            else if(solver_type == SolverType::CHOLMOD)
                linearSolver = g2o::make_unique<SolverCholmod<BlockSim3::PoseMatrixType>>();
            else if(solver_type == SolverType::CSPARSE)
                linearSolver = g2o::make_unique<SolverCSparse<BlockSim3::PoseMatrixType>>();

            return g2o::make_unique<BlockSim3>(std::move(linearSolver));
        }
        else if(block_type == BlockType::SE2)
        {
            if(solver_type == SolverType::DENSE)
                linearSolver = g2o::make_unique<SolverDense<BlockSE2::PoseMatrixType>>();
            else if(solver_type == SolverType::CHOLMOD)
                linearSolver = g2o::make_unique<SolverCholmod<BlockSE2::PoseMatrixType>>();
            else if(solver_type == SolverType::CSPARSE)
                linearSolver = g2o::make_unique<SolverCSparse<BlockSE2::PoseMatrixType>>();

            return g2o::make_unique<BlockSE2>(std::move(linearSolver));
        }
    }
}

#endif // G2OFACTORY_H
