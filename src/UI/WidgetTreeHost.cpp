#include "omegaWTK/UI/WidgetTreeHost.h"
#include "../Composition/Compositor.h"

namespace OmegaWTK {
    WidgetTreeHost::WidgetTreeHost():
    compositor(new Composition::Compositor()),attachedToWindow(false)
    {

    };

    WidgetTreeHost::~WidgetTreeHost(){
        delete compositor;
    };

    WidgetTreeHost * WidgetTreeHost::Create(){
        return new WidgetTreeHost();
    };

    void WidgetTreeHost::attachToWindow(AppWindow * window){
        if(!attachedToWindow) {
            attachedToWindow = true;
            window->_add_widget(root);
            window->widgetTreeHosts.emplace_back(this);
            if(window->widgetTreeHosts.size() == 1)
                window->setFrontendPtr(compositor);
        }
    };

    void WidgetTreeHost::setRoot(Widget * widget){
        root = widget;
    };
};