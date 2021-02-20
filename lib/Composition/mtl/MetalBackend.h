#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Core/ImgCodec.h"
#include "omegaWTK/Composition/Backend.h"
#import <Metal/Metal.h>
#import <QuartzCore/QuartzCore.h>

#ifndef OMEGAWTK_COMPOSITION_MACOS_METALBACKEND_H
#define OMEGAWTK_COMPOSITION_MACOS_METALBACKEND_H



namespace OmegaWTK::Composition {

void *make_metal_backend();

CAMetalLayer *drawBitmapToMetalLayer(Core::SharedPtr<Core::BitmapImage> & img,Core::Rect & rect,void *metal_backend,unsigned v_id);

}

#endif
