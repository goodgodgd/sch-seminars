#ifndef EXAMPLECONSTRUCTOR_H
#define EXAMPLECONSTRUCTOR_H

#include <iostream>
#include "g2o/core/sparse_optimizer.h"
#include "g2o/core/block_solver.h"
#include "g2o/core/optimization_algorithm_gauss_newton.h"
#include "g2o/core/optimization_algorithm_levenberg.h"
#include "g2o/solvers/csparse/linear_solver_csparse.h"
#include "g2o/solvers/cholmod/linear_solver_cholmod.h"
#include "g2o/core/factory.h"
#include "g2o/types/slam3d/vertex_se3.h"
#include "g2o/types/slam3d/edge_se3.h"
#include "g2o/types/sba/types_six_dof_expmap.h"

//G2O_USE_TYPE_GROUP(slam2d);
//G2O_USE_TYPE_GROUP(slam3d);


class ExampleConstructor
{
public:
    ExampleConstructor() { vertex_id = 0; }
    virtual ~ExampleConstructor() {}
    void setOptimizer(g2o::SparseOptimizer* optin) { optimizer = optin; }
    g2o::SparseOptimizer* getOptimizer() { return optimizer; }
    virtual void construct()=0;

protected:
    int getNewID() { return vertex_id++; }

    g2o::SparseOptimizer* optimizer;
    int vertex_id;
};

#endif // EXAMPLECONSTRUCTOR_H
