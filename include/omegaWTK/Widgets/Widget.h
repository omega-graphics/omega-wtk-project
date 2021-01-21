#include "omegaWTK/Core/Core.h"
#include <memory>

#ifndef OMEGAWTK_WIDGETS_WIDGET_H
#define OMEGAWTK_WIDGETS_WIDGET_H

namespace OmegaWTK {

    class Compositor;
    //Null Object Placeholder for native object!
    class NativeItem {};

    class Widget {
        Core::Vector<Widget *> children;
        Compositor * comp;
        NativeItem * native;
        bool isEnabled;
        bool isRoot;
        Compositor *getCompositorPtr();
        public:
        Widget(Widget * parent,bool enabled = true);
        /// NOTE: Only Call this Function on the Root Widget!
        void setCompositor(Compositor *_comp);
        void addChild(Widget * child);
        virtual void render();
        virtual ~Widget();
    };
};

#endif
