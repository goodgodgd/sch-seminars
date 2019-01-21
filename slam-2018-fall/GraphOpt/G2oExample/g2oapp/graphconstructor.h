#ifndef GRAPHCONSTRUCTOR_H
#define GRAPHCONSTRUCTOR_H

#include <iostream>
#include "g2oall.h"
#include "g2oconfig.h"

class GraphConstructor
{
public:
    GraphConstructor() : optimizer(nullptr), vertex_id(0) {}
    virtual ~GraphConstructor() {}
    virtual void construct(g2o::SparseOptimizer* optim, G2oConfig& config)=0;

protected:
    int getNewID() { return vertex_id++; }

    g2o::SparseOptimizer* optimizer;
    int vertex_id;
};

#endif // GRAPHCONSTRUCTOR_H
