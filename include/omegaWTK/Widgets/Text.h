#include "omegaWTK/UI/Widget.h"
#include "omegaWTK/Core/Unicode.h"

#ifndef OMEGAWTK_UI_TEXTINPUT_H
#define OMEGAWTK_UI_TEXTINPUT_H

namespace OmegaWTK::UI {
    
    /**
     Defines one or more lines of text layed out in a rect.
     */
    class OMEGAWTK_EXPORT TextWidget : public Widget {
        OmegaWTK::UniString textStr;
        SharedHandle<Composition::Font> currentfont;
        SharedHandle<Composition::TextRect> textRect;
        void render();
    public:
        TextWidget(const Core::Rect & rect,Widget *parent,SharedHandle<Composition::Font> & currentfont);

        void setText(const OmegaCommon::String & textStr);

        void setText(const OmegaCommon::WString & textStr);
        
        void setText(const OmegaCommon::UString & textStr);
        
        void setFont(SharedHandle<Composition::Font> & font);
        void resize(Core::Rect &newRect);
    };

    class OMEGAWTK_EXPORT TextInput : public Widget {
        OmegaCommon::String textStr;
        OmegaCommon::String placeholder;
        SharedHandle<Composition::TextRect> textRect;

        SharedHandle<ViewDelegate> delegate;

        SharedHandle<WidgetState<bool>> canRecieveInput;

        SharedHandle<Composition::Font> currentfont;

        // void pushChar(char32_t c);
        friend class UserInputDelegate;
    public:
        OmegaCommon::String & string();
        void resize(Core::Rect &newRect);
        void setFont(SharedHandle<Composition::Font> & font);
        TextInput(const Core::Rect & rect,Widget *parent,SharedHandle<Composition::Font> currentfont,OmegaCommon::String placeholder = "Input Text Here...");
    };
};

#endif
