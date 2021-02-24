#include "Main.h"
#include <iostream>


namespace OmegaWTK {
class MyWidget : public Widget {
    class MyRootDelegate : public ViewDelegate {
        void onLeftMouseDown(Native::NativeEventPtr event) {
            // Composition::Layer *layer = view->getLayer();
            // Composition::Visual *text = layer->getVisualByIdx(1);
            // text->setColor(Composition::Color::Green);
            // layer->redraw();
        };
        void onLeftMouseUp(Native::NativeEventPtr event) {
            
        };
    };
    MyRootDelegate *delegate;
public:
    MyWidget(const Core::Rect & rect):Widget(rect),delegate(new MyRootDelegate()){
        rootView->setDelegate(delegate);
        auto black = Composition::ColorBrush(Composition::Color::Black);
        Composition::Layer *rootLayer = rootView->getLayer();
//        rootLayer->setBackgroundColor(Composition::Color::Blue);
//        rootLayer->drawRect(Rect(0,0,100,100),Composition::ColorBrush(Composition::Color::Red),Composition::Border(black,5));
//          rootLayer->drawRoundedRect(RoundedRect(0,0,200,200,25,25),Composition::ColorBrush(Composition::Color::Green));
        //  rootLayer->drawEllipse({{150,150},30,50},Composition::Color::Green);
//          rootLayer->drawBitmap(Core::loadImageFromFile(Core::String("./assets/test.png")),Rect(0,0,300,300));
        rootLayer->drawText("Hello World!",25,Composition::ColorBrush(Composition::Color::Black),Rect(0,0,300,300));
        rootLayer->drawText("Hello World!",50,Composition::ColorBrush(Composition::Color::Black),Rect(0,0,300,200));
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



int omegaWTKMain(OmegaWTK::AppInst *app)
{
    auto menu = new OmegaWTK::Menu("AppMenu",{
        OmegaWTK::CategoricalMenu("File",{
            OmegaWTK::SubMenu("Inside",{
                new OmegaWTK::MenuItem("Here!",false,nullptr),
                OmegaWTK::MenuSeperatorItem(),
                new OmegaWTK::MenuItem("Test!",false,nullptr)
            },new OmegaWTK::MyMenuDelegate())
        })
    });
 app->menu = menu;
    OmegaWTK::FSPath path = OmegaWTK::Core::String("./assets/test.png");
//
    std::cout << path.serialize() << std::endl;
//    // OmegaWTK::FSPath path = OmegaWTK::Core::String("./test.png");
//    // MessageBoxA(GetForegroundWindow(),std::to_string(path.getTokenCount()).c_str(),"Token Count",MB_OK);
//    // // MessageBoxA(GetForegroundWindow(),path.debugString().c_str(),"DEBUG STRING",MB_OK);
//    // MessageBoxA(GetForegroundWindow(),path.serialize().c_str(),"RESULT",MB_OK);

    OmegaWTK::MyWidget widget({{0,0},{500,500}});
    widget.show();
    // OmegaWTK::MyWidget widget2({{500,0},{300,300}});
    // widget2.show();
    app->setRoot(&widget);
    return 0;
}

