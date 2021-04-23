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
        layerStyle->setBackgroundColor(Composition::Color::Red);
        layerStyle->add(VISUAL_ROUNDED_RECT_W_FRAME(OmegaWTK::RoundedRect(rect.pos.x + 25,rect.pos.y + 25,rect.dimen.minWidth - 50,rect.dimen.minHeight - 50,25,25),Composition::ColorBrush(Composition::Color::White),Composition::Border(b,2)));
        auto shadow = make<Composition::LayerEffect>(LAYER_EFFECT_DROPSHADOW(-5.0,-5.0,20,1.0,1.0,Composition::Color::Black));
        layerStyle->addEffect(shadow);
        visiblelayerStyle = layerStyle;
        visibleLayer = makeLayer(OmegaWTK::Rect(0,0,rect.dimen.minWidth,rect.dimen.minHeight));
        visibleLayer->setStyle(layerStyle);
        rootView->getLayerTreeLimb()->addLayer(visibleLayer);
    };

    void Panel::resize(Core::Rect &newRect){
        /// Call if using default style..
        /// 1. Resize Native View.
        rootView->resize(newRect);
        /// 2. Resize Transparent Root Layer.
        rootView->getLayerTreeLimb()->getRootLayer()->resize(newRect);
        /// 3. Resize Visible Layer.
        visibleLayer->resize(newRect);
        /// 4. Resize Visuals in Visible Layer Style.
        {
           auto roundedRect = visiblelayerStyle->getVisualAtIndex<Composition::Visual::RoundedRectParams>(0);
           roundedRect->rect.pos.x = newRect.pos.x + 25;
           roundedRect->rect.pos.y = newRect.pos.y + 25;
           roundedRect->rect.dimen.minWidth = newRect.dimen.minWidth - 50;
           roundedRect->rect.dimen.minHeight = newRect.dimen.minHeight - 50;
        }
    };

    void Panel::setAnimator(SharedHandle<ViewAnimator> &viewAnimator){
        rootViewAnim = viewAnimator;
    };
    void Panel::setStyle(SharedHandle<Composition::LayerStyle> &style){
        visiblelayerStyle = style;
        visibleLayer->setStyle(style);
    };
}; 