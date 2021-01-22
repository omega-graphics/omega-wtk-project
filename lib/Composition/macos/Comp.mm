#include "Compositor_macOS.h"

namespace OmegaWTK::Composition {

Native::NativeItemPtr CocoaCanvas::getNativeItem(){
    return native_item;
};

void CocoaCompositor::setCanvas(Canvas * canvas){
    current_canvas = canvas;
};

