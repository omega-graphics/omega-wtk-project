#include "omegaWTK/Native/NativeApp.h"

#import <Cocoa/Cocoa.h>

#ifndef OMEGAWTK_NATIVE_COCOAMENU_H
#define OMEGAWTK_NATIVE_COCOAMENU_H

namespace OmegaWTK::Native {

namespace Cocoa {

class CocoaMenuItem : public NativeAppMenuItem {
    NSMenuItem *menu;
public:
    CocoaMenuItem();
    ~CocoaMenuItem();
};

class CocoaMenu : public NativeAppMenu {
    NSMenu *menu;
public:
    CocoaMenu();
    ~CocoaMenu();
};


};
};

#endif
