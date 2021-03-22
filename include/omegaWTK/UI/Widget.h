#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Composition/Compositor.h"
#include "View.h"

#ifndef OMEGAWTK_UI_WIDGET_H
#define OMEGAWTK_UI_WIDGET_H

namespace OmegaWTK {

class AppWindow;
class AppWindowManager;
class VideoView;

OMEGAWTK_EXPORT class Widget {
protected:
    SharedHandle<CanvasView> rootView;
    SharedHandle<Widget> parent;
    SharedHandle<Composition::LayerTree> layerTree;
    /**
     Constructs a Layer with Widget's Compositor Pointer and Core::Rect
     */
    SharedHandle<Composition::Layer> makeLayer(const Core::Rect & rect);
    Composition::Compositor * compositor;
private:
    /**
     Makes a Canvas View attached to this widget and returns it.
     @param rect The Rectangle to use
     @param parent The Parent View (NOTE: This view MUST be within this widget's view heirarchy)
     @returns A standard View
     */
    SharedHandle<CanvasView> makeCanvasView(const Core::Rect & rect,View *parent);
    /**
     Makes a Video View attached to this widget and returns it.
     @param rect The Rectangle to use
     @param parent The Parent View (NOTE: This view MUST be within this widget's view heirarchy)
     @returns A Video View
     */
//    SharedHandle<VideoView> makeVideoView(const Core::Rect & rect,View *parent);
    
    friend class AppWindow;
    friend class AppWindowManager;
public:
    void setParentView(View *view);
    void show();
    void hide();
    Widget(const Core::Rect & rect,SharedHandle<Widget> parent);
//    Widget(Widget &widget);
    ~Widget();
};

};

#endif
