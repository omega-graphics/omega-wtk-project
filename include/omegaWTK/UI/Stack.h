#include "Widget.h"

#ifndef OMEGAWTK_UI_STACK_H
#define OMEGAWTK_UI_STACK_H

namespace OmegaWTK::UI {
    class VStack : public Widget {
        Core::Vector<SharedHandle<Widget>> childWidgets;
    public:
        void addChildWidget(SharedHandle<Widget> widget);
        void removeChildAtIndex(unsigned idx);
    };

    class HStack : public Widget {
        Core::Vector<SharedHandle<Widget>> childWidgets;
    public:
        void addChildWidget(SharedHandle<Widget> widget);
        void removeChildAtIndex(unsigned idx);
    };
};

#endif