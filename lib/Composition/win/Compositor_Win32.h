#include "omegaWTK/Composition/Compositor.h"
#include "NativePrivate/win/HWNDItem.h"
#include "DCVisualTree.h"

#include <wrl.h>
#include <dwrite.h>
#include <d2d1_1.h>
#include <dcomp.h>
#include <dcompanimation.h>

#ifndef OMEGAWTK_COMPOSITION_PRIVATE_INCLUDE_COMPOSITOR_WIN32_H
#define OMEGAWTK_COMPOSITION_PRIVATE_INCLUDE_COMPOSITOR_WIN32_H

namespace OmegaWTK::Composition {



    class WinCanvas : public Canvas {
        DCVisualTree tree;
        Native::Win::HWNDItem * native_item;
        Native::NativeItemPtr getNativeItem();
    };
    class WinCompositor : public Compositor {

        WinCanvas * current_canvas;
        void setCanvas(Canvas *canvas);
    public:
        WinCompositor(){
            
        };
        ~WinCompositor();
    };
};

#endif
