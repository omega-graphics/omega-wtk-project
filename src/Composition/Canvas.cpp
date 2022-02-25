#include "omegaWTK/Composition/Canvas.h"
#include "omegaWTK/Composition/CompositorClient.h"
#include "omegaWTK/Composition/Layer.h"

namespace OmegaWTK::Composition {

VisualCommand::Data::Data(const Core::Rect & rect,Core::SharedPtr<Brush> brush,Core::Optional<Border> border) :
rectParams({rect,brush,border})
{

}

VisualCommand::Data::Data(const Core::RoundedRect & rect,Core::SharedPtr<Brush> brush,Core::Optional<Border> border) :
roundedRectParams({rect,brush,border}){

};

VisualCommand::Data::Data(const Core::Ellipse & ellipse,Core::SharedPtr<Brush> brush,Core::Optional<Border> border) :
ellipseParams({ellipse,brush,border}){

};

VisualCommand::Data::Data(Core::SharedPtr<Media::BitmapImage> img,const Core::Rect &rect) :
bitmapParams({img,nullptr,nullptr,rect}){

};

VisualCommand::Data::Data(Core::SharedPtr<OmegaGTE::GETexture> texture,Core::SharedPtr<OmegaGTE::GEFence> textureFence,const Core::Rect &rect) :
bitmapParams({nullptr,texture,textureFence,rect}){

};



void VisualCommand::Data::_destroy(Type t){
    if(t == Rect){
        rectParams.border.~optional();
        rectParams.brush.~shared_ptr();
    }
    else if(t == RoundedRect){
        roundedRectParams.border.~optional();
        roundedRectParams.brush.~shared_ptr();
    }
    else if(t == Ellipse){
        ellipseParams.border.~optional();
        ellipseParams.brush.~shared_ptr();
    }
    else if(t == Bitmap){
        bitmapParams.texture.~shared_ptr();
        bitmapParams.textureFence.~shared_ptr();
        bitmapParams.img.~shared_ptr();
    }
}

VisualCommand::~VisualCommand(){
    params._destroy(type);
}

Canvas::Canvas(CompositorClientProxy &proxy,Layer &layer): CompositorClient(proxy),rect(layer.getLayerRect()),layer(layer),current(new CanvasFrame {&layer,rect}){

};

// Layer * Canvas::getParentLayer(){
//     return parentLayer;
// };

void Canvas::drawRect(Core::Rect &rect, Core::SharedPtr<Brush> &brush){
    current->currentVisuals.push_back(VisualCommand(rect,brush,Core::Optional<Border>{}));
};

void Canvas::drawImage(SharedHandle<Media::BitmapImage> &img,const Core::Rect & rect) {
    current->currentVisuals.push_back(VisualCommand(img,rect));
}

void Canvas::drawGETexture(OmegaGTE::SharedHandle<OmegaGTE::GETexture> &img,const Core::Rect & rect,SharedHandle<OmegaGTE::GEFence> fence) {
    current->currentVisuals.push_back(VisualCommand(img,fence,rect));
}

SharedHandle<CanvasFrame> Canvas::getCurrentFrame() {
    return current;
}

SharedHandle<CanvasFrame> Canvas::nextFrame() {
    auto frame = getCurrentFrame();
    current.reset(new CanvasFrame {&layer,rect});
    return frame;
}

void Canvas::sendFrame() {
    auto frame = nextFrame();
    Timestamp ts = std::chrono::high_resolution_clock::now();
    pushFrame(frame,ts);
}

}
