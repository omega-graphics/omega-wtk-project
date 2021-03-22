#include "ButtonBase.h"

#ifndef OMEGAWTK_UI_PUSHBUTTON_H
#define OMEGAWTK_UI_PUSHBUTTON_H

namespace OmegaWTK::UI {
OMEGAWTK_EXPORT class PushButton : public ButtonBase {
        SharedHandle<Composition::LayerStyle> style;
        void handleMouseEvent(Native::NativeEventPtr event);
        std::function<void(Native::NativeEventPtr)> handler;
        SharedHandle<ViewAnimator> animator;
    public:
        void onClick(std::function<void(Native::NativeEventPtr)> handler);
        void setStyle(SharedHandle<Composition::LayerStyle> & style);
        void setAnimator(SharedHandle<ViewAnimator> & anim);
        PushButton(const Core::Rect &rect,SharedHandle<Widget> parent = nullptr);
//        ~PushButton();
    };

/// @name
/// Animator Key Values
/// @{
#define PUSH_BUTTON_EXIT 0
#define PUSH_BUTTON_ENTER 1
#define PUSH_BUTTON_DOWN 2
#define PUSH_BUTTON_UP 3
/// @}
};

#endif
