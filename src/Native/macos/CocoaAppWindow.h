#include "omegaWTK/Native/NativeWindow.h"
#import <Cocoa/Cocoa.h>
#include "NativePrivate/macos/CocoaItem.h"


#ifndef OMEGAWTK_NATIVE_COCOA_COCOAAPPWINDOW_H
#define OMEGAWTK_NATIVE_COCOA_COCOAAPPWINDOW_H
@class OmegaWTKNativeCocoaAppWindowDelegate;
@class OmegaWTKNativeCocoaAppWindowController;

namespace OmegaWTK::Native::Cocoa {
class CocoaAppWindow : public NativeWindow {
    SharedHandle<CocoaItem> rootView;
    OmegaWTKNativeCocoaAppWindowController *windowController;
    OmegaWTKNativeCocoaAppWindowDelegate *windowDelegate;
public:
    NativeEventEmitter *getEmitter();
    NativeItemPtr getRootView() override;
    void disable() override;
    void enable() override;
    void initialDisplay() override;
    void close() override;
    void addNativeItem(NativeItemPtr item) override;
    __strong NSWindow *getWindow();
    CocoaAppWindow(Core::Rect & rect,NativeEventEmitter *emitter);
    // ~CocoaAppWindow();
};
};

#endif
