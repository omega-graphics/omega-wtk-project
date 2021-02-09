#include "omegaWTK/UI/Menu.h"

namespace OmegaWTK {

MenuItem::MenuItem(const Core::String & name,bool hasSubMenu,Menu *subMenu):name(name),native(nullptr),hasSubMenu(hasSubMenu),subMenu(subMenu){
    
};

MenuItem::MenuItem():name(),native(Native::make_native_menu_seperator()),hasSubMenu(false),subMenu(nullptr){
    
};

void MenuItem::setParentAndInit(Menu *menu){
    parent = menu;
    if(hasSubMenu)
        native = Native::make_native_menu_item(name,parent->native,hasSubMenu,subMenu->native);
    else
        native = Native::make_native_menu_item(name,parent->native,hasSubMenu,nullptr);
};

Menu::Menu(const Core::String & name,std::initializer_list<MenuItem *> menu_items,MenuDelegate * delegate):name(name),menuItems(menu_items),native(Native::make_native_menu(name)),delegate(delegate),hasDelegate(delegate != nullptr){
    auto it = menu_items.begin();
    while(it != menu_items.end()){
        auto & menu_item = *it;
        menu_item->setParentAndInit(this);
        native->addMenuItem(menu_item->native);
        ++it;
    };
    
    if(hasDelegate)
        native->setDelegate(delegate);
};

MenuDelegate::MenuDelegate(){};

};
