#include "omegaWTK/Native/NativeMenu.h"
#include "NativePrivate/macos/CocoaUtils.h"

#import <Cocoa/Cocoa.h>

namespace OmegaWTK::Native::Cocoa {
    class CocoaMenuItem;
};

@interface CocoaMenuItemDelegate : NSObject
-(void)hasMenu;
-(instancetype)initWithCppBinding:(OmegaWTK::Native::Cocoa::CocoaMenuItem *)cpp_binding;
@end


namespace OmegaWTK::Native::Cocoa {


class CocoaMenu;

class CocoaMenuItem : public NativeMenuItem {
    CocoaMenuItemDelegate *delegate;
    SharedHandle<CocoaMenu> sub_menu;
    SharedHandle<CocoaMenu> parent_menu;
    // bool hasSubMenu;
    unsigned idx = 0;
    friend class CocoaMenu;
public:
    bool isSeperator;
    void setState(bool state) override;
    NSMenuItem *item;
    void selectThisItem();
    void assignIndex(unsigned _idx){
        idx = _idx;
    };
    /**
     Constructs a Menu Item!
     */
    CocoaMenuItem(const OmegaCommon::String & str,SharedHandle<CocoaMenu> parent,bool hasSubMenu,SharedHandle<CocoaMenu> subMenu);
    /**
     Consructs a Seperator Menu Item!
     */
    explicit CocoaMenuItem():sub_menu(nullptr),isSeperator(true),item([NSMenuItem separatorItem]){
        [item setEnabled:NO];
    };
    ~CocoaMenuItem() override = default;
};

class CocoaMenu : public NativeMenu {
    NSMenu *menu;
    OmegaCommon::Vector<SharedHandle<CocoaMenuItem>> items;
    void menuItemSelected(unsigned idx){
        if(hasDelegate)
            delegate->onSelectItem(idx);
    };
    void addMenuItem(NMI menu_item) override {
        auto item = std::dynamic_pointer_cast<CocoaMenuItem>(menu_item);
        [menu addItem:item->item];
        item->assignIndex(items.size());
        items.push_back(item);
    };
    void insertMenuItem(NMI menu_item, unsigned idx) override {
        auto item = std::dynamic_pointer_cast<CocoaMenuItem>(menu_item);
        [menu insertItem:item->item atIndex:idx];
        item->assignIndex(idx);
        items.emplace(items.cbegin() + idx,item);
    };
    void * getNativeBinding() override{
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
    explicit CocoaMenu(const OmegaCommon::String & name ){
        menu = [[NSMenu alloc] initWithTitle:common_string_to_ns_string(name)];
        [menu setAutoenablesItems:NO];
    };
    ~CocoaMenu() override{
        [menu dealloc];
    };
};

CocoaMenuItem::CocoaMenuItem(const OmegaCommon::String & str,
                             SharedHandle<CocoaMenu> parent,
                             bool hasSubMenu,
                             SharedHandle<CocoaMenu> subMenu):
        delegate([[CocoaMenuItemDelegate alloc] initWithCppBinding:this]),
        sub_menu(subMenu),parent_menu(parent),
        isSeperator(false),
        item([[NSMenuItem alloc] initWithTitle:common_string_to_ns_string(str) action:nil keyEquivalent:@""]){
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

};

namespace OmegaWTK::Native {
    NMI make_native_menu_item(const OmegaCommon::String & str,NM parent,bool hasSubMenu,NM subMenu){
        return (NMI)new Cocoa::CocoaMenuItem(str,std::dynamic_pointer_cast<Cocoa::CocoaMenu>(parent),hasSubMenu,std::dynamic_pointer_cast<Cocoa::CocoaMenu>(subMenu));
    };

    NMI make_native_menu_seperator(){
        return (NMI)new Cocoa::CocoaMenuItem();
    };

    NM make_native_menu(const OmegaCommon::String & name){
        return (NM)new Cocoa::CocoaMenu(name);
    };
}

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
