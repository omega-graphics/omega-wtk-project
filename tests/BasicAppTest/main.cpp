#include <OmegaWTK.h>

using namespace OmegaWTK;

class MyWindowDelegate : public AppWindowDelegate {
    AppInst *app;
public:
    MyWindowDelegate(AppInst *app):app(app){};
    void windowWillClose(Native::NativeEventPtr event){
        app->terminate();
    };
};

Core::SharedPtr<Composition::Font> f;
Core::String val = "Hello World";
Core::SharedPtr<Composition::TextRect> textRect;

int omegaWTKMain(AppInst *app){
    Composition::FontDescriptor fontDesc("Arial",20);
    f = Composition::FontEngine::instance->CreateFont(fontDesc);
    textRect = Composition::TextRect::Create(val,f,OmegaWTK::Rect(75,75,375,375));
    std::cout << "Hello World" << std::endl;
    
    auto panel = make<UI::Panel>(OmegaWTK::Rect(0,0,500,500));
    auto layerStyle = make<Composition::LayerStyle>();
    auto b = Composition::ColorBrush(0xCCCCCC);
    auto black = Composition::ColorBrush(Composition::Color::White);
    layerStyle->setBackgroundColor(Composition::Color::Transparent);
    layerStyle->add(VISUAL_ROUNDED_RECT_W_FRAME(OmegaWTK::RoundedRect(50,50,400,400,25,25),Composition::ColorBrush(Composition::Color::White),Composition::Border(b,2)));
    layerStyle->add(VISUAL_TEXT(textRect,black));
    auto shadow = make<Composition::LayerEffect>(LAYER_EFFECT_DROPSHADOW(-5.0,-5.0,20,1.0,1.0,Composition::Color::Black));
    layerStyle->addEffect(shadow);
    panel->setStyle(layerStyle);

    panel->show();

    auto appWindow = make<AppWindow>(OmegaWTK::Rect(0,0,1000,1000),new MyWindowDelegate(app));
    appWindow->addWidget(panel);
    app->windowManager->setRootWindow(appWindow);
    app->windowManager->displayRootWindow();

    return 0;
};