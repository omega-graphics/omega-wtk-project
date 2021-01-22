#include "Widget.h"


#ifndef OMEGAWTK_WIDGETS_LABEL_H
#define OMEGAWTK_WIDGETS_LABEL_H

namespace OmegaWTK {
    class Label : Widget {
        Core::Text text;
        void render();
        public:
        Label(Core::Text text_to_draw,Widget *parent,Core::Rect _rect,bool isEnabled = true);
        ~Label();
    };
};

#endif
