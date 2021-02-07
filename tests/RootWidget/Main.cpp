#include "Main.h"
#include <iostream>

namespace OmegaWTK {
class MyWidget : public Widget {
    class MyRootDelegate : public ViewDelegate {
        void onLeftMouseDown(Native::NativeEventPtr event) {
            Composition::Layer *layer = view->getLayer();
            auto & rect_visual = layer->getTargetVisuals()[0];
            Core::Rect rc = {{0,0},{100,100}};
            
            delete (Composition::Visual::RectParams *)rect_visual->params;
            rect_visual->params = new Composition::Visual::RectParams({rc,{Composition::Color::Red}});
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
        rootLayer->drawRect(rect,{Composition::Color::Green});
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

