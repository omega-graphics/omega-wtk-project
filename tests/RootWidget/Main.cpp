#include "Main.h"
#include <iostream>

namespace OmegaWTK {
class MyWidget : public Widget {
    class MyRootDelegate : public ViewDelegate {
        void onLeftMouseDown(Native::NativeEventPtr event) {
            Composition::Layer *layer = view->getLayer();
            Composition::Visual *text = layer->getVisualByIdx(1);
            text->setColor(Composition::Color::Green);
            layer->redraw();
        };
        void onLeftMouseUp(Native::NativeEventPtr event) {
            
        };
    };
    MyRootDelegate *delegate;
public:
    MyWidget(const Core::Rect & rect):Widget(rect),delegate(new MyRootDelegate()){
        rootView->setDelegate(delegate);
        Composition::Layer *rootLayer = rootView->getLayer();
        /// ID: 0
        rootLayer->drawRect(rect,Composition::Color::Red);
        /// ID: 1
        rootLayer->drawText("Hello World",20,Composition::Color::Black,rect,{"Times New Roman",Composition::Text::Font::Regular});
        
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
                new OmegaWTK::MenuItem(),
                new OmegaWTK::MenuItem("Test!",false,nullptr)
            },new OmegaWTK::MyMenuDelegate())
        })
    });
 app->menu = menu;
    
    OmegaWTK::MyWidget widget({{0,0},{100,100}});
    widget.show();
    app->setRoot(&widget);
    return 0;
}

