#include "ButtonBase.h"

#ifndef OMEGAWTK_UI_PUSHBUTTON_H
#define OMEGAWTK_UI_PUSHBUTTON_H

namespace OmegaWTK::UI {
    class PushButton : public ButtonBase {
        SharedHandle<Composition::LayerStyle> style;
        void handleMouseEvent(Native::NativeEventPtr event);
        std::function<void(Native::NativeEventPtr)> handler;
    public:
        void onClick(std::function<void(Native::NativeEventPtr)> handler);
        void setStyle(SharedHandle<Composition::LayerStyle> & style);
        void setAnimator();
        PushButton(Core::Rect &rect,SharedHandle<Widget> parent = nullptr);
    };
};

#endif
