
#include <OmegaWTK.h>
#include <chrono>
#include <thread>

static SharedHandle<OmegaWTK::Composition::Brush> colorBrush;
static SharedHandle<OmegaWTK::Composition::Font> font;

class MyWidget : public OmegaWTK::Widget {
protected:
    SharedHandle<OmegaWTK::TextView> textView;
    void onThemeSet(OmegaWTK::Native::ThemeDesc & desc) override{

    }
    void init() override {
        textView->startCompositionSession();
        textView->updateFont(font);
        textView->setContent(u"Hello World");
        textView->endCompositionSession();
        textView->enable();
    }
public:
    explicit MyWidget(const OmegaWTK::Core::Rect & rect,OmegaWTK::Widget *parent) : OmegaWTK::Widget(rect,parent){
        auto layer = rootView->getLayerTreeLimb()->getRootLayer();
        textView = makeTextView(OmegaWTK::Core::Rect{{0,0},200,200},rootView.get());
    }
};

class MyWindowDelegate : public OmegaWTK::AppWindowDelegate {

public:
    MyWindowDelegate(){

    }
    void windowWillClose(OmegaWTK::Native::NativeEventPtr event) override {
        std::cout << "Closing" << std::endl;
        OmegaWTK::AppInst::inst()->terminate();
    }
};

int omegaWTKMain(OmegaWTK::AppInst *app){
    OmegaWTK::Composition::FontDescriptor font_desc("Arial",15);
    font = OmegaWTK::Composition::FontEngine::inst()->CreateFont(font_desc);

    colorBrush = OmegaWTK::Composition::ColorBrush(OmegaWTK::Composition::Color::create8Bit(OmegaWTK::Composition::Color::Green8));
    
 
    // img = OmegaWTK::Media::loadImageFromFile("./test.png").getValue();
    // std::this_thread::sleep_for(std::chrono::seconds(35));
    
    OmegaCommon::LogV("Hello World @{0}",colorBrush);

    auto window = make<OmegaWTK::AppWindow>(OmegaWTK::Core::Rect {{0,0},500,500},new MyWindowDelegate());

    auto menu = make<OmegaWTK::Menu>(OmegaWTK::Menu("MyMenu",{
        OmegaWTK::CategoricalMenu("BasicAppTest",{
            OmegaWTK::ButtonMenuItem("Hello World!")
        })
    }));

    auto widget = make<MyWidget>(OmegaWTK::Core::Rect {{0,0},400,400},nullptr);

    window->add(widget);
    window->setMenu(menu);


    auto & windowManager = app->windowManager;

    windowManager->setRootWindow(window);
    windowManager->displayRootWindow();

    return OmegaWTK::AppInst::start();
};