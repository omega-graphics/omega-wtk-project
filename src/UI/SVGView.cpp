#include "omegaWTK/UI/SVGView.h"

namespace OmegaWTK {

    SVGView::SVGView(const Core::Rect &rect,Composition::LayerTree *layerTree, View *parent) :
    View(rect,layerTree,parent) {

    }

    SVGSession::SVGSession(SharedHandle<SVGView> & view):view(view){

    }

    SharedHandle<SVGSession> SVGSession::Create(SharedHandle<SVGView> &view) {
        return (SharedHandle<SVGSession>)new SVGSession(view);
    }

    void SVGSession::setSVGSource(const SVGDocument &document) {

    }

    void SVGSession::start() {

    }

    void SVGSession::pause() {

    }
}