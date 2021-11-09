#include "omegaWTK/UI/AppWindow.h"
#include "omegaWTK/UI/Widget.h"
#include "omegaWTK/Native/NativeWindow.h"
#include "omegaWTK/Native/NativeDialog.h"
#include "omegaWTK/Composition/CompositorClient.h"
#include "omegaWTK/UI/WidgetTreeHost.h"

namespace OmegaWTK {

    AppWindow::AppWindow(Core::Rect rect,AppWindowDelegate *delegate):
    layer(std::make_unique<Composition::WindowLayer>(rect,Native::make_native_window(rect,this))),
    rootViewRenderTarget(new Composition::ViewRenderTarget(layer->native_window_ptr->getRootView())),
    proxy(rootViewRenderTarget),
    delegate(delegate),
    rect(rect){
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

void AppWindow::setTitle(OmegaCommon::StrRef title){
    layer->native_window_ptr->setTitle(title);
}

void AppWindow::setEnableWindowHeader(bool enable) {
    layer->native_window_ptr->setEnableWindowHeader(enable);
}

// void AppWindow::setLayerStyle(SharedHandle<Composition::WindowStyle> & style){
//     layer->setWindowStyle(style);
// };

// void AppWindow::setMenuStyle(SharedHandle<Composition::MenuStyle> & style){
//     menuStyle = style;
// };

void AppWindow::_add_widget(Widget * handle){
//    (*handle)->compositor->prepareDraw((*handle)->layerTree.get());
    if(handle->rootView->renderTarget) {
        layer->native_window_ptr->addNativeItem(handle->rootView->renderTarget->getNativePtr());
    }
};

void AppWindow::add(Widget *widget){
    auto treeHost = WidgetTreeHost::Create();
    treeHost->setRoot(widget);
    treeHost->attachToWindow(this);
};


SharedHandle<Native::NativeFSDialog> AppWindow::openFSDialog(const Native::NativeFSDialog::Descriptor & desc){
    return Native::NativeFSDialog::Create(desc,layer->native_window_ptr);
};

SharedHandle<Native::NativeNoteDialog> AppWindow::openNoteDialog(const Native::NativeNoteDialog::Descriptor & desc){
    return Native::NativeNoteDialog::Create(desc,layer->native_window_ptr);
};

void AppWindow::close(){
    layer->native_window_ptr->close();
};

AppWindow::~AppWindow(){
    std::cout << "Closing Window" << std::endl;
    close();
    for(auto widgetHost : widgetTreeHosts){
        delete widgetHost;
    };
};

AppWindowManager::AppWindowManager():rootWindow(nullptr){};

void AppWindowManager::setRootWindow(AppWindow * handle){
    rootWindow = handle;
};

AppWindow * AppWindowManager::getRootWindow(){
    return rootWindow;
};

void AppWindowManager::displayRootWindow(){
    rootWindow->layer->native_window_ptr->initialDisplay();
};

void AppWindowManager::closeAllWindows(){
    rootWindow->close();
};

void AppWindowDelegate::onRecieveEvent(Native::NativeEventPtr event){
    switch (event->type) {
        case Native::NativeEvent::WindowWillClose: {
            windowWillClose(event);
            break;
        }
        case Native::NativeEvent::WindowWillResize : {
            Native::WindowWillResize *params = (Native::WindowWillResize *)event->params;
            // MessageBoxA(HWND_DESKTOP,"Window Will Resize","NOTE",MB_OK);
            windowWillResize(params->rect);
            // MessageBoxA(HWND_DESKTOP,"Window Has Resized","NOTE",MB_OK);
            window->rect = params->rect;
            break;
        }
        default:
            break;
    }
};

void AppWindowDelegate::windowWillClose(Native::NativeEventPtr event){
    /// To Be Overrided by its sub-classes!
};

void AppWindowDelegate::windowWillResize(Core::Rect & nRect){
    /// To Be Overrided by its sub-classes!
};

    


};
