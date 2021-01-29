#include "omegaWTK/UI/Label.h"

namespace OmegaWTK {
    Label::Label(Core::Text text_to_draw,Widget * parent,Core::Rect _rect,bool isEnabled):Widget(parent,_rect,isEnabled),text(std::move(text_to_draw)){
        setNativeItemPtr(Native::make_native_item(_rect));
        canvas = Composition::make_canvas(getNativeItemPtr());
    };

    void Label::render(){
        Composition::CompPtr comp = getCompositorPtr();
        comp->setCanvas(canvas);
        comp->drawText(text);
    };
}
