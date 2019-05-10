#ifndef G2OFACTORY_H
#define G2OFACTORY_H

#include <iostream>
#include "g2o/core/block_solver.h"
// solver options
#include "g2o/solvers/dense/linear_solver_dense.h"
#include "g2o/solvers/csparse/linear_solver_csparse.h"
#include "g2o/solvers/cholmod/linear_solver_cholmod.h"
// optmization options
#include "g2o/core/optimization_algorithm_gauss_newton.h"
#include "g2o/core/optimization_algorithm_levenberg.h"

//#include "g2o/core/factory.h"
//G2O_USE_TYPE_GROUP(slam2d);
//G2O_USE_TYPE_GROUP(slam3d);

#include "g2oconfig.h"
#include "se3loopconstructor.h"
#include "se3pointconstructor.h".h"

class G2oFactory
{
    typedef g2o::BlockSolverX           BS_Var;
    typedef g2o::BlockSolver_6_3        BS_SE3;
    typedef g2o::BlockSolver_7_3        BS_Sim3;
    typedef g2o::BlockSolver_3_2        BS_SE2;

    typedef g2o::OptimizationAlgorithmLevenberg     Levenberg;
    typedef g2o::OptimizationAlgorithmGaussNewton   GaussNewton;

public:
    static GraphConstructor* graphFactory(G2oConfig& config)
    {
        if(config.example == GraphExample::SE3Only)
            return new SE3LoopConstructor;
        else if(config.example == GraphExample::SE3Point)
            return new Se3PointConstructor;
        else
            nullptr;
    }

    static g2o::SparseOptimizer* optimizerFactory(G2oConfig& options)
    {
        g2o::OptimizationAlgorithm* algorithm;
        if(options.block_type == BlockType::Var)
        {
            auto block_solver = solverFactory<BS_Var,
                                              BS_Var::LinearSolverType,
                                              BS_Var::PoseMatrixType>
                                              (options.sovler_type);
            algorithm = algorithmFactory(options.algorithm,
                                         std::move(block_solver));
        }
        else if(options.block_type == BlockType::SE3)
        {
            auto block_solver = solverFactory<BS_SE3,
                                              BS_SE3::LinearSolverType,
                                              BS_SE3::PoseMatrixType>
                                              (options.sovler_type);
            algorithm = algorithmFactory(options.algorithm,
                                         std::move(block_solver));
        }
        else if(options.block_type == BlockType::Sim3)
        {
            auto block_solver = solverFactory<BS_Sim3,
                                              BS_Sim3::LinearSolverType,
                                              BS_Sim3::PoseMatrixType>
                                              (options.sovler_type);
            algorithm = algorithmFactory(options.algorithm,
                                         std::move(block_solver));
        }
        else if(options.block_type == BlockType::SE2)
        {
            auto block_solver = solverFactory<BS_SE2,
                                              BS_SE2::LinearSolverType,
                                              BS_SE2::PoseMatrixType>
                                              (options.sovler_type);
            algorithm = algorithmFactory(options.algorithm,
                                         std::move(block_solver));
        }

        g2o::SparseOptimizer* optimizer = new g2o::SparseOptimizer;
        optimizer->setAlgorithm(algorithm);
        optimizer->setVerbose(options.verbosity);
        return optimizer;
    }

private:
    template <typename BSType, typename LSType, typename PMType>
    static std::unique_ptr<BSType> solverFactory(SolverType solver_type)
    {
        std::unique_ptr<LSType> linear_solver;
        if(solver_type == SolverType::DENSE)
            linear_solver = g2o::make_unique<g2o::LinearSolverDense<PMType>>();
        else if(solver_type == SolverType::CHOLMOD)
            linear_solver = g2o::make_unique<g2o::LinearSolverCholmod<PMType>>();
        else if(solver_type == SolverType::CSPARSE)
            linear_solver = g2o::make_unique<g2o::LinearSolverCSparse<PMType>>();

        return g2o::make_unique<BSType>(std::move(linear_solver));
    }

    template <typename BSType>
    static g2o::OptimizationAlgorithm* algorithmFactory
        (AlgorithmType algo_type, BSType block_solver)
    {
        if(algo_type == AlgorithmType::Levenberg)
            return new Levenberg(std::move(block_solver));
        else if(algo_type == AlgorithmType::GaussNewton)
            return new GaussNewton(std::move(block_solver));
        else
            return nullptr;
    }

};

#endif // G2OFACTORY_H
