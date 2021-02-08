#include "Main.h"
#include <iostream>

namespace OmegaWTK {
class MyWidget : public Widget {
    class MyRootDelegate : public ViewDelegate {
        void onLeftMouseDown(Native::NativeEventPtr event) {
             
        };
        void onLeftMouseUp(Native::NativeEventPtr event) {
            
        };
    };
    MyRootDelegate *delegate;
public:
    MyWidget(const Core::Rect & rect):Widget(rect),delegate(new MyRootDelegate()){
        rootView->setDelegate(delegate);
        Composition::Layer *rootLayer = rootView->getLayer();
    //    rootLayer->drawRect(rect,{Composition::Color::Red});
        rootLayer->drawText("Hello World",20,Composition::Color::Black,rect,{"Arial",Composition::Text::Font::Regular});
        
    };
    
};
};


int omegaWTKMain(OmegaWTK::AppInst *app)
{
    OmegaWTK::MyWidget widget({{0,0},{100,100}});
    widget.show();
    app->setRoot(&widget);
    return 0;
}

