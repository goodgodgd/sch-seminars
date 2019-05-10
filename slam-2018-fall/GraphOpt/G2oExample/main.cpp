#include <iostream>
#include "g2oapp/g2ofactory.h"

int main()
{
    G2oConfig options;
    options.sovler_type = SolverType::CHOLMOD;
    options.block_type = BlockType::Var;
    options.algorithm = AlgorithmType::Levenberg;
    options.example = GraphExample::SE3Point;
    options.verbosity = false;
    options.init_vtx = false;
    options.edge_noise = true;
    options.tran_noise = Eigen::Vector3d(0.01, 0.01, 0.01);
    options.quat_noise = Eigen::Vector4d(0.01, 0.01, 0.01, 0.01);
    options.point_noise = Eigen::Vector3d(0.01, 0.01, 0.01);
    options.pixel_noise = Eigen::Vector2d(0.1, 0.1);

    g2o::SparseOptimizer* optimizer = G2oFactory::optimizerFactory(options);
    GraphConstructor* graph_constr = G2oFactory::graphFactory(options);

    graph_constr->construct(optimizer, options);
    optimizer->initializeOptimization();

    std::string filename;
    filename = std::string(PRJ_PATH) + "/../igdata/before_opt.g2o";
    optimizer->save(filename.c_str());

    optimizer->optimize(100);

    filename = std::string(PRJ_PATH) + "/../igdata/after_opt.g2o";
    optimizer->save(filename.c_str());
}
