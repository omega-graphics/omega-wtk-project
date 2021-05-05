#include "Widget.h"

#ifndef OMEGAWTK_UI_TEXTINPUT_H
#define OMEGAWTK_UI_TEXTINPUT_H

namespace OmegaWTK::UI {
    
    /**
     Defines one or more lines of text layed out in a rect.
     */
    class OMEGAWTK_EXPORT TextWidget : public Widget {
        Core::String textStr;
        SharedHandle<Composition::Font> currentfont;
    public:
        TextWidget(const Core::Rect & rect,Core::String textStr,SharedHandle<Widget> parent = nullptr);
        void setFont(SharedHandle<Composition::Font> & font);
        void resize(Core::Rect &newRect);
    };

    class OMEGAWTK_EXPORT TextInput : public Widget {
        Core::String textStr;
        Core::String placeholder;
        SharedHandle<Composition::TextRect> textRect;
    public:
        Core::String & string();
        void resize(Core::Rect &newRect);
        TextInput(const Core::Rect & rect,Core::String placeholder = "Input Text Here...",SharedHandle<Widget> parent = nullptr);
    };
};

#endif
