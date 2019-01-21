#ifndef EXAMPLEFACTORY_H
#define EXAMPLEFACTORY_H

#include "g2oconfig.h"
#include "se3loopconstructor.h"

class ExampleFactory
{
public:
    static GraphConstructor* graphFactory(G2oConfig& config)
    {
        if(config.example == GraphExample::SE3Loop)
            return new SE3LoopConstructor;
        else
            nullptr;
    }
};

#endif // EXAMPLEFACTORY_H
