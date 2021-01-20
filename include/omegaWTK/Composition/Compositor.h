#include "omegaWTK/Core/Core.h"
#include <string>
#include <vector>

#ifndef OMEGAWTK_COMPOSITION_COMPOSITOR_H
#define OMEGAWTK_COMPOSITION_COMPOSITOR_H

namespace OmegaWTK {
    class Animation {

        public:
        STATIC_ENUM Fade = 0;
        STATIC_ENUM KeyFramed = 1;
        STATIC_ENUM CubicBezier = 2;

        class KeyFrame {

        };
        Animation(std::string name,std::vector<KeyFrame> key_fs);
    };
    
    class Compositor {
        public:
        Compositor(){};
        virtual void DrawRect(CoreRect rect,CoreRect::Type type);
        virtual void DoAnimation(Animation & anim);
        virtual ~Compositor();
    };

    Compositor * make_compositor();
}

#endif