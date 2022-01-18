#include "omegaWTK/Core/Core.h"
#include "Brush.h"
#include "Path.h"
#include "FontEngine.h"
#include "Brush.h"
#include "omegaWTK/Media/ImgCodec.h"

#include "CompositorClient.h"

#include <algorithm>

#ifndef OMEGAWTK_COMPOSITION_CANVAS_H
#define OMEGAWTK_COMPOSITION_CANVAS_H

namespace OmegaWTK {
    class View;
    namespace Composition {

    struct OMEGAWTK_EXPORT  Border {
        Core::SharedPtr<Brush> brush;
        unsigned width;

        Border() = delete;

        Border(Core::SharedPtr<Brush> &_brush, unsigned _width) : brush(_brush), width(_width) {};
    };

    struct OMEGAWTK_EXPORT  CanvasEffect {
        typedef enum : OPT_PARAM {
            DirectionalBlur,
            GaussianBlur
        } Type;
        Type type;
        void *params;
        typedef struct {
        float radius;
        } GaussianBlurParams;
        typedef struct {
            float radius;
            float angle;
        } DirectionalBlurParams;
    };
///   


    class Canvas;
    /// An object drawn by a Compositor.
    struct  OMEGAWTK_EXPORT VisualCommand {
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
            Core::RoundedRect rect;
            Core::SharedPtr<Brush> brush;
            Core::Optional<Border> border;
        } RoundedRectParams;
        
        typedef struct {
            Core::Ellipse ellipse;
            Core::SharedPtr<Brush> brush;
            Core::Optional<Border> border;
        } EllipseParams;
        typedef struct {
            Core::SharedPtr<Media::BitmapImage> img;
            Core::SharedPtr<OmegaGTE::GETexture> texture;
            Core::Rect rect;
        } BitmapParams;
        void * params;
        VisualCommand() = delete;
        VisualCommand(Type type,void * params):type(type),params(params){};
    };

    class Layer;

    /// @brief A frozen state of visual items drawn to a Canvas.
    struct CanvasFrame {
        Layer *targetLayer;
        Core::Rect & rect;
        struct {
            float r = 0.f,g = 0.f,b = 0.f,a = 0.f;
        } background;
        OmegaCommon::Vector<VisualCommand> currentVisuals;
        OmegaCommon::Vector<CanvasEffect> currentEffects;
    };

    /**
     @brief Renders 2D vector graphics to a Layer.
    */
    class OMEGAWTK_EXPORT Canvas : public CompositorClient {

        Core::Rect & rect;

        SharedHandle<CanvasFrame> current;

        Layer &layer;

        friend class ::OmegaWTK::View;

        explicit Canvas(CompositorClientProxy &proxy,Layer &layer);

    public:
        OMEGACOMMON_CLASS("OmegaWTK.Composition.Canvas")
        /**
         @brief Draw a Path.
         */
        void drawPath(Path & path);

        /**
         @brief Draw a Rect.
         @param rect The Rect.
         @param brush The Brush to fill the Rect with.
         */
        void drawRect(Core::Rect & rect,Core::SharedPtr<Brush> & brush);

        /**
         @brief Draw a Rounded Rect.
         @param rect The Rounded Rect.
         @param brush The Brush to fill the Rect with.
         */
        void drawRoundedRect(Core::RoundedRect & rect,Core::SharedPtr<Brush> & brush);

        /**
         @brief Draw an Ellipse.
         @param ellipse The Ellipse.
         @param brush The Brush to fill the Rect with.
         */
        void drawEllipse(Core::Ellipse & ellipse,Core::SharedPtr<Brush> & brush);

        /**
            @brief Draw an Image to the corresponding rect.
            @param img The Image.
            @param rect The Rect to bound the image to.
           */
        void drawImage(SharedHandle<Media::BitmapImage> & img,const Core::Rect & rect);

        /**
           @brief Draw a GETexture to the corresponding rect.
           @param img The GETExture.
           @param rect The Rect to bound the image to.
          */
        void drawGETexture(SharedHandle<OmegaGTE::GETexture> & img,const Core::Rect & rect);

        /**
           @brief Apply an effect to the current frame.
           @param effect The CanvasEffect to apply.
          */
        void applyEffect(SharedHandle<CanvasEffect> & effect);

        /// @brief Sends current frame to CompositorClientProxy to be drawn.
        void sendFrame();
        /// @brief Retrives current Frame drawn.
        SharedHandle<CanvasFrame> getCurrentFrame();
        /// @brief Retrives current Frame drawn and resets Canvas state.
        SharedHandle<CanvasFrame> nextFrame();
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
