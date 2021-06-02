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
        WIDGET_CONSTRUCTOR(TextWidget,SharedHandle<Composition::Font> & currentfont);
        void setFont(SharedHandle<Composition::Font> & font);
        void resize(Core::Rect &newRect);
    };

    class OMEGAWTK_EXPORT TextInput : public Widget {
        Core::String textStr;
        Core::String placeholder;
        SharedHandle<Composition::TextRect> textRect;

        SharedHandle<ViewDelegate> delegate;

        SharedHandle<WidgetState<bool>> canRecieveInput;

        SharedHandle<Composition::Font> currentfont;

        void pushChar(UniChar16 c);
        friend class UserInputDelegate;
    public:
        Core::String & string();
        void resize(Core::Rect &newRect);
        void setFont(SharedHandle<Composition::Font> & font);
        WIDGET_CONSTRUCTOR(TextInput,SharedHandle<Composition::Font> currentfont,Core::String placeholder = "Input Text Here...");
    };
};

#endif
