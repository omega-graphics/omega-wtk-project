#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Composition/Compositor.h"

#ifndef OMEGAWTK_WIDGETS_WIDGET_H
#define OMEGAWTK_WIDGETS_WIDGET_H

namespace OmegaWTK {

    class Widget {
        Core::Vector<Widget *> children;
        Composition::CompPtr comp;
        Native::NativeItemPtr native;
        Core::Rect rect;
        bool isEnabled;
        bool isRoot;
        protected: 
        Native::NativeItemPtr getNativeItemPtr();
        void setNativeItemPtr(Native::NativeItemPtr ptr);
        Composition::CompPtr getCompositorPtr();
        Composition::CanvasPtr canvas;
        public:
        Widget(Widget * parent,Core::Rect _rect,bool enabled = true);
        /// NOTE: Only Call this Function on the Root Widget!
        void setCompositor(Composition::CompPtr _comp);
        void addChild(Widget * child);
        virtual void render();
        virtual ~Widget();
    };
};

#endif
