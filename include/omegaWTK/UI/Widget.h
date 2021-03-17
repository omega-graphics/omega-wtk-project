#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Composition/Compositor.h"
#include "View.h"

#ifndef OMEGAWTK_UI_WIDGET_H
#define OMEGAWTK_UI_WIDGET_H

namespace OmegaWTK {

class AppWindow;
class AppWindowManager;

class OMEGAWTK_EXPORT Widget {
protected:
    SharedHandle<View> rootView;
    SharedHandle<Widget> parent;
    SharedHandle<Composition::LayerTree> layerTree;
    /**
     Constructs a Layer with Widget's Compositor Pointer and Core::Rect
     */
    SharedHandle<Composition::Layer> makeLayer(const Core::Rect & rect);
    Composition::Compositor * compositor;
private:
    
    friend class AppWindow;
    friend class AppWindowManager;
public:
    void setParentView(View *view);
    void show();
    void hide();
    Widget(const Core::Rect & rect,SharedHandle<Widget> parent = nullptr);
//    Widget(Widget &widget);
    ~Widget();
};

};

#endif
