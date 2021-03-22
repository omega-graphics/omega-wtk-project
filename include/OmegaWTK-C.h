/**
  @file OmegaWTK-C.h
  The C Language Based bindings from C++.
 
  NOTE: This is a C ONLY file.
 */

#ifndef OMEGAWTK_C_LIB_H
#define OMEGAWTK_C_LIB_H

/**
 Typedefs!
 */

struct OmegaWTKObject {
    
};

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

struct OmegaWTKVisualEllipseParams {
    
};

struct OmegaWTKVisualImageParams {
    struct OmegaWTKRect rect;
    OmegaWTKCompBrush *brush;
};

struct OmegaWTKVisualTextParams {
    struct OmegaWTKRect textRect;
    OmegaWTKCompBrush *brush;
};

/// UI
typedef struct  __OmegaWTKView OmegaWTKView;
typedef OmegaWTKView OmegaWTKCanvasView;
typedef struct  __OmegaWTKWidget OmegaWTKWidget;


void OmegaWTKFree(struct OmegaWTKObject *anyObj);

/// OmegaWTK::Composition::Layer
OmegaWTKCompLayer *omegawtk_construct_layer(OmegaWTKWidget *widget,struct OmegaWTKRect rect);
void omegawtk_layer_set_style(OmegaWTKCompLayer *layer,OmegaWTKCompLayerStyle *style);
/// OmegaWTK::Composition::LayerStyle
OmegaWTKCompLayerStyle *omegawtk_construct_layer_style();
void omegawtk_layer_style_add_visual(OmegaWTKCompVisualType type,...);

/// OmegaWTK::Composition::LayerTree::Limb
OmegaWTKCompLayer *omegawtk_layer_tree_limb_get_root_layer(OmegaWTKCompLayerTreeLimb *limb);
void omegawtk_layer_tree_limb_add_child_layer(OmegaWTKCompLayerTreeLimb *limb,OmegaWTKCompLayer *layer);

/// OmegaWTK::View Class
OmegaWTKCanvasView *omegawtk_construct_canvas_view(OmegaWTKWidget *widget,struct OmegaWTKRect rect,OmegaWTKView *parent);
OmegaWTKCompLayerTreeLimb * omegawtk_canvas_view_get_layer_tree_limb(OmegaWTKCanvasView *view);

/// OmegaWTK::Widget Class
OmegaWTKWidget *omegawtk_construct_widget(struct OmegaWTKRect rect,OmegaWTKWidget *parent);

#endif

