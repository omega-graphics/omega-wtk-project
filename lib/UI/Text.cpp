#include "omegaWTK/UI/Text.h"

namespace OmegaWTK::UI {

    void TextWidget::resize(Core::Rect &newRect){
        
    };


    Core::String & TextInput::string(){
        return textStr;
    };

    class UserInputDelegate : public ViewDelegate {
        TextInput *widgetPtr;
        Composition::TextRect *textRect;
    public:
        UserInputDelegate(Composition::TextRect *textRect,TextInput *widgetPtr);
        void onKeyDown(Native::NativeEventPtr event);
        void onKeyUp(Native::NativeEventPtr event);
    };

    UserInputDelegate::UserInputDelegate(Composition::TextRect *textRect,TextInput *widgetPtr):textRect(textRect),widgetPtr(widgetPtr){

    };

    void UserInputDelegate::onKeyDown(Native::NativeEventPtr event){
        Native::KeyDownParams *params = (Native::KeyDownParams *)event->params;
        widgetPtr->pushChar(params->key);
    };

    void UserInputDelegate::onKeyUp(Native::NativeEventPtr event){

    };

    
    TextInput::TextInput(const Core::Rect & rect,Core::String placeholder,SharedHandle<Widget> parent):Widget(rect,parent),textStr(""),placeholder(placeholder){
        canRecieveInput = WidgetState<bool>::Create(false);
        textRect = Composition::TextRect::Create(textStr,currentfont,rect);
        /// Assume default style.
        auto rootLayer = rootView->getLayerTreeLimb()->getRootLayer();
        auto style = make<Composition::LayerStyle>();
        style->setBackgroundColor(Composition::Color::Transparent);
        style->add(VISUAL_RECT(Rect(0,0,rect.dimen.minWidth,rect.dimen.minHeight),Composition::ColorBrush(Composition::Color::Red)));
        style->add(VISUAL_TEXT(textRect,Composition::ColorBrush(Composition::Color::Black)));
        rootLayer->setStyle(style);
        delegate = std::shared_ptr<ViewDelegate>(new UserInputDelegate(textRect.get(),this));
    };

    void TextInput::setFont(SharedHandle<Composition::Font> &font){
        currentfont = font;
    };

    void TextInput::pushChar(UniChar c){
        auto & temp = textRect->getString();
        temp += char(c);
        refresh();
    };

    void TextInput::resize(Core::Rect &newRect){

    };
};
