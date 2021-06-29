#include "Widget.h"

#ifndef OMEGAWTK_UI_PANEL_H
#define OMEGAWTK_UI_PANEL_H

namespace OmegaWTK::UI {

    class OMEGAWTK_EXPORT Panel : public Widget {
        SharedHandle<ViewAnimator> rootViewAnim;
        SharedHandle<Composition::Layer> visibleLayer;
    public:
        void setAnimator(SharedHandle<ViewAnimator> & viewAnimator);
        WIDGET_CONSTRUCTOR_DEFAULT(Panel);
        void resize(Core::Rect &newRect);
    };

};

#endif