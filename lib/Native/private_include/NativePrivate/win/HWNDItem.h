#include "omegaWTK/Native/NativeItem.h"
#include <Windows.h>

#ifndef OMEGAWTK_NATIVE_WIN_HWNDITEM_H
#define OMEGAWTK_NATIVE_WIN_HWNDITEM_H
namespace OmegaWTK::Native {
    namespace Win {
        class HWNDItem : public NativeItem {
            HWND hwnd;
            public:
            enum : OPT_PARAM {
                View,
                PushButton,
            };
            private:
            OPT_PARAM type;
            public:
            HWNDItem(HWND _hwnd,OPT_PARAM _type):hwnd(_hwnd),type(_type){};
            ~HWNDItem(){};
        };
    };
};

#endif