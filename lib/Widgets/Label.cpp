#include "omegaWTK/Widgets/Label.h"

namespace OmegaWTK {
    Label::Label(Core::String text_to_draw,Widget * parent,bool isEnabled):Widget(parent,isEnabled),text(text_to_draw){};

    Composition::CanvasPtr Label::render(){
        Composition::CanvasPtr canvas = Composition::make_canvas(getNativeItemPtr());
        return canvas;
    };
}