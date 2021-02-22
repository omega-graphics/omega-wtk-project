#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTK_COMPOSITION_MTL_MTLBACKEND_H
#define OMEGAWTK_COMPOSITION_MTL_MTLBACKEND_H 

namespace OmegaWTK::Composition {
class BDCompositionDevice;
Core::SharedPtr<BDCompositionDevice> CreateMTLBDCompositonDevice();
};

#endif
