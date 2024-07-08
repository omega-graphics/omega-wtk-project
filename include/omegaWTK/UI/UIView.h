#include "Widget.h"
#include "omegaWTK/Composition/Canvas.h"
#include "omegaWTK/Composition/Animation.h"
#include "View.h"


#ifndef OMEGAWTK_UI_UIVIEW_H
#define OMEGAWTK_UI_UIVIEW_H

namespace OmegaWTK {

    namespace Composition {
        struct Color;
    }

    struct StyleSheet;
    OMEGACOMMON_SHARED_CLASS(StyleSheet);

    /**
     * @brief A generic struct for 2D shapes used by the UIView.
     * 
     */
    struct OMEGAWTK_EXPORT Shape {
    private:
        typedef enum : int {

        } T;
        T type;
        union {
            OmegaGTE::GRect _rect;
        };
    public:
        static Shape Scalar(int width,int height);
        static Shape Rect(OmegaGTE::GRect rect);
        static Shape RoundedRect(OmegaGTE::GRoundedRect rect);
        static Shape Ellipse(OmegaGTE::GEllipsoid ellipse);
        static Shape Path(OmegaGTE::GVectorPath2D path);
    };

    enum ElementAnimationKey : int {
        ElementAnimationKeyColorRed,
        ElementAnimationKeyColorGreen,
        ElementAnimationKeyColorBlue,
        ElementAnimationKeyColorAlpha,
        ElementAnimationKeyWidth,
        ElementAnimationKeyHeight,
        ElementAnimationKeyPathNodeX,
        ElementAnimationKeyPathNodeY,
    };


    /**
     * @brief Applied to a whole widget
     *  @paragraph The tag parameter for all function that make any changes to the background or border elements is the CanvasView tag.
     *  All other functions use tag per element.
        It's a generic class and can be used on one UIView OR on the entire Widget. (Can also apply to other views on other widgets.)
     */
    struct OMEGAWTK_EXPORT StyleSheet {

        struct Entry {
            UIViewTag tag;
        };
        
        OmegaCommon::Vector<Entry> entries;

    public:
        static StyleSheetPtr Create();
    /**
     * @brief Duplicates the style
     * 
     * @return StyleSheetPtr 
     */
        StyleSheetPtr copy();

        /**
         * @brief If the canvasTag is empty, then it will apply the color to ALL Views that use this style.
         * 
         * @param color 
         * @param transition 
         * @param duration 
         * @return StyleSheetPtr 
         */
        StyleSheetPtr backgroundColor(
            UIViewTag tag,
            const Composition::Color &color,
            bool transition = false,
            float duration = 0.f
            );
        /**
         * @brief If the canvasTag is empty, then it will apply the color to ALL Views that use this style.
         * 
         * @param use 
         * @return StyleSheetPtr 
         */
        StyleSheetPtr border(
            UIViewTag tag,
            bool use
            );

        StyleSheetPtr borderColor(
            UIViewTag tag,
            const Composition::Color &color,
            bool transition = false,
            float duration = 0.f
            );

        StyleSheetPtr borderWidth(
            UIViewTag tag,
            float width,
            bool transition = false,
            float duration = 0.f
            );

        /**
         * @brief If the elementTag is empty, then it will apply the color to ALL elements in ALL views that use this style. (Generally not recommended)
         * 
         * @param elementTag 
         * @param brush 
         * @param transition 
         * @param duration 
         * @return StyleSheetPtr 
         */
        StyleSheetPtr elementBrush(
            Composition::CanvasElementTag elementTag,
            SharedHandle<Composition::Brush> brush,
            bool transition = false,
            float duration = 0.f
            );
        /**
         * @brief Will affect ALL elements styled with this brush.
         * 
         * @param brush 
         * @param key 
         * @param curve 
         * @param duration 
         * @return StyleSheetPtr 
         */
        StyleSheetPtr elementBrushAnimation(SharedHandle<Composition::Brush> brush,ElementAnimationKey key,SharedHandle<Composition::AnimationCurve> curve,float duration);

        /**
         * @brief 
         * 
         * @param elementTag 
         * @param key 
         * @param curve 
         * @param duration 
         * @return StyleSheetPtr 
         */
        StyleSheetPtr elementAnimation(Composition::CanvasElementTag elementTag,ElementAnimationKey key,SharedHandle<Composition::AnimationCurve> curve,float duration);

        StyleSheetPtr elementPathAnimation(Composition::CanvasElementTag elementTag,SharedHandle<Composition::AnimationCurve> curve,int nodeIndex,float duration);

        StyleSheet();
        ~StyleSheet() = default;
    };
    /**
     * @brief Consumes a style. (Generally a widget)
     * 
     */
    INTERFACE StyleSheetUser {
        int framesPerSec = 60;
        INTERFACE_METHOD void useStyleSheet(StyleSheetPtr style) DEFAULT;
    };

    class UIViewLayout;
    typedef SharedHandle<UIViewLayout> UIViewLayoutPtr;

    /**
     * @brief An easy interface for designing UI (on a CanvasView)
     * @note This should only apply to one view, unless you want a duplicate view.
     */
    class UIViewLayout {
        struct Element {
            unsigned type = 0;
            Composition::CanvasElementTag tag;
            Core::Optional<OmegaCommon::UString> str;
            Core::Optional<Shape> shape;
        };
        OmegaCommon::Vector<Element> _content;
    public:
    /**
     * @brief Adds a text element. If the text element already exists, it will update with the new `content` parameter.
     * 
     * @param tag 
     * @param content 
     */
        void text(Composition::CanvasElementTag tag,OmegaCommon::UString content);
    /**
     * @brief Adds a shape element. If the shape element already exists, it will replace it with the `content` parameter.
     * 
     * @param tag 
     * @param content 
     */
        void shape(Composition::CanvasElementTag tag,Shape & shape);
    };

    INTERFACE LayoutUser {
        INTERFACE_METHOD void useLayout(UIViewLayout user) DEFAULT;
    };


    /**
     * @brief CanvasViewTag. 
     Used for tagging the entire UIView.
     * 
     */
    typedef Composition::CanvasElementTag UIViewTag;

    /**
     * @brief A simple UI container.
     * 
     */
    class OMEGAWTK_EXPORT UIView : public CanvasView, LayoutUser, StyleSheetUser {
        UIViewTag tag;
    public:

        explicit UIView(const Core::Rect & rect,Composition::LayerTree * layerTree,ViewPtr parent,UIViewTag tag);

        /**
         * @brief Method should be called before `enable()` If one layout is used, then you only have to call this ONCE.
         * 
         * @param user 
         */
        void useLayout(UIViewLayout user) override;
        /**
         * @brief Method should be called before `enable()`. If one style is used, then you only have to call this ONCE.
         * 
         * @param style
         */
        void useStyleSheet(StyleSheetPtr style) override;
        /**
         * @brief Updates the view (Invoke ONLY if there's a change made to the layout or the style)
         * 
         */
        void update();
    };

}

#endif //OMEGAWTK_UI_UIVIEW_H