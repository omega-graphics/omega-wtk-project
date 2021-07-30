#include "omegaWTK/Core/Core.h"
#include "Color.h"
#include "FontEngine.h"
#include "Brush.h"
#include "omegaWTK/Media/ImgCodec.h"
#include "Animation.h"

#include <algorithm>

#ifndef OMEGAWTK_COMPOSITION_CANVAS_H
#define OMEGAWTK_COMPOSITION_CANVAS_H

namespace OmegaWTK {
    namespace Composition {
    
        
    struct OMEGAWTK_EXPORT  Border {
        Core::SharedPtr<Brush> brush;
        unsigned width;
        Border() = delete;
        Border(Core::SharedPtr<Brush> & _brush,unsigned _width):brush(_brush),width(_width){};
    };
    
    struct OMEGAWTK_EXPORT  CanvasLayerEffect {
        typedef enum : OPT_PARAM {
            DropShadow,
            Translation,
            Rotation,
            DirectionalBlur,
            GaussianBlur
        } Type;
        Type type;
        void * params;
        typedef struct {
            float x_offset, y_offset;
            float radius;
            float blurAmount;
            float opacity;
            Color color;  
        } DropShadowParams;
        typedef struct {
            
        } TranslationParams;
        typedef struct {
            
        } RotationParams;
        typedef struct {
            float radius;
        } GaussianBlurParams;
        typedef struct {
            float radius;
            float angle;
        } DirectionalBlurParams;
    };
    
// #define __LAYER_EFFECT ::OmegaWTK::Composition::LayerEffect
    
// #define LAYER_EFFECT_DROPSHADOW(x,y,radius,blurAmount,opacity,color) __LAYER_EFFECT ({__LAYER_EFFECT::DropShadow,new __LAYER_EFFECT::DropShadowParams({x,y,radius,blurAmount,opacity,color})})
    
// #define LAYER_EFFECT_GAUSSIANBLUR(radius) __LAYER_EFFECT ({__LAYER_EFFECT::GaussianBlur,new __LAYER_EFFECT::GaussianBlurParams({radius})})
// #define LAYER_EFFECT_DIRECTIONALBLUR(radius,angle) __LAYER_EFFECT ({__LAYER_EFFECT::DirectionalBlur,new __LAYER_EFFECT::DirectionalBlurParams({radius,angle})})

///   


    class CanvasSurface;
    /// An object drawn by a Compositor.
    struct  OMEGAWTK_EXPORT VisualCommand {

        CanvasSurface *targetSurface;

        typedef enum : OPT_PARAM {
            Rect,
            RoundedRect,
            Ellipse,
            Text,
            Bitmap
        } Type;
        Type type;
        typedef struct {
            Core::Rect rect;
            Core::SharedPtr<Brush> brush;
            Core::Optional<Border> border;
        } RectParams;
        
        typedef struct {
            Core::Rect rect;
            unsigned rad_x;
            unsigned rad_y;
            Core::SharedPtr<Brush> brush;
            Core::Optional<Border> border;
        } RoundedRectParams;
        
        typedef struct {
            Core::Ellipse ellipse;
            Core::SharedPtr<Brush> brush;
            Core::Optional<Border> border;
        } EllipseParams;
        typedef struct {
            Core::SharedPtr<TextRect> textRect;
            Core::SharedPtr<Brush> brush;
        } TextParams;
        typedef struct {
            Core::SharedPtr<Media::BitmapImage> img;
            Core::Rect rect;
        } BitmapParams;
        void * params;
        VisualCommand() = delete;
        VisualCommand(Type type,void * params):type(type),params(params){};
    };

    class Layer;

    class CompositorClient;
    /**
     
    */
    class OMEGAWTK_EXPORT CanvasSurface {

        friend class Layer;

        Layer * parentLayer = nullptr;
        
        CompositorClient *client = nullptr;

        Core::Rect & rect;

        void submitCommandToClient(VisualCommand * visual);

    public:

        CanvasSurface(Core::Rect & rect);

        Layer * getParentLayer();

        void drawRect(Core::Rect & rect,Core::SharedPtr<Brush> & brush);

        void drawRoundedRect(Core::RoundedRect & rect,Core::SharedPtr<Brush> & brush);

        void drawEllipse(Core::Ellipse & ellipse,Core::SharedPtr<Brush> & brush);

        void drawTextRect(SharedHandle<Composition::TextRect> & textRect,Core::SharedPtr<Brush> & brush);

