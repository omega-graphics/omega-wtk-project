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
    // bool hasSubMenu;
    unsigned idx = 0;
    friend class CocoaMenu;
public:
    bool isSeperator;
    void setState(bool state);
    NSMenuItem *item;
    void selectThisItem();
    void assignIndex(unsigned _idx){
        idx = _idx;
    };
    /**
     Constructs a Menu Item!
     */
    CocoaMenuItem(const OmegaCommon::String & str,CocoaMenu *parent,bool hasSubMenu,CocoaMenu *subMenu);
    /**
     Consructs a Seperator Menu Item!
     */
    CocoaMenuItem():sub_menu(nullptr),isSeperator(true),item([NSMenuItem separatorItem]){
        [item setEnabled:NO];
    };
    ~CocoaMenuItem(){};
};

class CocoaMenu : public NativeMenu {
    NSMenu *menu;
    OmegaCommon::Vector<CocoaMenuItem *> items;
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
    void updateMenu(){
        [menu update];
    };
    friend class CocoaMenuItem;
public:
    /**
     Constructs a Menu!
     */
    CocoaMenu(const OmegaCommon::String & name ){
        menu = [[NSMenu alloc] initWithTitle:common_string_to_ns_string(name)];
        [menu setAutoenablesItems:NO];
    };
    ~CocoaMenu(){
        [menu dealloc];
    };
};

CocoaMenuItem::CocoaMenuItem(const OmegaCommon::String & str,CocoaMenu *parent,bool hasSubMenu,CocoaMenu *subMenu):delegate([[CocoaMenuItemDelegate alloc] initWithCppBinding:this]),sub_menu(subMenu),parent_menu(parent),isSeperator(false),item([[NSMenuItem alloc] initWithTitle:common_string_to_ns_string(str) action:nil keyEquivalent:@""]){
    [delegate hasMenu];
    if(hasSubMenu){
        [item setSubmenu:sub_menu->menu];
    };
};

void CocoaMenuItem::selectThisItem(){
    parent_menu->menuItemSelected(this->idx);
};
void CocoaMenuItem::setState(bool state) {
    if(!isSeperator) {
       BOOL _state = NO;
       if(state){
           _state = YES;
       };
       [item setEnabled:_state];
       parent_menu->updateMenu();
    }
};


NMI make_cocoa_menu_item(const OmegaCommon::String & str,NM parent,bool hasSubMenu,NM subMenu){
    return new CocoaMenuItem(str,(CocoaMenu *)parent,hasSubMenu,(CocoaMenu *)subMenu);
};

NMI make_cocoa_menu_seperator(){
    return new CocoaMenuItem();
};

NM make_cocoa_menu(const OmegaCommon::String & name){
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
    if(!_cpp_binding->isSeperator) {
        [_cpp_binding->item setTarget:self];
        [_cpp_binding->item setAction:@selector(menuItemAction)];
        [_cpp_binding->item setEnabled:YES];
    }
};

-(void)menuItemAction {
    _cpp_binding->selectThisItem();
};

@end
