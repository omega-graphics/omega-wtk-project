#include "omegaWTK/UI/AppWindow.h"
#include "omegaWTK/Native/NativeWindow.h"
#include "omegaWTK/Native/NativeDialog.h"

namespace OmegaWTK {
    namespace Composition {
        WindowLayer::WindowLayer(Core::Rect & rect,Native::NWH native_window_ptr):native_window_ptr(native_window_ptr),rect(rect){};
    }

    AppWindow::AppWindow(Core::Rect rect,AppWindowDelegate *delegate):layer(std::make_unique<Composition::WindowLayer>(rect,Native::make_native_window(rect))),delegate(delegate),rect(rect){
        delegate->window = this;
    };

    UniqueHandle<Native::NativeFSDialog> AppWindow::openFSDialog(){
        return nullptr;
    };

    UniqueHandle<Native::NativeNoteDialog> AppWindow::openNoteDialog(){

    };
    
    AppWindowDelegate::AppWindowDelegate(){
        
    };


};