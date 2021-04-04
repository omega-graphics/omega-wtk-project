#include "omegaWTK/UI/Layout.h"

namespace OmegaWTK {
WidgetLayoutDescriptor::WidgetLayoutDescriptor(SharedHandle<Widget> widget,float ratio,Core::Optional<unsigned> height):widget(widget),ratio(ratio),height(height){
    
};

StaticLayout::StaticLayout(const Core::Rect & rect,std::initializer_list<WidgetLayoutDescriptor> widgetLayouts,bool implyPadding):layoutRect(rect),widgetLayouts(widgetLayouts){
    auto layout_desc_it = widgetLayouts.begin();
    while (layout_desc_it != widgetLayouts.end()) {
        ++layout_desc_it;
    }
};

}
