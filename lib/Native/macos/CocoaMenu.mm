#include "CocoaMenu.h"
#include "NativePrivate/macos/CocoaUtils.h"

#import <Cocoa/Cocoa.h>

namespace OmegaWTK::Native::Cocoa {
    class CocoaMenuItem;
};

@interface CocoaMenuItemDelegate : NSObject
@property (strong) NSMenuItem *menu;
-(void)hasMenu;
-(instancetype)initWithCppBinding:(OmegaWTK::Native::Cocoa::CocoaMenuItem *)cpp_binding;
@end


namespace OmegaWTK::Native::Cocoa {


class CocoaMenu;

class CocoaMenuItem : public NativeMenuItem {
    CocoaMenuItemDelegate *delegate;
    CocoaMenu *sub_menu;
    CocoaMenu *parent_menu;
    bool hasSubMenu;
    unsigned idx = 0;
    friend class CocoaMenu;
public:
    NSMenuItem *item;
    void selectThisItem();
    void assignIndex(unsigned _idx){
        idx = _idx;
    };
    /**
     Constructs a Menu Item!
     */
    CocoaMenuItem(const Core::String & str,CocoaMenu *parent,bool hasSubMenu,CocoaMenu *subMenu);
    /**
     Consructs a Seperator Menu Item!
     */
    CocoaMenuItem():item([NSMenuItem separatorItem]),sub_menu(nullptr),hasSubMenu(false){
       
    };
    ~CocoaMenuItem(){};
};

class CocoaMenu : public NativeMenu {
    NSMenu *menu;
    Core::Vector<CocoaMenuItem *> items;
    void menuItemSelected(unsigned idx){
        if(hasDelegate)
            delegate->onSelectItem(idx);
    };
    void addMenuItem(NMI menu_item) {
        CocoaMenuItem *item = (CocoaMenuItem *)menu_item;
        [menu addItem:item->item];
        item->assignIndex(items.size());
        items.push_back(item);
    };
    void insertMenuItem(NMI menu_item, unsigned idx) {
        CocoaMenuItem *item = (CocoaMenuItem *)menu_item;
        [menu insertItem:item->item atIndex:idx];
        item->assignIndex(idx);
        items.insert(items.begin() + idx,item);
    };
    void * getNativeBinding(){
        return (void *)menu;
    };
    friend class CocoaMenuItem;
public:
    /**
     Constructs a Menu!
     */
    CocoaMenu(const Core::String & name ){
        menu = [[NSMenu alloc] initWithTitle:core_string_to_ns_string(name)];
    };
    ~CocoaMenu(){
        [menu dealloc];
    };
};

CocoaMenuItem::CocoaMenuItem(const Core::String & str,CocoaMenu *parent,bool hasSubMenu,CocoaMenu *subMenu):item([[NSMenuItem alloc] initWithTitle:core_string_to_ns_string(str) action:nil keyEquivalent:@""]),delegate([[CocoaMenuItemDelegate alloc] initWithCppBinding:this]),parent_menu(parent),hasSubMenu(hasSubMenu),sub_menu(subMenu){
    [delegate hasMenu];
    if(hasSubMenu){
        [item setSubmenu:sub_menu->menu];
    };
};

void CocoaMenuItem::selectThisItem(){
    parent_menu->menuItemSelected(this->idx);
};


NMI make_cocoa_menu_item(const Core::String & str,NM parent,bool hasSubMenu,NM subMenu){
    return new CocoaMenuItem(str,(CocoaMenu *)parent,hasSubMenu,(CocoaMenu *)subMenu);
};

NMI make_cocoa_menu_seperator(){
    return new CocoaMenuItem();
};

NM make_cocoa_menu(const Core::String & name){
    return new CocoaMenu(name);
};

};

@implementation CocoaMenuItemDelegate {
    OmegaWTK::Native::Cocoa::CocoaMenuItem *_cpp_binding;
}

- (instancetype)initWithCppBinding:(OmegaWTK::Native::Cocoa::CocoaMenuItem *)cpp_binding
{
    self = [super init];
    if (self) {
        _cpp_binding = cpp_binding;
    }
    return self;
}

-(void)hasMenu {
    [_cpp_binding->item setTarget:self];
    [_cpp_binding->item setAction:@selector(menuItemAction)];
    [_cpp_binding->item setEnabled:YES];
};

-(void)menuItemAction {
    _cpp_binding->selectThisItem();
};

@end
