#include "omegaWTK/UI/WidgetTreeHost.h"
#include "../Composition/Compositor.h"

#include "omegaWTK/UI/Widget.h"
#include "omegaWTK/UI/AppWindow.h"

namespace OmegaWTK {
    WidgetTreeHost::WidgetTreeHost():
    compositor(new Composition::Compositor()),attachedToWindow(false)
    {

    };

    WidgetTreeHost::~WidgetTreeHost(){
        delete compositor;
    };

    SharedHandle<WidgetTreeHost> WidgetTreeHost::Create(){
        return SharedHandle<WidgetTreeHost>(new WidgetTreeHost());
    };

    void WidgetTreeHost::initWidgetRecurse(Widget *parent){
        parent->init();
        for(auto & child : parent->children){
            initWidgetRecurse(child);
        }
    }

    void WidgetTreeHost::initWidgetTree(){
        root->setTreeHostRecurse(this);
        initWidgetRecurse(root);
    }

    void WidgetTreeHost::attachToWindow(AppWindow * window){
        if(!attachedToWindow) {
            attachedToWindow = true;
            window->_add_widget(root);
            if(window->widgetTreeHosts.size() == 1)
                window->proxy.setFrontendPtr(compositor);
        }
    };

    void WidgetTreeHost::setRoot(Widget * widget){
        root = widget;
    };
};