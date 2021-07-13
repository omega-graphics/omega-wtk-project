/**
 @file Layer.h
 
 Defines the ViewRenderTarget, Layer, and LayerTree.
 */

#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Native/NativeItem.h"
#include "omegaWTK/Native/NativeWindow.h"
#include "Canvas.h"
#include <functional>

#ifndef OMEGAWTK_COMPOSITION_LAYER_H
#define OMEGAWTK_COMPOSITION_LAYER_H

namespace OmegaWTK {

class AppWindow;
class AppWindowManager;
class View;

namespace Composition {
    
    class ViewRenderTarget;
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
    protected:
        OmegaCommon::Vector<LayerTreeObserver *> observers;

        friend class Layer;

        void notifyObserversOfResize(Layer *layer);

        void notifyObserversOfDisable(Layer *layer);

        void notifyObserversOfEnable(Layer *layer);

    public:
        class OMEGAWTK_EXPORT Limb : public Native::NativeLayerTreeLimb {
            LayerTree *parentTree;
            Layer *limbRoot;
            bool enabled;
            ViewRenderTarget *renderTarget;
            friend class LayerTree;
        public:

            LayerTree *getParentTree();
            using iterator = OmegaCommon::Vector<SharedHandle<Layer>>::iterator;
            Layer *getRootLayer();
            void addLayer(SharedHandle<Layer> layer);
            /**
             Constructs a LayerTree::Limb
             @param rect The Root Layer's Rect
             @param compClient A Pointer to the parent CompositorClient.
             @returns A Limb
             */
            Limb(const Core::Rect & rect,CompositorClient *compClient,ViewRenderTarget *renderTarget);
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
        Limb *getTreeRoot();
        unsigned getParentLimbChildCount(Limb *parent);
        Limb *getLimbAtIndexFromParent(unsigned idx,Limb *parent);
        /**
         Creates a Limb for the Layer Tree
         */
        SharedHandle<Limb> createLimb(const Core::Rect &rect_for_root_layer,CompositorClient *compClient,ViewRenderTarget *renderTarget);
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

    class OMEGAWTK_EXPORT Layer {
        unsigned id_gen = 0;
        LayerTree::Limb *parentLimb;
        SharedHandle<CanvasSurface> surface;
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
        LayerTree::Limb *getParentLimb();
        /// @name Base Functions
        /// @{
//            Native::NativeItemPtr getTargetNativePtr(){return compTarget->native;};
//            void setStyle(Style & style){
//                compTarget->style.reset(new Style(std::move(style)));
//            };
//            auto & getStyle(){return *compTarget->style;};

        /// Resize the Layer with the new rect
        void resize(Core::Rect & newRect);
        Core::Rect & getLayerRect(){return surface_rect;};
        void setEnabled(bool state){enabled = state;};
        bool isChildLayer(){return parent_ptr != nullptr;}
        /// @}
        
        /// @name Composing Functions!
        /// Draws on to its target!
        /// @{
        SharedHandle<CanvasSurface> & getSurface();
        /// @}
        
        
        Layer(const Core::Rect & rect,CompositorClient *compClient);
//            Layer(Layer &layer);
        ~Layer();
    };

    INTERFACE LayerTreeObserver {
    public:
        /**
         A method called when the target layer has resized within this LayerTree
         @param layer
        */
        INTERFACE_METHOD(void,layerHasResized,Layer *layer)

        /**
         A method called when the target layer has been disabled within this LayerTree
         @param layer
        */
        INTERFACE_METHOD(void,layerHasDisabled,Layer *layer)

        /**
         A method called when the target layer has been enabled within this LayerTree
         @param layer
        */
        INTERFACE_METHOD(void,layerHasEnabled,Layer *layer)
    };

    /**
     A singular surface
     */
    class OMEGAWTK_EXPORT  WindowLayer {
        Native::NWH native_window_ptr;
        Core::Rect & rect;
        SharedHandle<CanvasSurface> windowSurface;
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
