#include "Widget.h"

#ifndef OMEGAWTK_UI_PANEL_H
#define OMEGAWTK_UI_PANEL_H

namespace OmegaWTK::UI {

    class OMEGAWTK_EXPORT Panel : public Widget {
        SharedHandle<ViewAnimator> rootViewAnim;
        SharedHandle<Composition::Layer> visibleLayer;
        SharedHandle<Composition::LayerStyle> visiblelayerStyle;
    public:
        void setAnimator(SharedHandle<ViewAnimator> & viewAnimator);
        void setStyle(SharedHandle<Composition::LayerStyle> & style);
        Panel(const Core::Rect & rect,SharedHandle<Widget> parent = nullptr);
        void resize(Core::Rect &newRect);
    };

};

#endif