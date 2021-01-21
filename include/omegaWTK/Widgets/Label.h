#include "Widget.h"


#ifndef OMEGAWTK_WIDGETS_LABEL_H
#define OMEGAWTK_WIDGETS_LABEL_H

namespace OmegaWTK {
    class Label : Widget {
        Core::String text;
        void render();
        public:
        Label(Core::String text_to_draw,Widget *parent,bool isEnabled = true);
        ~Label();
    };
};

#endif