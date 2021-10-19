#include "WinMenu.h"

#include <Windows.h>
#include <memory>
#include <windowsx.h>
namespace OmegaWTK::Native::Win {

    class WinMenu;

    class WinMenuItem : public NativeMenuItem {
        MENUITEMINFO info;
        WinMenu *subMenu;
        bool hasSubMenu;
        WinMenu *parent;
        unsigned idx = 0;
        friend class WinMenu;
    public:
        void assignIndex(unsigned _idx){ idx = _idx;};
        void setState(bool state) override;
        /**
         Constructs a Regular Menu Item
         NOTE: Only Initalizes info!
        */
        WinMenuItem(const OmegaCommon::String & name,WinMenu *parent,bool hasSubMenu,WinMenu *subMenu);
        /**
         Constructs a Menu Seperator Item
         NOTE: Only Initalizes info!
        */
        WinMenuItem():subMenu(nullptr),hasSubMenu(false),parent(nullptr){
            info.cbSize = sizeof(info);
            // info.dwItemData = (ULONG_PTR)this;
            info.fMask = MIIM_FTYPE;
            info.fType = MFT_SEPARATOR;

        };
        ~WinMenuItem() override{

        };
    };

        class WinMenu : public NativeMenu {
            HMENU hmenu;
            MENUINFO info;
            friend class WinMenuItem;
            OmegaCommon::Vector<SharedHandle<WinMenuItem>> items;
            void insertMenuItem(NMI menu_item, unsigned idx) override {
                auto item = std::dynamic_pointer_cast<WinMenuItem>(menu_item);
                item->idx = idx;
                item->parent = this;
                InsertMenuItem(hmenu,idx,TRUE,&item->info);
                items.insert(items.begin() + idx,item);
            };
            void addMenuItem(NMI menu_item) override {
                auto item = std::dynamic_pointer_cast<WinMenuItem>(menu_item);
                InsertMenuItem(hmenu,items.size(),TRUE,&item->info);
                item->idx = items.size();
                item->parent = this;
                items.push_back(item);
            };
            /**
                Returns HMENU
            */
            void *getNativeBinding() override {
                return hmenu;
            };
            public:
            void onSelectItem(unsigned idx){
                if(hasDelegate)
                    delegate->onSelectItem(idx);
            };
            WinMenu(const OmegaCommon::String & name){

                info.dwMenuData = (ULONG_PTR)this;
                info.cbSize = sizeof(info);
                info.fMask = MIM_MENUDATA | MIM_BACKGROUND | MIM_STYLE;
                info.dwStyle = MNS_NOTIFYBYPOS;
                info.hbrBack = (HBRUSH)COLOR_WINDOW;
                // info.cyMax = 0;
                hmenu = CreateMenu();

                SetMenuInfo(hmenu,&info);    
            };
            ~WinMenu() override = default;
        };

        WinMenuItem::WinMenuItem(const OmegaCommon::String &name,WinMenu *parent,bool hasSubMenu,WinMenu *subMenu):subMenu(subMenu),hasSubMenu(hasSubMenu),parent(parent){
            info.cbSize = sizeof(info);
            info.dwItemData = (ULONG_PTR)this;
            info.fMask = MIIM_STRING | MIIM_DATA | MIIM_STATE;
            if(hasSubMenu){
                info.fMask |= MIIM_SUBMENU;
            }
            // info.fType = MFT_OWNERDRAW;
            info.dwTypeData = const_cast<char *>(name.c_str());
            info.cch = name.size();
            info.fState = MFS_ENABLED;
            if(hasSubMenu)
                info.hSubMenu = subMenu->hmenu;
        }

        void WinMenuItem::setState(bool state){
            UINT mask = MF_BYPOSITION;
            if(state) {
                mask |= MF_ENABLED;
            }
            else {
                mask |= MF_GRAYED | MF_DISABLED;
            }
            EnableMenuItem(parent->hmenu,idx,mask);
        }


    void select_item(void * menu,unsigned idx){
        WinMenu *_menu = (WinMenu *)menu;
        _menu->onSelectItem(idx);
    };
};

namespace OmegaWTK::Native {
    NM make_native_menu(const OmegaCommon::String &name){
        return (NM)new Win::WinMenu(name);
    };
    NMI make_native_menu_item(const OmegaCommon::String & str,NM parent,bool hasSubMenu,NM subMenu){
        return (NMI)new Win::WinMenuItem(str,(Win::WinMenu *)parent.get(),hasSubMenu,(Win::WinMenu *)subMenu.get());
    };
    NMI make_native_menu_seperator(){
        return (NMI)new Win::WinMenuItem();
    };
}