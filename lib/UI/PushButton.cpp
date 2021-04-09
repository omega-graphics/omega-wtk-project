#include "omegaWTK/UI/PushButton.h"

namespace OmegaWTK::UI {
    PushButton::PushButton(const Core::Rect & rect,SharedHandle<Widget> parent):ButtonBase(rect,parent){
        /// Assume Default Style
//        std::cout << "Ptr:" << rootView.get() << std::endl;
        auto rootLayer = rootView->getLayerTreeLimb()->getRootLayer();
        auto defaultStyle = make<Composition::LayerStyle>();
        defaultStyle->setBackgroundColor(Composition::Color::Transparent);
        rootLayer->setStyle(defaultStyle);
    };

    void PushButton::setStyle(SharedHandle<Composition::LayerStyle> & style){
        this->style = style;
    };

    void PushButton::handleMouseEvent(Native::NativeEventPtr event){
//        if(event->type == Native::NativeEvent::CursorEnter){
//            animator->activateTrigger(PUSH_BUTTON_ENTER);
//        }
//        else if(event->type == Native::NativeEvent::CursorExit){
//            animator->activateTrigger(PUSH_BUTTON_EXIT);
//        }
//        else if(event->type == Native::NativeEvent::LMouseDown){
//            animator->activateTrigger(PUSH_BUTTON_DOWN);
//        }
//        else if(event->type == Native::NativeEvent::LMouseUp){
//            animator->activateTrigger(PUSH_BUTTON_UP);
//            handler(event);
//        };
    };

    void PushButton::onClick(std::function<void(Native::NativeEventPtr)> handler){
        this->handler = handler;
    };

//    PushButton::~PushButton(){
//        
//    };
}
