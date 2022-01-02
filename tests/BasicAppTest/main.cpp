
#include <OmegaWTK.h>

static SharedHandle<OmegaWTK::Composition::Brush> colorBrush;

class MyWidget : public OmegaWTK::Widget {
    SharedHandle<OmegaWTK::Composition::Canvas> canvas;
protected:
    void onThemeSet(OmegaWTK::Native::ThemeDesc & desc) override{

    }
    void init() override {
        rootView->startCompositionSession();
        canvas->drawRect(this->rect(),colorBrush);
        canvas->sendFrame();
        rootView->endCompositionSession();
    }
public:
    explicit MyWidget(const OmegaWTK::Core::Rect & rect,OmegaWTK::Widget *parent) : OmegaWTK::Widget(rect,parent){
        auto layer = rootView->getLayerTreeLimb()->getRootLayer();
        canvas = rootView->makeCanvas(layer);
    }
};

class MyWindowDelegate : public OmegaWTK::AppWindowDelegate {

public:
    MyWindowDelegate(){

    }
    void windowWillClose(OmegaWTK::Native::NativeEventPtr event) override {
        OmegaWTK::AppInst::inst()->terminate();
    }
};

int omegaWTKMain(OmegaWTK::AppInst *app){

    colorBrush = OmegaWTK::Composition::ColorBrush(OmegaWTK::Composition::Color::create8Bit(OmegaWTK::Composition::Color::Red8,0xFF));

    OmegaWTK::AppWindow window(OmegaWTK::Core::Rect {{0,0},500,500},new MyWindowDelegate());

    auto menu = make<OmegaWTK::Menu>(OmegaWTK::Menu("MyMenu",{
        OmegaWTK::CategoricalMenu("BasicAppTest",{
            OmegaWTK::ButtonMenuItem("Hello World!")
        })
    }));

    MyWidget widget(OmegaWTK::Core::Rect {{0,0},200,200},nullptr);

    window.add(&widget);
    window.setMenu(menu);


    auto & windowManager = app->windowManager;

    windowManager->setRootWindow(&window);
    windowManager->displayRootWindow();

    return OmegaWTK::AppInst::start();
};