#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Composition/Compositor.h"

#ifndef OMEGAWTK_WIDGETS_WIDGET_H
#define OMEGAWTK_WIDGETS_WIDGET_H

namespace OmegaWTK {

    class Widget {
        Core::Vector<Widget *> children;
        Composition::CompPtr comp;
        Native::NativeItemPtr native;
        bool isEnabled;
        bool isRoot;
        protected: 
        Native::NativeItemPtr getNativeItemPtr();
        Composition::CompPtr getCompositorPtr();
        public:
        Widget(Widget * parent,bool enabled = true);
        /// NOTE: Only Call this Function on the Root Widget!
        void setCompositor(Composition::CompPtr _comp);
        void addChild(Widget * child);
        virtual Composition::CanvasPtr render();
        virtual ~Widget();
    };
};

#endif
