#include "omegaWTK/Widgets/Widget.h"

#ifdef TARGET_WIN32
#include "NativeItem_Win.h"
#endif

#ifdef TARGET_MACOS
#include "NativeItem_Mac.h"
#endif

namespace OmegaWTK {

    NativeItem * make_native_item_ptr(){
        #ifdef TARGET_WIN32
        return win_native_item(); 
        #endif
        #ifdef TARGET_MACOS
        return mac_native_item(); 
        #endif
    };

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