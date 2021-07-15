#include "omegaWTK/Native/NativeWindow.h"
#import <Cocoa/Cocoa.h>


#ifndef OMEGAWTK_NATIVE_COCOA_COCOAAPPWINDOW_H
#define OMEGAWTK_NATIVE_COCOA_COCOAAPPWINDOW_H
@class OmegaWTKNativeCocoaAppWindowDelegate;
@class OmegaWTKNativeCocoaAppWindowController;

namespace OmegaWTK::Native::Cocoa {
class CocoaAppWindow : public NativeWindow {
    OmegaWTKNativeCocoaAppWindowController *windowController;
    OmegaWTKNativeCocoaAppWindowDelegate *windowDelegate;
public:
    NativeEventEmitter *getEmitter();
    void disable();
    void enable();
    void attachWidgets();
    void initialDisplay();
    void close();
    __strong NSWindow *getWindow();
    CocoaAppWindow(Core::Rect & rect,NativeEventEmitter *emitter);
    // ~CocoaAppWindow();
};
};

#endif
