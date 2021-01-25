
#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Native/NativeEvent.h"

#ifndef OMEGAWTK_WIDGETS_CLICKABLE_H
#define OMEGAWTK_WIDGETS_CLICKABLE_H

namespace OmegaWTK {

    class Clickable {
    public:
        Clickable();
        virtual void onHover(Native::NativeEventCallback);
        virtual void onClick(Native::NativeEventCallback);
        virtual ~Clickable();
    };
};

#endif
