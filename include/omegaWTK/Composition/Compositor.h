#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Native/NativeItem.h"
#include <string>
#include <vector>

#ifndef OMEGAWTK_COMPOSITION_COMPOSITOR_H
#define OMEGAWTK_COMPOSITION_COMPOSITOR_H

namespace OmegaWTK {
    namespace Composition {
        // class Animation {

        //     public:
        //     STATIC_OPT Fade = 0;
        //     STATIC_OPT KeyFramed = 1;
        //     STATIC_OPT CubicBezier = 2;

        //     class KeyFrame {

        //     };
        //     Animation(std::string name,std::vector<KeyFrame> key_fs);
        // };

        /// A specified area on the screen that can be drawn on by a parent Compositor
        class Canvas {

        };
        /// The visuals renderer!
        class Compositor {
            public:
            Compositor(){};
            virtual void DrawRect(Core::Rect rect,Core::Rect::Type type);
            // virtual void DoAnimation(Animation & anim);
            virtual ~Compositor();
        };

        typedef Compositor * CompPtr;
        typedef Canvas * CanvasPtr;

        CompPtr make_compositor();
        CanvasPtr make_canvas(Native::NativeItemPtr native_item);

    };
}

#endif