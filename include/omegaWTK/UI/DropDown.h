#include "Widget.h"

#ifndef OMEGAWTK_UI_DROPDOWN_H
#define OMEGAWTK_UI_DROPDOWN_H

namespace OmegaWTK::UI {

    class OMEGAWTK_EXPORT DropDown : public Widget {
        Core::Vector<SharedHandle<Widget>> childWidgets;
    public:
        void addChildWidget(SharedHandle<Widget> & widget);
    };
};

#endif