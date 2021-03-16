#include "omegaWTK/UI/ButtonBase.h"

namespace OmegaWTK::UI {

ButtonBase::ButtonViewDelegate(){};

void ButtonBase::ButtonViewDelegate::onLeftMouseDown(Native::NativeEventPtr event){
    widget->handleMouseEvent(event);
};

void ButtonBase::ButtonViewDelegate::onLeftMouseUp(Native::NativeEventPtr event){
    widget->handleMouseEvent(event);
};

void ButtonBase::ButtonViewDelegate::onRightMouseDown(Native::NativeEventPtr event){
    widget->handleMouseEvent(event);
}

void ButtonBase::ButtonViewDelegate::onRightMouseUp(Native::NativeEventPtr event){
    widget->handleMouseEvent(event);
};

ButtonBase::ButtonBase(const Core::Rect & rect,SharedHandle<Widget> parent):Widget(rect,parent),viewDelegate(new ButtonBase::ButtonViewDelegate()){
    
};

}
