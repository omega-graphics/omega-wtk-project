#include "OmegaWTK-C.h"

#include "OmegaWTK.h"


struct __OmegaWTKCObject {
    void *object;
};

class __OmegaWTKCWidget : public OmegaWTK::Widget {
public:
    void resize(OmegaWTK::Core::Rect &newRect){

    };
    __OmegaWTKCWidget(const OmegaWTK::Core::Rect & rect,SharedHandle<OmegaWTK::Widget> parent):OmegaWTK::Widget(rect, parent){

    };
};

extern "C" {


OmegaWTKWidget * omegawtk_construct_widget(struct OmegaWTKRect rect, OmegaWTKWidget *parent){
    __OmegaWTKCObject * o = (__OmegaWTKCObject *)malloc(sizeof(__OmegaWTKCObject));
    __OmegaWTKCObject *_parent_widget = (__OmegaWTKCObject *)parent;

    __OmegaWTKCObject _rc;
    _rc.object = new __OmegaWTKCWidget(OmegaWTK::Rect(rect.x,rect.y,rect.w,rect.h),std::shared_ptr<OmegaWTK::Widget>((OmegaWTK::Widget *)_parent_widget->object));
    memmove(o,&_rc,sizeof(__OmegaWTKCObject));
    return (OmegaWTKWidget *)o;
};

void omegawtk_free_widget(OmegaWTKWidget *widget){
    __OmegaWTKCObject *_widget = (__OmegaWTKCObject *)widget;
    OmegaWTK::Widget *object = (OmegaWTK::Widget *)_widget->object;
    delete object;
    free(_widget);
};


}