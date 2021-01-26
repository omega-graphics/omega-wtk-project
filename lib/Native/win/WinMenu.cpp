#include "WinMenu.h"

namespace OmegaWTK::Native::Win {
    WinMenuItem::WinMenuItem(Core::String & name,bool hasSubMenu,NAM subMenu){
        UINT featureMask; 
        if(hasSubMenu)
            featureMask = MIIM_STRING | MIIM_SUBMENU;
        else 
            featureMask = MIIM_STRING;
        WinMenu *sM = (WinMenu*)subMenu;
        menu_item_info.cbSize = sizeof(MENUITEMINFO);
        menu_item_info.fMask = featureMask;
        if(hasSubMenu)
            menu_item_info.hSubMenu = sM->hMenu;
        menu_item_info.cch = name.size();
        menu_item_info.dwTypeData = name.data();
    };

    WinMenu::WinMenu(){
        menu_info.cbSize = sizeof(MENUINFO);
        hMenu = CreateMenu();
        SetMenuInfo(hMenu,&menu_info);
    };

    void WinMenu::addMenuItem(NAMI menu_item){
        WinMenuItem *item = (WinMenuItem *)menu_item;
        InsertMenuItem(hMenu,GetMenuItemCount(hMenu) - 1,TRUE,&item->menu_item_info);
    };
};