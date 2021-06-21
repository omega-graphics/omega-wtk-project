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
    class Compositor;
        /**
            A mutlifeatured surface for composing visuals on.
         */
    class OMEGAWTK_EXPORT Layer {
        unsigned id_gen = 0;
        SharedHandle<Canvas> canvas;
        Core::Vector<SharedHandle<Layer>> children;
        Layer * parent_ptr;
        Core::Rect surface_rect;
        bool enabled;
        bool needsNativeResize;
        Compositor *ownerCompositor;
        friend class Compositor;
        friend class BackendImpl;
        friend class LayerTree;
        void addSubLayer(SharedHandle<Layer> & layer);
        void removeSubLayer(SharedHandle<Layer> & layer);
    public:
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
        SharedHandle<Canvas> & getCanvas();
        /// @}
        
        
        Layer(const Core::Rect & rect,Compositor *compPtr);
//            Layer(Layer &layer);
        ~Layer();
    };

        typedef Layer CanvasLayer;
    
//    typedef std::function<bool(Layer *)> LayerTreeTraversalCallback;
    /**
     An entire widget's layer construct
     */
    class OMEGAWTK_EXPORT LayerTree {
        Compositor *widgetCompositor;
        friend class ::OmegaWTK::View;
    public:
        class OMEGAWTK_EXPORT Limb : public Native::NativeLayerTreeLimb {
            Layer *limbRoot;
            bool enabled;
            ViewRenderTarget *renderTarget;
            friend class BackendImpl;
        public:
            using iterator = Core::Vector<SharedHandle<Layer>>::iterator;
            Layer *getRootLayer();
            void addLayer(SharedHandle<Layer> layer);
            /**
             Constructs a LayerTree::Limb
             @param rect The Root Layer's Rect
             @param compPtr A Pointer to the Widget's Compositor.
             @returns A Limb
             */
            Limb(const Core::Rect & rect,Compositor *compPtr,ViewRenderTarget *renderTarget);
            iterator begin();
            iterator end();
            void disable();
            void enable();
            void redraw();
            void layout();
        };
    private:
        SharedHandle<Limb> rootLimb;
        Core::Map<Limb *,Core::Vector<SharedHandle<Limb>>> body;
    public:
        Limb *getTreeRoot();
        unsigned getParentLimbChildCount(Limb *parent);
        Limb *getLimbAtIndexFromParent(unsigned idx,Limb *parent);
        /**
         Creates a Limb for the Layer Tree
         */
        SharedHandle<Limb> createLimb(const Core::Rect &rect_for_root_layer,ViewRenderTarget *renderTarget);
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
        LayerTree(Compositor *compPtr);
        ~LayerTree();
    };
    /**
     A singular surface
     */
    class OMEGAWTK_EXPORT  WindowLayer {
        Native::NWH native_window_ptr;
        Core::Rect & rect;
        SharedHandle<Canvas> windowCanvas;
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
