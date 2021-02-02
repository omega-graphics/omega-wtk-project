#include "omegaWTK/Native/NativeApp.h"

#import <Cocoa/Cocoa.h>

#ifndef OMEGAWTK_NATIVE_COCOAMENU_H
#define OMEGAWTK_NATIVE_COCOAMENU_H

namespace OmegaWTK::Native {

namespace Cocoa {

class CocoaMenuItem : public NativeAppMenuItem {
    NSMenuItem *item;
    friend class CocoaMenu;
public:
    CocoaMenuItem(Core::String & name,bool hasSubMenu,bool isSeperator,NAM subMenu);
    ~CocoaMenuItem();
};

class CocoaMenu : public NativeAppMenu {
    NSMenu *menu;
    void addMenuItem(NAMI menu_item);
    void insertMenuItem(NAMI menu_item,unsigned idx);
    friend class CocoaMenuItem;
public:
    NSMenu *getPtr(){return menu;};
    CocoaMenu();
    ~CocoaMenu();
};


};
};

#endif
