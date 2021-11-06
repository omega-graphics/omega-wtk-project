#include "omegaWTK/Native/NativeWindow.h"


namespace OmegaWTK::Native {
    #ifdef TARGET_MACOS
    NativeWindow::NativeWindow(Core::Rect & rect):rect(rect){};
    #endif
    #ifdef TARGET_MACOS
    bool NativeWindow::hasEventEmitter() {
        return eventEmitter != nullptr;
    };
    #endif
//    void NativeWindow::setMenu(NM menu){
//        this->menu = menu;
//    };
    // void NativeWindow::addNativeItem(NativeItemPtr item){
    //     windowWidgetRootViews.push_back(item);
    // };
};
