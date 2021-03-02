#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Composition/Compositor.h"
#include "View.h"

#ifndef OMEGAWTK_WIDGETS_WIDGET_H
#define OMEGAWTK_WIDGETS_WIDGET_H

namespace OmegaWTK {

class AppWindow;
class AppWindowManager;

class OMEGAWTK_EXPORT Widget {
protected:
    View *rootView;
    Widget *parent = nullptr;
private:
    Composition::Compositor *compositor;
    friend class AppWindow;
    friend class AppWindowManager;
public:
    void show();
    void hide();
    Widget(const Core::Rect & rect,Widget *parent = nullptr);
    ~Widget();
};

};

#endif
