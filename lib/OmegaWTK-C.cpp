#include "OmegaWTK-C.h"

#include "OmegaWTK.h"



struct __OmegaWTKCObject {
    void *object;
};

#define COBJECT_STD_MALLOC(o) __OmegaWTKCObject * o = (__OmegaWTKCObject *)malloc(sizeof(__OmegaWTKCObject))
#define COBJECT_STD_INIT(o,desc) memmove(o,&desc,sizeof(__OmegaWTKCObject))
#define COBJECT_STD_FREE(o) free(o)

// int omegaWTKMain(OmegaWTK::AppInst *app){
//     COBJECT_STD_MALLOC(obj);
//     __OmegaWTKCObject desc;
//     desc.object = app;
//     COBJECT_STD_INIT(obj,desc);

//     int rc = omegawtk_main((OmegaWTKAppInst *)obj);
//     COBJECT_STD_FREE(obj);
//     return rc;
// };

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