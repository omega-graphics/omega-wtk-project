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

    Compositor * Widget::getCompositorPtr(){
        return comp;
    };

    void Widget::setCompositor(Compositor *_comp){
        comp = _comp;
    };

    void Widget::addChild(Widget *child){
        children.push_back(child);
    };
}