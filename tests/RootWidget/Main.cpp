#include <OmegaWTK.h>

using namespace OmegaWTK;

static SharedHandle<Composition::Font> font;

int omegaWTKMain(OmegaWTK::AppInst *app){
    Composition::FontDescriptor desc ("Arial",20);

    font = Composition::FontEngine::instance->CreateFont(desc);

    auto window = make<AppWindow>(Core::Rect {Core::Position {0,0},500,500});

    auto treeHost = WidgetTreeHost::Create();

    auto textWidget = treeHost->makeWidget<UI::TextWidget>(Core::Rect {Core::Position {0,0},200,50},nullptr,font);
    textWidget->setText(U"Hello World!");
    textWidget->show();

    treeHost->setRoot(textWidget);

    treeHost->attachToWindow(window);

    app->windowManager->setRootWindow(window);
    app->windowManager->displayRootWindow();
     
    return 0;
};
