#include "omegaWTK/UI/AppWindow.h"
#include "omegaWTK/UI/Widget.h"
#include "omegaWTK/Native/NativeWindow.h"
#include "omegaWTK/Native/NativeDialog.h"
#include "omegaWTK/Composition/Compositor.h"
#include "omegaWTK/Composition/ViewRenderTarget.h"

namespace OmegaWTK {

    AppWindow::AppWindow(Core::Rect rect,AppWindowDelegate *delegate):layer(std::make_unique<Composition::WindowLayer>(rect,Native::make_native_window(rect,this))),compositor(new Composition::Compositor()),delegate(delegate),rect(rect){
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

void AppWindow::setLayerStyle(SharedHandle<Composition::WindowStyle> & style){
    layer->setWindowStyle(style);
};

void AppWindow::setMenuStyle(SharedHandle<Composition::MenuStyle> & style){
    menuStyle = style;
};

void AppWindow::_add_widget(SharedHandle<Widget> * handle){
//    (*handle)->compositor->prepareDraw((*handle)->layerTree.get());
    rootWidgets.push_back(*handle);
    layer->native_window_ptr->addNativeItem((*handle)->rootView->renderTarget->getNativePtr());
};

UniqueHandle<Native::NativeFSDialog> AppWindow::openFSDialog(){
    return nullptr;
};

UniqueHandle<Native::NativeNoteDialog> AppWindow::openNoteDialog(){
    return nullptr;
};

void AppWindow::drawWidgets(){
    auto widget_it = rootWidgets.begin();
    while(widget_it != rootWidgets.end()){
        (*widget_it)->compositor->prepareDraw(widget_it->get()->layerTree.get());
        ++widget_it;
    };
};

AppWindowManager::AppWindowManager():rootWindow(nullptr){};

void AppWindowManager::setRootWindow(SharedHandle<AppWindow> & handle){
    rootWindow = handle;
};

void AppWindowManager::displayRootWindow(){
    rootWindow->layer->native_window_ptr->attachWidgets();
    rootWindow->drawWidgets();
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
