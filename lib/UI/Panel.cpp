#include "omegaWTK/UI/Panel.h"

namespace OmegaWTK::UI {
    void Panel::setAnimator(SharedHandle<ViewAnimator> &viewAnimator){
        rootViewAnim = viewAnimator;
    };
    void Panel::setStyle(SharedHandle<Composition::LayerStyle> &style){
        visiblelayerStyle = style;
    };
}; 