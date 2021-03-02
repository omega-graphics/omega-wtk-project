#include "omegaWTK/UI/AppWindow.h"
#include "omegaWTK/UI/Widget.h"
#include "omegaWTK/Native/NativeWindow.h"
#include "omegaWTK/Native/NativeDialog.h"

namespace OmegaWTK {
    namespace Composition {
        WindowLayer::WindowLayer(Core::Rect & rect,Native::NWH native_window_ptr):native_window_ptr(native_window_ptr),rect(rect){
            // MessageBoxA(HWND_DESKTOP,"Creating Window Layer!","NOTE",MB_OK);
            native_window_ptr->setNativeLayer(this);
        };
        void WindowLayer::redraw(){
            
        };
    }

    AppWindow::AppWindow(Core::Rect rect,AppWindowDelegate *delegate):layer(std::make_unique<Composition::WindowLayer>(rect,Native::make_native_window(rect,this))),delegate(delegate),rect(rect){
        // MessageBoxA(HWND_DESKTOP,"Create Window Layer!","NOTE",MB_OK);
        if(delegate) {
            setReciever(delegate);
            delegate->window = this;
        }
    };

void AppWindow::setMenu(SharedHandle<Menu> & menu){
    this->menu = menu;
    layer->native_window_ptr->setMenu(this->menu->getNativeMenu());
};

void AppWindow::_add_widget(SharedHandle<Widget> * handle){
    (*handle)->compositor->prepareDraw((*handle)->rootView->getLayer());
    rootWidgets.push_back(*handle);
    layer->native_window_ptr->addNativeItem((*handle)->rootView->getLayer()->getTargetNativePtr());
};

    UniqueHandle<Native::NativeFSDialog> AppWindow::openFSDialog(){
        return nullptr;
    };

    UniqueHandle<Native::NativeNoteDialog> AppWindow::openNoteDialog(){
        return nullptr;
    };

AppWindowManager::AppWindowManager():rootWindow(nullptr){};

void AppWindowManager::setRootWindow(SharedHandle<AppWindow> & handle){
    rootWindow = handle;
};

void AppWindowManager::displayRootWindow(){
    rootWindow->layer->native_window_ptr->attachWidgets();
    rootWindow->layer->native_window_ptr->initialDisplay();
};

void AppWindowDelegate::onRecieveEvent(Native::NativeEventPtr event){
    switch (event->type) {
        case Native::NativeEvent::WindowWillClose: {
            windowWillClose(event);
            break;
        }
        default:
            break;
    }
};

void AppWindowDelegate::windowWillClose(Native::NativeEventPtr event){
    /// To Be Overrided by its sub-classes!
};

    


};
