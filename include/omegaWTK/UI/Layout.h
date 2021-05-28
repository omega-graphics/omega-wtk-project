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
    /// Describes a Widget's relative size, position, and resizing paramters in a Layout.
    struct OMEGAWTK_EXPORT WidgetLayoutDescriptor {
        /// The widget.
        SharedHandle<Widget> widget;
        /// The widget's size to Layout rect ratio.
        float ratio;
        /**
         Depending on what direction is specified in this descriptor, 
         this parameter is how big to intially size the widget's rect height or width.
        */
        Core::Optional<unsigned> dimen;
        /// The parameter to set for dimen
        bool verticalOrHorizontal;
    public:
        WidgetLayoutDescriptor(SharedHandle<Widget> widget,float ratio,Core::Optional<unsigned> dimen = {});
    };
    
    class OMEGAWTK_EXPORT Layout {
        Core::Rect layoutRect;
        Core::Vector<WidgetLayoutDescriptor> widgetLayouts;
    public:
        struct Iterator {
            Widget *widgetPtr;
        public:
            Widget &operator*();
            Widget &operator->();
            void operator++();
            bool operator!=(Iterator & other);
        };

        Iterator begin();
        Iterator end();

        Layout(const Core::Rect & rect,std::initializer_list<WidgetLayoutDescriptor> widgetLayouts,bool implyPadding = false);
    };

    typedef Layout StaticLayout;

    class OMEGAWTK_EXPORT DynamicLayout : public Layout {
    public:
        DynamicLayout(const Core::Rect & initRect,std::initializer_list<WidgetLayoutDescriptor> widgetLayouts,bool implyPadding = false);
        void resizeLayout(const Core::Rect & newRect);
    };
};

#endif
