#include "omegaWTK/Composition/Compositor.h"
#include "NativePrivate/macos/CocoaItem.h"

#ifndef OMEGAWTK_COMPOSITION_MAC_COMPOSITOR_MACOS_H
#define OMEGAWTK_COMPOSITION_MAC_COMPOSITOR_MACOS_H



namespace OmegaWTK {

namespace Composition {

class CocoaCanvas : public Canvas {
    Native::Cocoa::CocoaItem * native_item;
    Native::NativeItemPtr getNativeItem();
};

class CocoaCompositor : public Compositor {
    CocoaCanvas * current_canvas;
    void setCanvas(Canvas * canvas);
public:
    CocoaCompositor();
    ~CocoaCompositor();
};

};
};
#endif
