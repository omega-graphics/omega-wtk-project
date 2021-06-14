#include "omegaWTK/UI/Layout.h"

namespace OmegaWTK {




WidgetLayoutDescriptor::WidgetLayoutDescriptor(SharedHandle<Widget> widget,float ratio,Core::Optional<unsigned> dimen):widget(widget),ratio(ratio),dimen(dimen){
    
};


class LayoutChildObserver : public WidgetObserver {
    Widget * neighbor;
    void resizeNeighbor(Core::Rect & nRect){
        neighbor->resize(nRect);
    };
public:
    void onWidgetChangeSize(Core::Rect oldRect, Core::Rect &newRect){
        /// Move Neighbor
    };
    void onWidgetDidHide(){
        /// Move Neighbor
    };
    void onWidgetDidShow(){
        /// Move Neighbor
    };


};


Layout::Layout(const Core::Rect & rect,std::initializer_list<WidgetLayoutDescriptor> widgetLayouts,bool implyPadding):layoutRect(rect),widgetLayouts(widgetLayouts){
    auto layout_desc_it = widgetLayouts.begin();
    while (layout_desc_it != widgetLayouts.end()) {
        ++layout_desc_it;
    }
};

bool Layout::Iterator::operator!=(Iterator &other){
    return other.widgetPtr != widgetPtr;
};

void Layout::Iterator::operator++(){
    ++widgetPtr;
};

Widget & Layout::Iterator::operator*(){
    return *widgetPtr;
};

Widget & Layout::Iterator::operator->(){
    return *widgetPtr;
};

Layout::Iterator Layout::begin(){
    auto obj = widgetLayouts.front();
    return {obj.widget.get()};
};

Layout::Iterator Layout::end(){
    auto obj = widgetLayouts.back();
    auto ptr = obj.widget.get();
    return {++ptr};
};



DynamicLayout::DynamicLayout(const Core::Rect & rect,std::initializer_list<WidgetLayoutDescriptor> widgetLayouts,bool implyPadding):Layout(rect,widgetLayouts,implyPadding){

};

}
