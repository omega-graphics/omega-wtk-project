#include "omegaWTK/UI/Widget.h"

namespace OmegaWTK {

Widget::Widget(const Core::Rect & rect,Widget *parent):rootView(make_view(rect)),compositor(new Composition::Compositor(rootView->layer)),parent(parent){
    if(parent)
        parent->rootView->addSubView(this->rootView);
};
void Widget::show(){
    compositor->prepareDraw();
};
void Widget::hide(){
    compositor->prepareCleanup();
};

Widget::~Widget(){
    delete rootView;
    delete compositor;
};

}