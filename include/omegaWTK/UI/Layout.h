#include "Widget.h"

#ifndef OMEGAWTK_UI_LAYOUT_H
#define OMEGAWTK_UI_LAYOUT_H

namespace OmegaWTK {

    enum class LayoutStyle : OPT_PARAM {
        Vertical,
        Horizontal,
        VerticalWithWrap,
        HorizontalWithWrap
    };

    struct OMEGAWTK_EXPORT WidgetLayoutDescriptor {
    private:
        SharedHandle<Widget> widget;
        float ratio;
        Core::Optional<unsigned> height;
    public:
        WidgetLayoutDescriptor(SharedHandle<Widget> widget,float ratio,Core::Optional<unsigned> height = {});
    };
    
    class OMEGAWTK_EXPORT StaticLayout {
        Core::Rect layoutRect;
        Core::Vector<WidgetLayoutDescriptor> widgetLayouts;
    public:
        StaticLayout(const Core::Rect & rect,std::initializer_list<WidgetLayoutDescriptor> widgetLayouts,bool implyPadding = false);
    };

    class OMEGAWTK_EXPORT DynamicLayout {
        Core::Rect layoutRect;
        Core::Vector<WidgetLayoutDescriptor> widgetLayouts;
    public:
        DynamicLayout(const Core::Rect & initRect,std::initializer_list<WidgetLayoutDescriptor> widgetLayouts,bool implyPadding = false);
        void resizeLayout(const Core::Rect & newRect);
    };
};

#endif
