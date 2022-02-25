#include "omegaWTK/UI/SVGView.h"
#include "omegaWTK/Composition/Animation.h"
#include "omegaWTK/Composition/Path.h"

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
        union D {
            Core::Ellipse e;
            Core::Rect r;
            Core::RoundedRect rr;
            Composition::Path p;
            void _destroy(Type t){
                if(t == Type::Path){
                    p.~Path();
                }
            };
            ~D() DEFAULT;
        }params;

        ~SVGObject(){
            params._destroy(type);
        };
    };

    string_enum SVGObjectTagName {

        string_enum_field circle = "circle";

        string_enum_field group = "g";
        
        string_enum_field def = "def";

        string_enum_field rect = "rect";

        string_enum_field path = "path";

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
        explicit SVGRenderState(SVGView & dest,SVGDocument &doc):dest(dest){
            auto tag = doc.root();
            auto children = tag.children();
            for(auto & t : children){
                if(t.name() == SVGObjectTagName::circle){
                    
                }
                else if(t.name() == SVGObjectTagName::path){
                    auto d = t.attribute("d");
                }
            }
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
        currentRenderState = std::make_shared<SVGRenderState>(*view,document);
        
    }

    void SVGSession::start() {
        currentRenderState->start();
    }

    void SVGSession::pause() {
        currentRenderState->pause();
    }
}