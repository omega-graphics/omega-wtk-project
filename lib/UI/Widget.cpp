#include "omegaWTK/Widgets/Widget.h"

namespace OmegaWTK {

Widget::Widget(Widget * parent,Core::Rect _rect,bool enabled):isEnabled(enabled),rect(_rect){
    if(parent){
        parent->addChild(this);
        isRoot = false;
        comp = parent->getCompositorPtr();
    }
    else
        isRoot = true;
};

void Widget::setNativeItemPtr(Native::NativeItemPtr ptr){
    native = ptr;
};

Composition::CompPtr Widget::getCompositorPtr(){
    return comp;
};

Native::NativeItemPtr Widget::getNativeItemPtr(){
    return native;
};

void Widget::setCompositor(Composition::CompPtr _comp){
    comp = _comp;
};

void Widget::addChild(Widget *child){
    children.push_back(child);
};

}
