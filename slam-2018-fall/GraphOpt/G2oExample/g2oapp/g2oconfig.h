#ifndef G2OCONFIG_H
#define G2OCONFIG_H

#include <eigen3/Eigen/Eigen>

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

enum class AlgorithmType
{
    Levenberg,
    GaussNewton,
};

enum class GraphExample
{
    SE3Loop,
};

struct G2oConfig
{
    SolverType sovler_type;
    BlockType block_type;
    AlgorithmType algorithm;
    GraphExample example;
    bool verbosity;
    Eigen::Vector3d tran_noise;
    Eigen::Vector3d quat_noise;
    Eigen::Vector3d point_noise;
    Eigen::Vector2d pixel_noise;
};

#endif // G2OCONFIG_H
