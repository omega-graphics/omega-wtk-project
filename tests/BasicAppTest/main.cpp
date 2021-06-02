#include <OmegaWTK.h>

using namespace OmegaWTK;

class MyWindowDelegate : public AppWindowDelegate {
    AppInst *app;
    SharedHandle<Widget> panel;
public:
    MyWindowDelegate(AppInst *app,SharedHandle<Widget> panel):app(app),panel(panel){};
    void windowWillClose(Native::NativeEventPtr event){
        // panel->hide();
    };
    void windowWillResize(Core::Rect &nRect) {
        std::cout << "NEW RECT: x:" << nRect.pos.x << " y:" << nRect.pos.y << " w:" << nRect.dimen.minWidth << " h:" << nRect.dimen.minHeight << std::endl;
        auto & widgetRect = panel->rect();
        widgetRect.dimen.minHeight = nRect.dimen.minHeight;
        panel->resize(widgetRect);
        std::cout << "Widget did Resize" << std::endl;
        panel->refresh();
    };
};

// Core::SharedPtr<Composition::Font> f;
// Core::String val = "Hello World";
// Core::SharedPtr<Composition::TextRect> textRect;

int omegaWTKMain(AppInst *app){
    // Composition::FontDescriptor fontDesc("Arial",20);
    // f = Composition::FontEngine::instance->CreateFont(fontDesc);
    // std::cout << "Hello World" << std::endl;
    auto widgetTreeHost = WidgetTreeHost::Create();

    auto panel = widgetTreeHost->makeWidget<UI::Panel>(OmegaWTK::Rect(0,0,500,1000));

    panel->show();

    auto appWindow = make<AppWindow>(OmegaWTK::Rect(0,0,1000,1000),new MyWindowDelegate(app,panel));
    
    widgetTreeHost->attachToWindow(appWindow);

    app->windowManager->setRootWindow(appWindow);
    app->windowManager->displayRootWindow();

    return 0;
};