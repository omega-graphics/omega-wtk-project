#include "omegaWTK/Native/NativeMenu.h"


#ifndef OMEGAWTK_NATIVE_WIN_WINMENU_H
#define OMEGAWTK_NATIVE_WIN_WINMENU_H

namespace OmegaWTK::Native {
    namespace Win {
        class WinMenuItem;
        class WinMenu : public NativeMenu {
            HMENU hmenu;
            MENUINFO info;
            friend class WinMenuItem;
            OmegaCommon::Vector<SharedHandle<WinMenuItem>> items;
            void insertMenuItem(NMI menu_item, unsigned idx) override;
            void addMenuItem(NMI menu_item) override;

        public:
            /**
               Returns HMENU
           */
            void *getNativeBinding() override {
                return hmenu;
            };
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

        void select_item(void * menu,unsigned idx);
    };
};


#endif