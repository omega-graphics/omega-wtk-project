#include "omegaWTK/UI/SVGView.h"
#include "omegaWTK/Composition/Animation.h"

namespace OmegaWTK {

    struct SVGDef {

    };

    struct SVGObject {
        enum class Type : int {
            Circle,
            Ellipse,
            Rect,
            RoundedRect,
            Path,
            Polygon,
            Polyline,
            Line
        } type;
        struct {
            float fill_opacity;
            Composition::Color fill_color;
            float stroke_width;
            float stroke_opacity;
            Composition::Color stroke_color;
        } style;
        void *params;
    };

    /**
     * @brief Describes the Render procecdure for an SVG view to render.*/
    class SVGRenderState {
        SVGView & dest;
        OmegaCommon::Vector<SVGDef> defs;
        OmegaCommon::Vector<SVGObject> objects;
        OmegaCommon::Vector<SharedHandle<Composition::Layer>> svgLayers;
        OmegaCommon::Vector<SharedHandle<Composition::Canvas>> svgCanvases;
    public:
        explicit SVGRenderState(SVGView & dest):dest(dest){

        };

        void start(){

        };
        void resume(){

        };
        void pause(){

        };
    };

    SVGView::SVGView(const Core::Rect &rect,Composition::LayerTree *layerTree, View *parent) :
    View(rect,layerTree,parent) {

    }

    SVGSession::SVGSession(SharedHandle<SVGView> & view):view(view){

    }

    SharedHandle<SVGSession> SVGSession::Create(SharedHandle<SVGView> &view) {
        return (SharedHandle<SVGSession>)new SVGSession(view);
    }

    void SVGSession::setSVGSource(SVGDocument &document) {

    }

    void SVGSession::start() {
        currentRenderState->start();
    }

    void SVGSession::pause() {
        currentRenderState->pause();
    }
}