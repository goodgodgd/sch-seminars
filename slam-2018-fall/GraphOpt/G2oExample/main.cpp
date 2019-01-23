#include <iostream>
#include "g2oapp/g2ofactory.h"
#include "g2oapp/examplefactory.h"

int main()
{
    // TODO
    // 1. add noise in graph constructor
    // 2. rename G2oFactory -> OptimizerFactory, optimizerFactory -> factory()
    // 3. rename graphFactory() -> factory()
    // 4. make "simple" example within main.cpp

    G2oConfig options;
    options.sovler_type = SolverType::CHOLMOD;
    options.block_type = BlockType::SE3;
    options.algorithm = AlgorithmType::Levenberg;
    options.example = GraphExample::SE3Loop;
    options.verbosity = true;
    options.init_vtx = false;
    options.edge_noise = true;
    options.tran_noise = Eigen::Vector3d(0.01, 0.01, 0.01);
    options.quat_noise = Eigen::Vector4d(0.01, 0.01, 0.01, 0.01);
    options.point_noise = Eigen::Vector3d(0.01, 0.01, 0.01);
    options.pixel_noise = Eigen::Vector2d(0.1, 0.1);

    g2o::SparseOptimizer* optimizer = G2oFactory::optimizerFactory(options);
    GraphConstructor* graph_constr = ExampleFactory::graphFactory(options);

    graph_constr->construct(optimizer, options);
    optimizer->initializeOptimization();

    std::string filename;
    filename = std::string(PRJ_PATH) + "/../igdata/before_opt.g2o";
    optimizer->save(filename.c_str());

    optimizer->optimize(100);

    filename = std::string(PRJ_PATH) + "/../igdata/after_opt.g2o";
    optimizer->save(filename.c_str());
}
