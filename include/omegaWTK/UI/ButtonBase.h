#include "Widget.h"
#include "ViewAnimator.h"

#ifndef OMEGAWTK_UI_BUTTONBASE_H
#define OMEGAWTK_UI_BUTTONBASE_H

namespace OmegaWTK::UI {
    /**
     
     @abstract
     */
    class OMEGAWTK_EXPORT ButtonBase : public Widget {
    protected:
        class ButtonViewDelegate : public ViewDelegate {
            ButtonBase *widget;
        public:
            void onMouseEnter(Native::NativeEventPtr event);
            void onMouseExit(Native::NativeEventPtr event);
            void onLeftMouseUp(Native::NativeEventPtr event);
            void onLeftMouseDown(Native::NativeEventPtr event);
        };
        virtual void handleMouseEvent(Native::NativeEventPtr event);
        ButtonViewDelegate *viewDelegate;
    public:
        ButtonBase(const Core::Rect & rect,SharedHandle<Widget> = nullptr);
    };
};

#endif
