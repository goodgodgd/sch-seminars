#include <iostream>
#include "ba3dconstructor.h"

int main()
{
//    g2oOptions options = readOptions(filename);
//    g2o::SparseOptimizer* optimizer = Factory::optimizerFactory(options);
//    GraphConstructor* graph_csrt = Factory::graphBuilderFactory(options);
//    graph_csrt.construct(optimizer);
//    optimize_and_write_result(optimizer);
    //------------------------------------------------------------

    std::unique_ptr<g2o::BlockSolver_6_3::LinearSolverType> linearSolver;
    linearSolver = g2o::make_unique<g2o::LinearSolverCholmod<g2o::BlockSolver_6_3::PoseMatrixType>>();

    g2o::OptimizationAlgorithmLevenberg* solver
            = new g2o::OptimizationAlgorithmLevenberg(
                g2o::make_unique<g2o::BlockSolver_6_3>(std::move(linearSolver)));

    g2o::SparseOptimizer* optimizer = new g2o::SparseOptimizer;
    optimizer->setAlgorithm(solver);
    optimizer->setVerbose(true);

    BA3dConstructor* ba = new BA3dConstructor;
    ba->setOptimizer(optimizer);
    ba->construct();

    optimizer->initializeOptimization();

    std::string filename;
    filename = std::string(PRJ_PATH) + "/../before_opt.g2o";
    optimizer->save(filename.c_str());

    optimizer->optimize(10);

    filename = std::string(PRJ_PATH) + "/../after_opt.g2o";
    optimizer->save(filename.c_str());
}
