#include "Main.h"
#include <iostream>

namespace OmegaWTK {
class MyWidget : public Widget {
public:
    MyWidget(const Core::Rect & rect):Widget(rect){
        Composition::Layer *rootLayer = rootView->getLayer();
        rootLayer->drawRect(rect,{0,255,0,255});
        std::cout << "Draw Rect" << std::endl;
    };
};
};


int omegaWTKMain(OmegaWTK::AppInst *app)
{
    OmegaWTK::MyWidget widget({{0,0},{100,100}});
    widget.show();
    app->setRoot(&widget);
}

