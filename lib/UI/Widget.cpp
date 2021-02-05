#include "omegaWTK/UI/Widget.h"

namespace OmegaWTK {

Widget::Widget(const Core::Rect & rect,Widget *parent):parent(parent),compositor(new Composition::Compositor()){
    rootView = make_view(rect,compositor);

    if(parent)
        parent->rootView->addSubView(this->rootView);
};
void Widget::show(){
    compositor->prepareDraw(rootView->layer);
};
void Widget::hide(){
    compositor->prepareCleanup();
};

Widget::~Widget(){
    // delete rootView;
    // delete compositor;
};

}
