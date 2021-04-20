#include "omegaWTK/UI/Panel.h"

namespace OmegaWTK::UI {
    Panel::Panel(const Core::Rect & rect,SharedHandle<Widget> parent):Widget(rect,parent){
        auto style = make<Composition::LayerStyle>();
        style->setBackgroundColor(Composition::Color::Transparent);
        rootView->getLayerTreeLimb()->getRootLayer()->setStyle(style);
        
        /// DEFAULT STYLE!
        auto layerStyle = make<Composition::LayerStyle>();
        auto b = Composition::ColorBrush(0xCCCCCC);
        auto black = Composition::ColorBrush(Composition::Color::Black);
        layerStyle->setBackgroundColor(Composition::Color::Transparent);
        layerStyle->add(VISUAL_ROUNDED_RECT_W_FRAME(OmegaWTK::RoundedRect(rect.pos.x + 25,rect.pos.y + 25,rect.dimen.minWidth - 50,rect.dimen.minHeight - 50,25,25),Composition::ColorBrush(Composition::Color::White),Composition::Border(b,2)));
        auto shadow = make<Composition::LayerEffect>(LAYER_EFFECT_DROPSHADOW(-5.0,-5.0,20,1.0,1.0,Composition::Color::Black));
        layerStyle->addEffect(shadow);
        visiblelayerStyle = layerStyle;
        auto layer = makeLayer(OmegaWTK::Rect(0,0,rect.dimen.minWidth,rect.dimen.minHeight));
        layer->setStyle(layerStyle);
        rootView->getLayerTreeLimb()->addLayer(layer);
    };

    void Panel::resize(Core::Rect &newRect){
        /// 1. Resize Native View.
        rootView->resize(newRect);
        /// 2. Resize Transparent Root Layer.
        rootView->getLayerTreeLimb()->getRootLayer();
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