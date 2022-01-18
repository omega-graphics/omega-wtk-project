/**
 @file Layer.h
 
 Defines the ViewRenderTarget, Layer, and LayerTree.
 */

#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Native/NativeItem.h"
#include "omegaWTK/Native/NativeWindow.h"

#include "Brush.h"

#include <functional>

#ifndef OMEGAWTK_COMPOSITION_LAYER_H
#define OMEGAWTK_COMPOSITION_LAYER_H

namespace OmegaWTK {

class AppWindow;
class AppWindowManager;
class View;
class Widget;

namespace Composition {
    
    class ViewRenderTarget;
	class Canvas;
        /**
            A mutlifeatured surface for composing visuals on.
         */
    class Layer;

    typedef Layer CanvasLayer;
    
    INTERFACE LayerTreeObserver;
//    typedef std::function<bool(Layer *)> LayerTreeTraversalCallback;
    /**
     An entire widget's layer construct
     */
    class OMEGAWTK_EXPORT LayerTree {
        friend class ::OmegaWTK::View;
        friend class ::OmegaWTK::Widget;
    protected:
        OmegaCommon::Vector<LayerTreeObserver *> observers;

        friend class Layer;

        void notifyObserversOfResize(Layer *layer);

        void notifyObserversOfDisable(Layer *layer);

        void notifyObserversOfEnable(Layer *layer);

         /**
         @note Only called by ::OmegaWTK::Widget
        */
        void notifyObserversOfWidgetDetach();

    public:
       
        class OMEGAWTK_EXPORT Limb : public Native::NativeLayerTreeLimb {
            LayerTree *parentTree;
            SharedHandle<Layer> limbRoot;
            bool enabled;
           
            friend class LayerTree;
        public:
            LayerTree *getParentTree();
            using iterator = OmegaCommon::Vector<SharedHandle<Layer>>::iterator;
            SharedHandle<Layer> & getRootLayer();
            void addLayer(SharedHandle<Layer> layer);
            /**
             Constructs a LayerTree::Limb
             @param rect The Root Layer's Rect
             @param compClient A Pointer to the parent CompositorClient.
             @returns A Limb
             */
            Limb(const Core::Rect & rect);
            iterator begin();
            iterator end();
            void disable();
            void enable();
            // void commit();
            // void layout();
        };
    private:
        SharedHandle<Limb> rootLimb;
        OmegaCommon::Map<Limb *,OmegaCommon::Vector<SharedHandle<Limb>>> body;
    public:
        void addObserver(LayerTreeObserver * observer);
        void removeObserver(LayerTreeObserver * observer);
        Limb *getTreeRoot();
        unsigned getParentLimbChildCount(Limb *parent);
        Limb *getLimbAtIndexFromParent(unsigned idx,Limb *parent);
        /**
         Creates a Limb for the Layer Tree
         */
        SharedHandle<Limb> createLimb(const Core::Rect &rect_for_root_layer);
        /**
         Adds a Limb to the Layer Tree.
         @param limb
         @param parent Assumes the root limb is the parent unless this value is specified
         */
        void addChildLimb(SharedHandle<Limb> & limb,Limb *parent = nullptr);
        /**
         NOTE: Only Call this function once!!
         */
        void setRootLimb(SharedHandle<Limb> & limb);
        LayerTree();
        ~LayerTree();
    };

    struct LayerEffect {
        enum : OPT_PARAM {
            DropShadow,
            Transformation,
        } type;
        void *params;
        typedef struct {
            float x_offset, y_offset;
            float radius;
            float blurAmount;
            float opacity;
            Color color;
        } DropShadowParams;
        typedef struct {
            struct {
                float x;
                float y;
                float z;
            } translate;
            struct {
                float pitch;
                float yaw;
                float roll;
            } rotate;
            struct {
                float x;
                float y;
                float z;
            } scale;
        } TransformationParams;
    };

    /**
     @brief A resizable rectangular surface for displaying vector graphics.
     */
    class OMEGAWTK_EXPORT Layer {
        unsigned id_gen = 0;
        LayerTree::Limb *parentLimb;
        OmegaCommon::Vector<SharedHandle<Layer>> children;

        Layer * parent_ptr;
        Core::Rect surface_rect;
        bool enabled;
        bool needsNativeResize;
        
        friend class LayerTree;
        friend class ::OmegaWTK::View;
        void addSubLayer(SharedHandle<Layer> & layer);
        void removeSubLayer(SharedHandle<Layer> & layer);
    public:
        OMEGACOMMON_CLASS("OmegaWTK.Composition.Layer")

        LayerTree::Limb *getParentLimb();

        /**
            @brief Resize the Layer with the new rect
            @param newRect
         */
        void resize(Core::Rect & newRect);

        /**
           @brief Retrieves the rect that defines the bounds.
           @returns A reference to the Core::Rect.
           */
        Core::Rect & getLayerRect(){return surface_rect;};

        /**
         @brief Enable the layer
         @param state The boolean to use.
         */
        void setEnabled(bool state){enabled = state;};

        /**
          @brief Checks if this layer is a child of another layer.
          @returns bool
          */
        bool isChildLayer(){return parent_ptr != nullptr;}
   
        /** @brief Construct a Layer.
           @param rect*/
        explicit Layer(const Core::Rect & rect);
//            Layer(Layer &layer);
        ~Layer();
    };

    /// @interface LayerTreeObserver
    class LayerTreeObserver {
    public:

        /**
         * @fn virtual void LayerTreeObserver::hasDetached(LayerTree *tree) = 0
         @brief A method called when the Widget/LayerTree has detached from a WidgetTree.
        */
        INTERFACE_METHOD void hasDetached(LayerTree *tree) ABSTRACT;

        /**
          @fn virtual void LayerTreeObserver::layerHasResized(Layer *layer) = 0
         @brief A method called when the target layer has resized within this LayerTree
         @param layer
        */
        INTERFACE_METHOD void layerHasResized(Layer *layer) ABSTRACT;

        /**
         @fn virtual void LayerTreeObserver::layerHasDisabled(Layer *layer) = 0
         @brief A method called when the target layer has been disabled within this LayerTree
         @param layer
        */
        INTERFACE_METHOD void layerHasDisabled(Layer *layer) ABSTRACT;

        /**
         @fn virtual void LayerTreeObserver::layerHasEnabled(Layer *layer) = 0
         @brief A method called when the target layer has been enabled within this LayerTree
         @param layer
        */
        INTERFACE_METHOD void layerHasEnabled(Layer *layer) ABSTRACT;

        /// @fn virtual LayerTreeObserver::~LayerTreeObserver() = default
        virtual ~LayerTreeObserver() = default;
    };

    /**
     A singular surface
     */
    class OMEGAWTK_EXPORT  WindowLayer {
        Native::NWH native_window_ptr;
        Core::Rect & rect;
        SharedHandle<Canvas> windowSurface;
        // SharedHandle<MenuStyle> menuStyle;
        friend class OmegaWTK::AppWindow;
        friend class OmegaWTK::AppWindowManager;
        friend class Compositor;
        void redraw();
        // void setWindowStyle(SharedHandle<WindowStyle> & style);
        // void setMenuStyle(SharedHandle<MenuStyle> & style);
    public:
        WindowLayer(Core::Rect & rect,Native::NWH native_window_ptr);
    };
    
};


};

#endif
