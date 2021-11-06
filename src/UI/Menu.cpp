#include "omegaWTK/UI/Menu.h"

namespace OmegaWTK {

MenuItem::MenuItem(const OmegaCommon::String & name,bool hasSubMenu,SharedHandle<Menu> subMenu):name(name),native(nullptr),hasSubMenu(hasSubMenu),subMenu(subMenu),isSeperator(false){
    
};

MenuItem::MenuItem():name(),native(nullptr),hasSubMenu(false),subMenu(nullptr),isSeperator(true){
    
};

void MenuItem::setParentAndInit(Menu *menu){
    parent = menu;
    if(!isSeperator) {
        if(hasSubMenu)
            native = Native::make_native_menu_item(name,parent->native,hasSubMenu,subMenu->native);
        else
            native = Native::make_native_menu_item(name,parent->native,hasSubMenu,nullptr);
    }
    else {
        native = Native::make_native_menu_seperator();
    }
};

void MenuItem::enable(){
    native->setState(true);
};

void MenuItem::disable(){
    native->setState(false);
};

Menu::Menu(OmegaCommon::String name,std::initializer_list<SharedHandle<MenuItem>> menu_items,MenuDelegate * delegate):
name(name),
menuItems(menu_items),
native(Native::make_native_menu(name)),
delegate(delegate),
hasDelegate(delegate != nullptr){
    auto it = menu_items.begin();
    while(it != menu_items.end()){
        auto & menu_item = *it;
        menu_item->setParentAndInit(this);
        native->addMenuItem(menu_item->native);
        ++it;
    };
    
    if(hasDelegate) {
        delegate->menu = this;
        native->setDelegate(delegate);
    }
};

MenuDelegate::MenuDelegate(){};

SharedHandle<MenuItem> CategoricalMenu(const OmegaCommon::String & name,std::initializer_list<SharedHandle<MenuItem>> items,MenuDelegate *delegate){
#ifdef TARGET_WIN32
    return new MenuItem(name,true,new Menu("",items,delegate));
#endif
#ifdef TARGET_MACOS
    return (SharedHandle<MenuItem>)new MenuItem("",true,(SharedHandle<Menu>)new Menu(name,items,delegate));
#endif
};

SharedHandle<MenuItem> ButtonMenuItem(const OmegaCommon::String & name){
    return (SharedHandle<MenuItem>)new MenuItem(name,false,nullptr);
};

SharedHandle<MenuItem> SubMenu(const OmegaCommon::String & name,std::initializer_list<SharedHandle<MenuItem>> items,MenuDelegate *delegate){
    #ifdef TARGET_WIN32
        return (SharedHandle<MenuItem>) new MenuItem(name,true,new Menu("",items,delegate));
    #endif 
    #ifdef TARGET_MACOS 
        return (SharedHandle<MenuItem>) new MenuItem(name,true,(SharedHandle<Menu>)new Menu("",items,delegate));
    #endif
};

SharedHandle<MenuItem> MenuItemSeperator(){
    return (SharedHandle<MenuItem>) new MenuItem();
};

};

#ifdef TARGET_WIN32 
#ifdef WINDOWS_PRIVATE

void select_item_on_win_menu(void * win_menu,unsigned idx){
    return __select_item_on_win_menu(win_menu, idx);
};

#endif 
#endif
