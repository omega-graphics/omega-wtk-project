#include "omegaWTK/UI/TextInput.h"

namespace OmegaWTK::UI {
    Core::String & TextInput::string(){
        return textStr;
    };

    
    TextInput::TextInput(const Core::Rect & rect,Core::String placeholder,SharedHandle<Widget> parent):Widget(rect,parent),textStr(""),placeholder(placeholder){
        /// Assume default style.
        auto rootLayer = rootView->getLayerTreeLimb()->getRootLayer();
        auto style = make<Composition::LayerStyle>();
        style->setBackgroundColor(Composition::Color::Transparent);
        style->add(VISUAL_RECT(Rect(0,0,rect.dimen.minWidth,rect.dimen.minHeight),Composition::ColorBrush(Composition::Color::Red)));
        rootLayer->setStyle(style);
        
    };
};