#include "Widget.h"

#ifndef OMEGAWTK_UI_STYLE_H
#define OMEGAWTK_UI_STYLE_H

namespace OmegaWTK {

    namespace Composition {
        struct Color;
    }

    struct OMEGAWTK_EXPORT Style {
        union Entry;
        UniqueHandle<OmegaCommon::Map<unsigned,Entry>> data;
        friend class GenericStyledWidget;
    public:
        Style copy();

        Style &backgroundColor(
            const Composition::Color &color,
            boolean animate = false,
            float duration = 0.f
            );

        Style &color(
            const Composition::Color &color,
            boolean animate = false,
            float duration = 0.f
            );

        Style &border(
            boolean use
            );

        Style &borderColor(
            const Composition::Color &color,
            boolean animate = false,
            float duration = 0.f
            );

        Style &borderWidth(
            float width,
            boolean animate = false,
            float duration = 0.f
            );

        Style();
        ~Style() = default;
    };

    class OMEGAWTK_EXPORT GenericStyledWidget : public Widget {
    private:
        void init() override;
        void performStylingOnView(SharedHandle<View> &v);
    public:
        explicit GenericStyledWidget(const Core::Rect &rect,Widget *parent = nullptr);
        void switchStyle(Style &style);
        virtual ~GenericStyledWidget() override = default;
    };

}

#endif //OMEGAWTK_UI_STYLE_H