        void drawImage(SharedHandle<Media::BitmapImage> & img,Core::SharedPtr<Brush> & brush);

        void applyEffect(SharedHandle<CanvasLayerEffect> & effect);
    };


    
    
// #define __COMPOSITION__ ::OmegaWTK::Composition::

// #define VISUAL_RECT(rect,brush) __COMPOSITION__ Visual::RectParams({rect,brush,{}})
// #define VISUAL_RECT_W_FRAME(rect,brush,border) __COMPOSITION__ Visual::RectParams({rect,brush,border})

// #define VISUAL_ROUNDED_RECT(rect,brush) __COMPOSITION__ Visual::RoundedRectParams({rect,rect.radius_x,rect.radius_y,brush,{}})
// #define VISUAL_ROUNDED_RECT_W_FRAME(rect,brush,border) __COMPOSITION__ Visual::RoundedRectParams({rect,rect.radius_x,rect.radius_y,brush,border})
    
// #define VISUAL_TEXT(textRectRef,brush) __COMPOSITION__ Visual::TextParams({textRectRef,brush})
// #define VISUAL_IMG(img,rect) __COMPOSITION__ Visual::BitmapParams({img,rect})
    
//#undef __COMPOSITION__
    // class BackendImpl;
    
    // class OMEGAWTK_EXPORT OMEGAWTK_DEPRECATED  LayerStyle {
    //     Core::Vector<Visual *> visuals;
    //     Core::Vector<SharedHandle<LayerEffect>> effects;
    //     Color background = Color(Color::White);
    //     template<class _Ty>
    //     void _construct_visual(Visual::Type type,_Ty & params){
    //         visuals.push_back(new Visual(visuals.size(),type,(void *)new _Ty(params)));
    //     };
    //     friend class BackendImpl;
    // public:
    //     LayerStyle();
    //     /**
    //      Adds A Rect to the Style!
    //      */
    //     void add(Visual::RectParams params);
    //     /**
    //      Adds A Rounded Rect to the Style!
    //      */
    //     void add(Visual::RoundedRectParams params);
    //     /**
    //      Adds An Ellipse to the Style!
    //      */
    //     void add(Visual::EllipseParams params);
    //     /**
    //      Adds A Bitmap to the Style!
    //      */
    //     void add(Visual::BitmapParams params);
    //     /**
    //      Adds A Text Object to the Style!
    //      */
    //     void add(Visual::TextParams params);
    //     /**
    //     Changes/Sets the brush for the visual at the provided index.
    //     */
    //     void setBrush(unsigned id,const Core::SharedPtr<Brush> & new_brush);
    //     void setBackgroundColor(const Color & color){ background = color;};
    //     void addEffect(SharedHandle<LayerEffect> & effect);
    //     template<class _Ty>
    //     _Ty * getVisualAtIndex(unsigned idx);


    //     template<>
    //     Visual::BitmapParams * getVisualAtIndex(unsigned idx)
    //     {
    //         return (Visual::BitmapParams *)visuals[idx]->params;
    //     };

    //     template<>
    //     Visual::EllipseParams * getVisualAtIndex(unsigned idx)
    //     {
    //         return (Visual::EllipseParams *)visuals[idx]->params;
    //     };

    //     template<>
    //     Visual::RectParams * getVisualAtIndex(unsigned idx)
    //     {
    //         return (Visual::RectParams *)visuals[idx]->params;
    //     };

    //     template<>
    //     Visual::RoundedRectParams * getVisualAtIndex(unsigned idx)
    //     {
    //         return (Visual::RoundedRectParams *)visuals[idx]->params;
    //     };

    //     template<>
    //     Visual::TextParams * getVisualAtIndex(unsigned idx)
    //     {
    //         return (Visual::TextParams *)visuals[idx]->params;
    //     };
        
    //     template<class _Ty>
    //     _Ty * operator[](unsigned idx){
    //         return getVisualAtIndex<_Ty>(idx);
    //     };
    //     ~LayerStyle();
    // };
    
    // class OMEGAWTK_EXPORT OMEGAWTK_DEPRECATED  WindowStyle {
    //     Color bkgrd;
    //     unsigned frameWidth;
    //     unsigned frameHeight;
    //     Core::String title;
    // public:
    //     ~WindowStyle();
    // };
    
   
    // class OMEGAWTK_EXPORT OMEGAWTK_DEPRECATED  MenuStyle {
        
    // };
    
};
    
};

#endif
