#include "Compositor_Win32.h"

namespace OmegaWTK::Composition {
     Native::NativeItemPtr WinCanvas::getNativeItem(){
         return native_item;
     };
     
     void WinCompositor::setCanvas(Canvas * canvas){
         current_canvas = (WinCanvas *)canvas;
     };
};