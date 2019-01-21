#include <iostream>
#include "ba3dconstructor.h"
#include "g2oapp/g2ofactory.h"
#include "g2oapp/se3loopconstructor.h"

GraphConstructor* graphConstructorFactory(G2oOptions& options)
{
//    if(options.)
}


int main()
{
    G2oOptions options;
    options.sovler_type = SolverType::CHOLMOD;
    options.block_type = BlockType::SE3;
    options.algorithm = AlgorithmType::Levenberg;
    options.verbosity = true;
    options.tran_noise = Eigen::Vector3d(0.01, 0.01, 0.01);
    options.quat_noise = Eigen::Vector3d(0.01, 0.01, 0.01);
    options.point_noise = Eigen::Vector3d(0.01, 0.01, 0.01);
    options.pixel_noise = Eigen::Vector2d(0.1, 0.1);

    g2o::SparseOptimizer* optimizer = G2oFactory::optimizerFactory(options);
//    GraphConstructor* graph_constr = graphConstructorFactory(options);
//    graph_constr->construct(graph_constr);

    BA3dConstructor* ba = new BA3dConstructor;
    ba->setOptimizer(optimizer);
    ba->construct();

    optimizer->initializeOptimization();

    std::string filename;
    filename = std::string(PRJ_PATH) + "/../igdata/before_opt.g2o";
    optimizer->save(filename.c_str());

    optimizer->optimize(10);

    filename = std::string(PRJ_PATH) + "/../igdata/after_opt.g2o";
    optimizer->save(filename.c_str());
}
