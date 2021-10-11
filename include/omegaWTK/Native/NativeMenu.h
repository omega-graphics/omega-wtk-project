#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTK_NATIVE_NATIVEMENU_H
#define OMEGAWTK_NATIVE_NATIVEMENU_H

namespace OmegaWTK {
namespace Native {


class NativeMenuItem {
public:
    virtual void setState(bool state) = 0;
    virtual ~NativeMenuItem() = default;
};

typedef SharedHandle<NativeMenuItem> NMI;

class NativeMenuDelegate;

class NativeMenu {
protected:
    NativeMenuDelegate *delegate;
    bool hasDelegate = false;
public:
    void setDelegate(NativeMenuDelegate *delegate);
    virtual void *getNativeBinding() = 0;
    virtual void addMenuItem(NMI menu_item) = 0;
    virtual void insertMenuItem(NMI menu_item,unsigned idx) = 0;
    virtual ~NativeMenu() = default;
};
typedef SharedHandle<NativeMenu> NM;


INTERFACE NativeMenuDelegate {
public:
    INTERFACE_METHOD void onSelectItem(unsigned itemIndex) ABSTRACT;
};

NMI make_native_menu_item(const OmegaCommon::String & str,NM parent,bool hasSubMenu,NM subMenu);
NMI make_native_menu_seperator();
NM make_native_menu(const OmegaCommon::String & name);

};
};

#ifdef TARGET_WIN32
#ifdef WINDOWS_PRIVATE

void __select_item_on_win_menu(void * win_menu,unsigned idx);
#endif
#endif


#endif
