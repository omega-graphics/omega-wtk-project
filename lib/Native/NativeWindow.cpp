#include "omegaWTK/Native/NativeWindow.h"

#ifdef TARGET_WIN32
#include "win/MakeItem.h"
#endif

#ifdef TARGET_MACOS
#include "macos/MakeItem.h"
#endif

namespace OmegaWTK::Native {
    #ifdef TARGET_MACOS
    NativeWindow::NativeWindow(Core::Rect & rect):rect(rect){};
    #endif
    bool NativeWindow::hasEventEmitter() {
        return event_emitter != nullptr;
    };
    void NativeWindow::setMenu(NM menu){
        this->menu = menu;
    };
    void NativeWindow::addNativeItem(NativeItemPtr item){
        windowWidgetRootViews.push_back(item);
    };
    NWH make_native_window(Core::Rect & rect,NativeEventEmitter *emitter){
        #ifdef TARGET_WIN32
            return Win::make_win_window(rect,emitter);
        #endif

        #ifdef TARGET_MACOS
            return Cocoa::make_cocoa_window(rect,emitter);
        #endif
    };
};
