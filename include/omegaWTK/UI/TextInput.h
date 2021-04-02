#include "Widget.h"

#ifndef OMEGAWTK_UI_TEXTINPUT_H
#define OMEGAWTK_UI_TEXTINPUT_H

namespace OmegaWTK::UI {
    class OMEGAWTK_EXPORT  TextInput : public Widget {
        Core::String textStr;
        Core::String placeholder;
    public:
        Core::String & string();
        TextInput(const Core::Rect & rect,Core::String placeholder = "Input Text Here...",SharedHandle<Widget> parent = nullptr);
    };
};

#endif