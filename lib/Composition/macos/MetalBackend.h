#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Core/ImgCodec.h"
#include "omegaWTK/Composition/Backend.h"
#import <Metal/Metal.h>
#import <QuartzCore/QuartzCore.h>

#ifndef OMEGAWTK_COMPOSITION_MACOS_METALBACKEND_H
#define OMEGAWTK_COMPOSITION_MACOS_METALBACKEND_H


namespace OmegaWTK::Composition {
namespace Cocoa {
class MetalBackend {
    
};

CAMetalLayer *drawBitmapToMetalLayer(Core::BitmapImage img);

}
}

#endif
