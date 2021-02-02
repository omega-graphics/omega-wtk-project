#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTK_COMPOSITION_BACKEND_H
#define OMEGAWTK_COMPOSITION_BACKEND_H

namespace OmegaWTK {
namespace Composition {

class Layer;

class OMEGAWTK_EXPORT Backend {
protected:
    Layer *currentLayer;
public:
    void setCurrentJob(Layer *layer);
    virtual void doWork() = 0;
    Backend();
    ~Backend();
};

Backend * make_backend();

};
};

#endif
