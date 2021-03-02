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
    bool isSeperator;
public:
    void disable();
    void enable();
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
    Native::NM getNativeMenu(){ return native;};
    MenuItem *getItemByIdx(unsigned idx){ return menuItems[idx];};
    Menu(Core::String name,std::initializer_list<MenuItem *> menu_items,MenuDelegate *delegate = nullptr);
//    ~Menu();
};

class OMEGAWTK_EXPORT MenuDelegate : public Native::NativeMenuDelegate {
protected:
    Menu *menu;
    friend class Menu;
public:
    MenuDelegate();
    virtual void onSelectItem(unsigned itemIndex) = 0;
};
/**
 Creates a Category Menu
 */
MenuItem *CategoricalMenu(const Core::String & name,std::initializer_list<MenuItem *> items,MenuDelegate *delegate = nullptr);
/**
 Creates a SubMenu under a Categorical Menu
 */
MenuItem *SubMenu(const Core::String & name,std::initializer_list<MenuItem *> items,MenuDelegate *delegate = nullptr);
/**
 Creates a Menu Seperator Item!
*/
MenuItem *MenuSeperatorItem();

};


#ifdef TARGET_WIN32 
#ifdef WINDOWS_PRIVATE

void select_item_on_win_menu(void * win_menu,unsigned idx);

#endif 
#endif

#endif
