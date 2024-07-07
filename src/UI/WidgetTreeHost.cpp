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
            initWidgetRecurse(child.get());
        }
    }

    void WidgetTreeHost::initWidgetTree(){
        root->setTreeHostRecurse(this);
        initWidgetRecurse(root.get());
    }

    void WidgetTreeHost::attachToWindow(AppWindow * window){
        if(!attachedToWindow) {
            attachedToWindow = true;
            window->_add_widget(root.get());
            if(window->widgetTreeHosts.size() == 1)
                window->proxy.setFrontendPtr(compositor);
        }
    };

    void WidgetTreeHost::setRoot(WidgetPtr widget){
        root = widget;
    };
};