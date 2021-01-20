#include <vector>
#include "omegaWTK/Composition/Compositor.h"

#ifndef OMEGAWTK_WIDGETS_WIDGET_H
#define OMEGAWTK_WIDGETS_WIDGET_H

namespace OmegaWTK {

    class Widget {
        std::vector<Widget *> children;
        Compositor *comp;
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
