#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Native/NativeMenu.h"


#ifndef OMEGAWTK_UI_MENU_H
#define OMEGAWTK_UI_MENU_H

namespace OmegaWTK {

class Menu;

class OMEGAWTK_EXPORT MenuItem {
    Core::String name;
    Native::NMI native;
    Menu * subMenu;
    bool hasSubMenu;
    Menu *parent;
    friend class Menu;
    void setParentAndInit(Menu *menu);
public:
    /**
     Constructs a Normal Menu Item!
     */
    MenuItem(const Core::String & name,bool hasSubMenu,Menu *subMenu);
    /**
     Constructs a Seperator Menu Item!
     */
    MenuItem();
//    ~MenuItem();
};

class MenuDelegate;

class OMEGAWTK_EXPORT Menu {
    Core::String name;
    Core::Vector<MenuItem *> menuItems;
    Native::NM native;
    MenuDelegate *delegate;
    bool hasDelegate;
    friend class MenuItem;
public:
#ifdef TARGET_MACOS
    Native::NM getNativeMenu(){ return native;};
#endif
    Menu(const Core::String & name,std::initializer_list<MenuItem *> menu_items,MenuDelegate *delegate = nullptr);
//    ~Menu();
};

class OMEGAWTK_EXPORT MenuDelegate : public Native::NativeMenuDelegate {
protected:
    Menu *menu;
public:
    MenuDelegate();
    virtual void onSelectItem(unsigned itemIndex) = 0;
};

};

#endif