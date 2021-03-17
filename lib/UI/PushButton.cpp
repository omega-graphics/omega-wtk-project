#include "omegaWTK/UI/PushButton.h"

namespace OmegaWTK::UI {
    PushButton::PushButton(const Core::Rect & rect,SharedHandle<Widget> parent):ButtonBase(rect,parent){
        /// Assume Default Style
        Composition::Layer *rootLayer = rootView->getLayerTreeLimb()->getRootLayer();
        auto defaultStyle = make<Composition::LayerStyle>();
        defaultStyle->setBackgroundColor(Composition::Color::Transparent);
        defaultStyle->add(VISUAL_RECT(Rect(0,0,rect.dimen.minWidth,rect.dimen.minHeight),Composition::ColorBrush(Composition::Color::Blue)));
        rootLayer->setStyle(defaultStyle);
    };

    void PushButton::handleMouseEvent(Native::NativeEventPtr event){
        if(event->type == Native::NativeEvent::CursorEnter){
            animator->activateTrigger(PUSH_BUTTON_ENTER);
        }
        else if(event->type == Native::NativeEvent::CursorExit){
            animator->activateTrigger(PUSH_BUTTON_EXIT);
        }
        else if(event->type == Native::NativeEvent::LMouseDown){
            animator->activateTrigger(PUSH_BUTTON_DOWN);
        }
        else if(event->type == Native::NativeEvent::LMouseUp){
            animator->activateTrigger(PUSH_BUTTON_UP);
            handler(event);
        };
    };

    void PushButton::onClick(std::function<void(Native::NativeEventPtr)> handler){
        this->handler = handler;
    };
}
