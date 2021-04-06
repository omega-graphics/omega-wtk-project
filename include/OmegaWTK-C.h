/**
  @file OmegaWTK-C.h
  The C Language Based bindings from C++.
 
  NOTE: This is a C ONLY file.
 */

#ifndef OMEGAWTK_C_LIB_H
#define OMEGAWTK_C_LIB_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 Typedefs!
 */

/// Core

#define OPT_PARAM unsigned char
#define ENUM typedef enum

struct OmegaWTKRect {
    unsigned x,y,w,h;
};

struct OmegaWTKRoundedRect {
    unsigned x,y,w,h,rad_x,rad_y;
};

struct OmegaWTKFRect {
    float x,y,w,h;
};

struct OmegaWTKFRoundedRect {
    float x,y,w,h,rad_x,rad_y;
};

struct OmegaWTKRect omegawtk_make_rect(unsigned x,unsigned y,unsigned width,unsigned height);
struct OmegaWTKFRect omegawtk_make_f_rect(float x,float y,float width,float height);
struct OmegaWTKRoundedRect omegawtk_make_rounded_rect(unsigned x,unsigned y,unsigned width,unsigned height,unsigned radius_x,unsigned radius_y);
struct OmegaWTKFRoundedRect omegawtk_make_f_rounded_rect(float x,float y,float width,float height,float radius_x,float radius_y);

/// Native


/// Composition

typedef struct  __OmegaWTKCompLayer OmegaWTKCompLayer;
typedef struct  __OmegaWTKCompLayerStyle OmegaWTKCompLayerStyle;
typedef struct  __OmegaWTKCompLayerTree OmegaWTKCompLayerTree;
typedef struct  __OmegaWTKCompLayerTreeLimb OmegaWTKCompLayerTreeLimb;
typedef struct  __OmegaWTKCompBrush OmegaWTKCompBrush;
typedef struct  __OmegaWTKCompColor OmegaWTKCompColor;
typedef struct  __OmegaWTKCOmpGradient OmegaWTKCompGradient;
typedef struct  __OmegaWTKCompFont OmegaWTKCompFont;

ENUM : OPT_PARAM {
    RectVisual = 0x00,
    RoundedRectVisual = 0x01,
    EllipseVisual = 0x02,
    ImageVisual = 0x03,
    TextVisual = 0x04
} OmegaWTKCompVisualType;

struct OmegaWTKVisualRectParams {
    struct OmegaWTKRect rect;
    OmegaWTKCompBrush *brush;
};

struct OmegaWTKVisualRoundedRectParams {
    struct OmegaWTKRoundedRect rect;
    OmegaWTKCompBrush *brush;
};

// struct OmegaWTKVisualEllipseParams {
    
// };

struct OmegaWTKVisualImageParams {
    struct OmegaWTKRect rect;
    OmegaWTKCompBrush *brush;
};

struct OmegaWTKVisualTextParams {
    struct OmegaWTKRect textRect;
    OmegaWTKCompFont *font;
    OmegaWTKCompBrush *brush;
};

/// UI
typedef struct  __OmegaWTKAppInst OmegaWTKAppInst;
typedef struct  __OmegaWTKView OmegaWTKView;
typedef struct  __OmegaWTKViewDelegate OmegaWTKViewDelegate;
typedef struct  __OmegaWTKScrollView OmegaWTKScrollView;
typedef struct  __OmegaWTKScrollViewDelegate OmegaWTKScrollViewDelegate;
typedef OmegaWTKView OmegaWTKCanvasView;
typedef struct  __OmegaWTKWidget OmegaWTKWidget;
typedef struct  __OmegaWTKWidgetObserver OmegaWTKWidgetObserver;

/// OmegaWTK::Composition::Brush
OmegaWTKCompBrush *omegawtk_color_brush(OmegaWTKCompColor *init_color);
void omegawtk_color_brush_set_color(OmegaWTKCompBrush *brush,OmegaWTKCompColor *new_color);
OmegaWTKCompBrush *omegawtk_gradient_brush(OmegaWTKCompGradient *init_gradient);
void omegawtk_free_comp_brush(OmegaWTKCompBrush *brush);

