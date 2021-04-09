#include "omegaWTK/UI/Panel.h"

namespace OmegaWTK::UI {
    Panel::Panel(const Core::Rect & rect,SharedHandle<Widget> parent):Widget(rect,parent){
        auto style = make<Composition::LayerStyle>();
        style->setBackgroundColor(Composition::Color::Transparent);
        rootView->getLayerTreeLimb()->getRootLayer()->setStyle(style);
    };

    void Panel::resize(Core::Rect &newRect){
        
    };

    void Panel::setAnimator(SharedHandle<ViewAnimator> &viewAnimator){
        rootViewAnim = viewAnimator;
    };
    void Panel::setStyle(SharedHandle<Composition::LayerStyle> &style){
        visiblelayerStyle = style;
        auto & rect = rootView->getRect();
        auto layer = makeLayer(OmegaWTK::Rect(0,0,rect.dimen.minWidth,rect.dimen.minHeight));
        layer->setStyle(style);
        rootView->getLayerTreeLimb()->addLayer(layer);
    };
}; 