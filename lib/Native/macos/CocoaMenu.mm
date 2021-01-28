#import "CocoaMenu.h"
#import "NativePrivate/macos/CocoaUtils.h"

namespace OmegaWTK::Native::Cocoa {
CocoaMenuItem::CocoaMenuItem(Core::String & name,bool hasSubMenu,bool isSeperator,NAM subMenu){
    if(isSeperator)
        item = [NSMenuItem separatorItem];
    else {
        item = [[NSMenuItem alloc] init];
        [item setTitle:core_string_to_ns_string(name)];
        if(hasSubMenu) {
            CocoaMenu *m = (CocoaMenu *)subMenu;
            [item setSubmenu:m->menu];
        }
    };
};

CocoaMenuItem::~CocoaMenuItem(){
    [item dealloc];
};

CocoaMenu::CocoaMenu(){
    menu = [[NSMenu alloc] init];
};

void CocoaMenu::addMenuItem(NAMI menu_item){
    [menu addItem:((CocoaMenuItem *)menu_item)->item];
};

void CocoaMenu::insertMenuItem(NAMI menu_item,unsigned idx){
    [menu insertItem:((CocoaMenuItem *)menu_item)->item atIndex:idx];
};

CocoaMenu::~CocoaMenu(){
    [menu dealloc];
};
};
