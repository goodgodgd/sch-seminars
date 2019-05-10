#ifndef GRAPHCONSTRUCTOR_H
#define GRAPHCONSTRUCTOR_H

#include <iostream>
#include <assert.h>
#include "g2o/core/sparse_optimizer.h"

#include "g2oconfig.h"

class GraphConstructor
{
public:
    GraphConstructor() : optimizer(nullptr), vertex_id(0) {}
    virtual ~GraphConstructor() {}
    virtual void construct(g2o::SparseOptimizer* _optimizer, G2oConfig& _config)=0;

protected:
    int getNewID() { return vertex_id++; }

    g2o::SparseOptimizer* optimizer;
    G2oConfig config;
    int vertex_id;
};

#endif // GRAPHCONSTRUCTOR_H
