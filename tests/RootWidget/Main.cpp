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

    std::cout << "Hello World" << std::endl;

    brush = Composition::ColorBrush(Composition::Color::Green);

    font = Composition::FontEngine::instance->CreateFont(desc);

    AppWindow window (Core::Rect {Core::Position {0,0},500,500});

    
    app->windowManager->setRootWindow(&window);
    app->windowManager->displayRootWindow();
     
    return AppInst::start();
};
