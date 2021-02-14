#include "omegaWTK/UI/Widget.h"

namespace OmegaWTK {

Widget::Widget(const Core::Rect & rect,Widget *parent):parent(parent),compositor(new Composition::Compositor()){
    rootView = make_view(rect,compositor);

    if(parent)
        parent->rootView->addSubView(this->rootView);

    compositor->prepareDraw(rootView->layer);
};
void Widget::show(){
    rootView->native->enable();
};
void Widget::hide(){
   rootView->native->disable();
};

Widget::~Widget(){
    //  compositor->prepareCleanup();
};

}