/// OmegaWTK::Composition::Layer
OmegaWTKCompLayer *omegawtk_construct_layer(OmegaWTKWidget *widget,struct OmegaWTKRect rect);
void omegawtk_layer_set_style(OmegaWTKCompLayer *layer,OmegaWTKCompLayerStyle *style);
/// OmegaWTK::Composition::LayerStyle
OmegaWTKCompLayerStyle *omegawtk_construct_layer_style();
void omegawtk_layer_style_add_visual(OmegaWTKCompVisualType type,...);
void omegawtk_free_layer_style(OmegaWTKCompLayerStyle *style);
/// OmegaWTK::Composition::LayerTree::Limb
OmegaWTKCompLayer *omegawtk_layer_tree_limb_get_root_layer(OmegaWTKCompLayerTreeLimb *limb);
void omegawtk_layer_tree_limb_add_child_layer(OmegaWTKCompLayerTreeLimb *limb,OmegaWTKCompLayer *layer);

/// OmegaWTK::View /OmegaWTK::CanvasView Class
void omegawtk_view_add_subview(OmegaWTKView *view,OmegaWTKView *parent);
OmegaWTKCanvasView *omegawtk_construct_canvas_view(OmegaWTKWidget *widget,struct OmegaWTKRect rect,OmegaWTKView *parent);
OmegaWTKCompLayerTreeLimb * omegawtk_canvas_view_get_layer_tree_limb(OmegaWTKCanvasView *view);
void omegawtk_free_canvas_view(OmegaWTKCanvasView *view);

/// OmegaWTK::ViewDelegate Class
ENUM : OPT_PARAM {
    CursorEnterView,
    CursorExitView,
    LMouseDownOverView,
    LMouseUpOverView,
    RMouseDownOverView,
    RMouseUpOverView
} OmegaWTKViewDelegateEventType;
OmegaWTKViewDelegate *omegawtk_construct_view_delegate();
void omegawtk_view_delegate_register_event_handler(OmegaWTKViewDelegate *delegate,OmegaWTKViewDelegateEventType type,...);
void omegawtk_free_view_delegate(OmegaWTKViewDelegate *delegate);
/// OmegaWTK::ScrollView Class
OmegaWTKScrollView *omegawtk_construct_scroll_view(OmegaWTKWidget *widget,struct OmegaWTKRect rect,OmegaWTKView *parent);
void omegawtk_free_scroll_view(OmegaWTKScrollView *view);

/// OmegaWTK::ScrollViewDelegate Class
ENUM : OPT_PARAM {
    ScrollUpView,
    ScrollDownView,
    ScrollLeftView,
    ScrollRightView,
} OmegaWTKScrollViewDelegateEventType;
OmegaWTKScrollViewDelegate *omegawtk_construct_scroll_view_delegate();
void omegawtk_scroll_view_delegate_register_event_handler(OmegaWTKScrollViewDelegate *delegate,OmegaWTKScrollViewDelegateEventType type,...);
void omegawtk_free_scroll_view_delegate(OmegaWTKScrollViewDelegate *delegate);
/// OmegaWTK::Widget Class
OmegaWTKWidget *omegawtk_construct_widget(struct OmegaWTKRect rect,OmegaWTKWidget *parent);
void omegawtk_widget_set_parent_view(OmegaWTKWidget * widget,OmegaWTKView *parent);
void omegawtk_widget_add_observer(OmegaWTKWidget *widget,OmegaWTKWidgetObserver *observer);
void omegawtk_widget_remove_observer(OmegaWTKWidget *widget,OmegaWTKWidgetObserver *observer);
void omegawtk_widget_show();
void omegawtk_widget_hide();
void omegawtk_free_widget(OmegaWTKWidget *widget);
/// OmegaWTK::WidgetObserver Class
ENUM : OPT_PARAM {
    WidgetDidResize,
    WidgetDidShow,
    WidgetDidHide
} OmegaWTKWidgetObserverEventType;
OmegaWTKWidgetObserver *omegawtk_construct_widget_observer();
void omegawtk_widget_observer_register_event_handler(OmegaWTKWidgetObserver *observer,OmegaWTKWidgetObserverEventType type,...);
void omegawtk_free_widget_observer(OmegaWTKWidgetObserver * observer);


// int omegawtk_main(OmegaWTKAppInst *appInst);

#endif

#ifdef __cplusplus
}
#endif