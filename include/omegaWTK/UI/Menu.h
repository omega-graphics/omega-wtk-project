#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Native/NativeMenu.h"


#ifndef OMEGAWTK_UI_MENU_H
#define OMEGAWTK_UI_MENU_H

namespace OmegaWTK {

class Menu;

class OMEGAWTK_EXPORT  MenuItem {
    OmegaCommon::String name;
    Native::NMI native;
    Menu * subMenu;
    bool hasSubMenu;
    Menu *parent;
    friend class Menu;
    void setParentAndInit(Menu *menu);
    bool isSeperator;
public:
    void disable();
    void enable();
    /**
     Constructs a Normal Menu Item!
     */
    MenuItem(const OmegaCommon::String & name,bool hasSubMenu,Menu *subMenu);
    /**
     Constructs a Seperator Menu Item!
     */
    MenuItem();
//    ~MenuItem();
};

class MenuDelegate;
 
 class OMEGAWTK_EXPORT  Menu {
    OmegaCommon::String name;
    OmegaCommon::Vector<MenuItem *> menuItems;
    Native::NM native;
    MenuDelegate *delegate;
    bool hasDelegate;
    friend class MenuItem;
public:
    Native::NM getNativeMenu(){ return native;};
    MenuItem *getItemByIdx(unsigned idx){ return menuItems[idx];};
    Menu(OmegaCommon::String name,std::initializer_list<MenuItem *> menu_items,MenuDelegate *delegate = nullptr);
//    ~Menu();
};

INTERFACE OMEGAWTK_EXPORT MenuDelegate : public Native::NativeMenuDelegate {
protected:
    Menu *menu;
    friend class Menu;
public:
    MenuDelegate();
    INTERFACE_METHOD void onSelectItem(unsigned itemIndex);
};
/**
 Creates a Category Menu
 */
OMEGAWTK_EXPORT MenuItem *CategoricalMenu(const OmegaCommon::String & name,std::initializer_list<MenuItem *> items,MenuDelegate *delegate = nullptr);
/**
 Creates a SubMenu under a Categorical Menu
 */
OMEGAWTK_EXPORT MenuItem *SubMenu(const OmegaCommon::String & name,std::initializer_list<MenuItem *> items,MenuDelegate *delegate = nullptr);

OMEGAWTK_EXPORT MenuItem *ButtonMenuItem(const OmegaCommon::String & name);
/**
 Creates a Menu Seperator Item!
*/
OMEGAWTK_EXPORT MenuItem *MenuItemSeperator();

};


#ifdef TARGET_WIN32 
#ifdef WINDOWS_PRIVATE

OMEGAWTK_EXPORT void select_item_on_win_menu(void * win_menu,unsigned idx);

#endif 
#endif

#endif
