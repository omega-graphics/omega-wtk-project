#include "omegaWTK/UI/WidgetTreeHost.h"
#include "../Composition/Compositor.h"

namespace OmegaWTK {
    WidgetTreeHost::WidgetTreeHost():
    compositor(new Composition::Compositor()),attachedToWindow(false)
    {

    };

    WidgetTreeHost::~WidgetTreeHost(){
        
    };

    WidgetTreeHost * WidgetTreeHost::Create(){
        return new WidgetTreeHost();
    };

    void WidgetTreeHost::attachToWindow(SharedHandle<AppWindow> & window){
        if(!attachedToWindow) {
            attachedToWindow = true;
            window->_add_widget(&root);
            window->widgetTreeHosts.emplace_back(this);
            if(window->widgetTreeHosts.size() == 1)
                window->setFrontendPtr(compositor.get());
        }
    };

    void WidgetTreeHost::setRoot(SharedHandle<Widget> widget){
        root = widget;
    };
};