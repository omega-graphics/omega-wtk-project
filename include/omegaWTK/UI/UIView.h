#include "Widget.h"
#include "omegaWTK/Composition/Canvas.h"
#include "View.h"

#ifndef OMEGAWTK_UI_UIVIEW_H
#define OMEGAWTK_UI_UIVIEW_H

namespace OmegaWTK {

    namespace Composition {
        struct Color;
    }

    struct Style;
    OMEGACOMMON_SHARED_CLASS(Style);
    /**
     * @brief Applied to a whole widget
     *  @paragraph The tag parameter for all function that make any changes to the background or border elements is the CanvasView tag.
     *  All other functions use tag per element.
     */
    struct OMEGAWTK_EXPORT Style {
        union Entry;
        UniqueHandle<OmegaCommon::Map<unsigned,Entry>> data;
    public:
        Style copy();

        StylePtr backgroundColor(
            const Composition::Color &color,
            bool animate = false,
            float duration = 0.f
            );

        StylePtr border(
            bool use
            );

        StylePtr borderColor(
            const Composition::Color &color,
            bool animate = false,
            float duration = 0.f
            );

        StylePtr borderWidth(
            float width,
            bool animate = false,
            float duration = 0.f
            );

        StylePtr elementColor(
            const Composition::Color &color,
            bool animate = false,
            float duration = 0.f
            );

        Style();
        ~Style() = default;
    };
    /**
     * @brief Consumes a style. (Generally a widget)
     * 
     */
    INTERFACE StyleUser {
        INTERFACE_METHOD void useStyle(StylePtr style) DEFAULT;
    };


    /**
     * @brief A generic strut for 2D shapes used by the UIView.
     * 
     */
    struct Shape {
    private:
        typedef enum : int {

        } T;
        T type;
    public:
        static Shape Line();
        static Shape Rect();
        static Shape RoundedRect();
        static Shape Ellipse();
        static Shape Path();
    };

    /**
     * @brief An easy interface for designing UI (on a CanvasView)
     * 
     */
    class UILayout {
    public:
        void text(Composition::CanvasElementTag tag,OmegaCommon::String content);
        void shape(Composition::CanvasElementTag tag,Shape shape);
    };

    INTERFACE LayoutUser {
        INTERFACE_METHOD void useLayout(LayoutUser user) DEFAULT;
    };
    /**
     * @brief A simple UI container.
     * 
     */
    class OMEGAWTK_EXPORT UIView : public CanvasView, LayoutUser, StyleUser {


        /**
         * @brief Method should be called before `enable()`
         * 
         * @param user 
         */
        void useLayout(LayoutUser user) override;
        /**
         * @brief Method should be called before `enable()`
         * 
         * @param style
         */
        void useStyle(StylePtr style) override;
    };

}

#endif //OMEGAWTK_UI_UIVIEW_H