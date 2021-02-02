#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTK_COMPOSITION_BACKEND_H
#define OMEGAWTK_COMPOSITION_BACKEND_H

namespace OmegaWTK {
namespace Composition {

class Layer;

class Backend {
protected:
    Layer *currentLayer;
public:
    void setCurrentJob(Layer *layer);
    virtual void doWork();
    Backend();
    ~Backend();
};

};
};

#endif
