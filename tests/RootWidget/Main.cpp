#include <OmegaWTK.h>

using namespace OmegaWTK;


static OmegaWTK::Core::SharedPtr<Composition::Brush> brush;
static SharedHandle<Composition::Font> font;

class RectWidget : public Widget {
public:
    RectWidget(const Core::Rect &rect,Widget *parent):Widget(rect,parent){
        
    };

    void render() override {
        auto & surface = rootView->getLayerTreeLimb()->getRootLayer()->getSurface();
        surface->drawRect(rect(),brush);
        rootView->commitRender();
    };

};

int omegaWTKMain(OmegaWTK::AppInst *app){
    Composition::FontDescriptor desc ("Arial",20);

    brush = Composition::ColorBrush(Composition::Color::Green);

    font = Composition::FontEngine::instance->CreateFont(desc);

    AppWindow window (Core::Rect {Core::Position {0,0},500,500});

    // auto treeHost = WidgetTreeHost::Create();

    // auto rectWidget = treeHost->makeWidget<RectWidget>(Core::Rect {Core::Position {0,0},100,100});

    // rectWidget->show();

    // treeHost->setRoot(rectWidget);

    // treeHost->attachToWindow(window);
    app->windowManager->setRootWindow(&window);
    app->windowManager->displayRootWindow();
     
    return AppInst::start();
};
