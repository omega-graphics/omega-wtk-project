#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTK_COMPOSITION_BITMAP_H
#define OMEGAWTK_COMPOSITION_BITMAP_H

namespace OmegaWTK {
    namespace Composition {
        struct BitmapImage {
            unsigned width;
            unsigned height;
            void * data;
            unsigned stride;
        };

        BitmapImage loadImageFromFile(const Core::String & name);
    };
};

#endif