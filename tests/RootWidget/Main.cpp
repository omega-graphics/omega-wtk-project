#include "Main.h"
#include <iostream>


namespace OmegaWTK {
class MyWidget : public Widget {
    bool selected = false;
    class MyRootDelegate : public ViewDelegate {
        bool * state;
        void onLeftMouseDown(Native::NativeEventPtr event) {
//            if(!*state) {
                std::cout << "On" << std::endl;
                Composition::Layer *layer = view->getLayer();
                Composition::Style & style = layer->getStyle();
                style.setBrush(0,Composition::ColorBrush(Composition::Color(0xBB0000)));
                layer->redraw();
                *state = !*(state);
//            };
            std::cout << "State:" << *state << std::endl;
        };
        void onLeftMouseUp(Native::NativeEventPtr event) {
//            if(*state){
                std::cout << "Off" << std::endl;
                Composition::Layer *layer = view->getLayer();
                Composition::Style & style = layer->getStyle();
                style.setBrush(0,Composition::ColorBrush(Composition::Color::Red));
                layer->redraw();
                *state = !*(state);
//            };
            std::cout << "State:" << *state << std::endl;
        };
    public:
        MyRootDelegate(bool * state):state(state){};
    };
    MyRootDelegate *delegate;
public:
    MyWidget(const Core::Rect & rect):Widget(rect),delegate(new MyRootDelegate(&selected)){
        rootView->setDelegate(delegate);
        auto black = Composition::ColorBrush(Composition::Color::Black);
        using namespace Composition;
        Layer *rootLayer = rootView->getLayer();
        Style layerStyle;
        layerStyle.add(VISUAL_RECT(Rect(0,0,200,200),ColorBrush(Composition::Color::Red)),{});
//        rootLayer->setBackgroundColor(Composition::Color::Blue);
        //        rootLayer->drawRect(Rect(0,0,100,100),Composition::ColorBrush(Composition::Color::Red),Composition::Border(black,5));
        //          rootLayer->drawRoundedRect(RoundedRect(0,0,200,200,25,25),Composition::ColorBrush(Composition::Color::Green));
                //  rootLayer->drawEllipse({{150,150},30,50},Composition::Color::Green);
        //          rootLayer->drawBitmap(IMPORT_IMG("test.png"),Rect(0,0,300,300));
        //        rootLayer->drawText("Hello World!",25,Composition::ColorBrush(Composition::Color::Black),Rect(0,0,200,200));
        //        rootLayer->drawText("Hello World!",50,Composition::ColorBrush(Composition::Color::Black),Rect(0,0,300,200));
        rootLayer->setStyle(layerStyle);
    };
    
};

class MyMenuDelegate : public MenuDelegate {
public:
    void onSelectItem(unsigned int itemIndex) {
        if(itemIndex == 2){
            auto item = menu->getItemByIdx(0);
            item->disable();
            std::cout << "Disabled Item!" << std::endl;
        };
    };
    MyMenuDelegate(){};
};

};

using namespace OmegaWTK;

int omegaWTKMain(AppInst *app)
{
    auto menu = new Menu("AppMenu",{
        CategoricalMenu("File",{
            SubMenu("Inside",{
                new MenuItem("Here!",false,nullptr),
                MenuSeperatorItem(),
                new MenuItem("Test!",false,nullptr)
            },new MyMenuDelegate())
        })
    });
 app->menu = menu;
    FSPath path = Core::String("./assets/test.png");
    
    auto r_rectFrame = Composition::RoundedRectFrame(FRoundedRect(200.f,200.f,100.f,75.f,20.f,20.f),5.f);
    
//    Core::Math::VectorPath2D vectorPath ({0,0});
//    vectorPath.append({50,50});
//    vectorPath.append({100,100});
//    vectorPath.append({250,175});
//    std::cout << vectorPath.toStr();
    
    std::cout << path.serialize() << std::endl;
//    // OmegaWTK::FSPath path = OmegaWTK::Core::String("./test.png");
//    // MessageBoxA(GetForegroundWindow(),std::to_string(path.getTokenCount()).c_str(),"Token Count",MB_OK);
//    // // MessageBoxA(GetForegroundWindow(),path.debugString().c_str(),"DEBUG STRING",MB_OK);
//    // MessageBoxA(GetForegroundWindow(),path.serialize().c_str(),"RESULT",MB_OK);

    MyWidget widget({{0,0},{200,200}});
    MyWidget widget2({{300,0},{200,200}});
    widget.show();
    widget2.show();
    // OmegaWTK::MyWidget widget2({{500,0},{300,300}});
     widget2.show();
    app->addWidgetToRoot(&widget);
    app->addWidgetToRoot(&widget2);
    return 0;
}

