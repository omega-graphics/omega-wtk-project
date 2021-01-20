#include "Widget.h"


#ifndef OMEGAWTK_WIDGETS_LABEL_H
#define OMEGAWTK_WIDGETS_LABEL_H

namespace OmegaWTK {
    class Label : Widget {
        std::string text;
        void render();
        public:
        Label(std::string text_to_draw,Widget *parent,bool isEnabled = true);
        ~Label();
    };
};

#endif