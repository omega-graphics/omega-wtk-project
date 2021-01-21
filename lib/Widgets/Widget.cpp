#include "omegaWTK/Widgets/Widget.h"

namespace OmegaWTK {

    Widget::Widget(Widget * parent,bool enabled):isEnabled(enabled){
        if(parent){
            parent->addChild(this);
            isRoot = false;
            comp = parent->getCompositorPtr();
        }
        else 
            isRoot = true;
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