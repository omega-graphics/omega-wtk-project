#include "omegaWTK/Core/Core.h"
#include "Layer.h"

#ifndef OMEGAWTK_COMPOSITION_BACKEND_H
#define OMEGAWTK_COMPOSITION_BACKEND_H

namespace OmegaWTK {
namespace Composition {

class Layer;

class OMEGAWTK_EXPORT Backend {
protected:
//    std::mutex mutex_lock;
//    std::condition_variable occupied_condition;
//    bool isAvailable = true;
    friend class Compositor;
protected:
    LayerTree::Limb *currentLimb;
public:
    void setCurrentJob(LayerTree::Limb *limb);
    virtual void doWork() = 0;
    virtual void doUpdate() = 0;
    virtual void redoLayout() = 0;
    Backend();
    ~Backend();
};

Core::UniquePtr<Backend> make_backend();

};
};

#endif
