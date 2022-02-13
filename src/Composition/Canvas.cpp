#include "omegaWTK/Composition/Canvas.h"
#include "omegaWTK/Composition/CompositorClient.h"
#include "omegaWTK/Composition/Layer.h"

namespace OmegaWTK::Composition {


// #define VISUAL_SET_PARAMS(arg)                                                 \
//   switch (type) {                                                              \
//   case Rect: {                                                                 \
//     RectParams *params = (RectParams *)this->params;                           \
//     arg break;                                                                 \
//   };                                                                           \
//   case Text: {                                                                 \
//     TextParams *params = (TextParams *)this->params;                           \
//     arg break;                                                                 \
//   }                                                                            \
//   case RoundedRect: {                                                          \
//     RoundedRectParams *params = (RoundedRectParams *)this->params;             \
//     arg break;                                                                 \
//   }                                                                            \
//   case Ellipse: {                                                              \
//     EllipseParams *params = (EllipseParams *)this->params;                     \
//     arg break;                                                                 \
//   }                                                                            \
//   };

// void Visual::setColor(const Color &new_color) {
//   // VISUAL_SET_PARAMS(params->color = std::move(new_color);)
// };
// //void Visual::setFont(const Text::Font &new_font){
// //    if(type == Text){
// //        TextParams *params = (TextParams *)this->params;
// //        params->text.setFont(std::move(new_font));
// //    };
// //};
// void Visual::setRect(const Core::Rect &new_rect){
    
// };

// VPVR Visual::getRect(){

// };

// VPVR Visual::getColor(){

// };

// VPVR Visual::getFont(){

// };


// LayerStyle::LayerStyle(){
    
// };

Canvas::Canvas(CompositorClientProxy &proxy,Layer &layer): CompositorClient(proxy),rect(layer.getLayerRect()),layer(layer),current(new CanvasFrame {&layer,rect}){

};

// Layer * Canvas::getParentLayer(){
//     return parentLayer;
// };

void Canvas::drawRect(Core::Rect &rect, Core::SharedPtr<Brush> &brush){
    current->currentVisuals.push_back(VisualCommand {VisualCommand::Rect,new VisualCommand::RectParams {rect,brush}});
};

void Canvas::drawImage(SharedHandle<Media::BitmapImage> &img,const Core::Rect & rect) {
    current->currentVisuals.push_back(VisualCommand {VisualCommand::Bitmap,new VisualCommand::BitmapParams {img,nullptr,nullptr,rect}});
}

void Canvas::drawGETexture(OmegaGTE::SharedHandle<OmegaGTE::GETexture> &img,const Core::Rect & rect,SharedHandle<OmegaGTE::GEFence> fence) {
    current->currentVisuals.push_back(VisualCommand {VisualCommand::Bitmap,new VisualCommand::BitmapParams {nullptr,img,fence,rect}});
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

// void CanvasSurface::drawTextRect(SharedHandle<Composition::TextRect> &textRect, Core::SharedPtr<Brush> &brush){
//     auto comm = new VisualCommand {VisualCommand::Text,new VisualCommand::TextParams {textRect,brush}};
//     submitCommandToClient(comm);
// };


// void LayerStyle::add(Visual::RectParams params){
//     _construct_visual(Visual::Rect,params);
// };

// void LayerStyle::add(Visual::RoundedRectParams params){
//     _construct_visual(Visual::RoundedRect,params);
// };

// void LayerStyle::add(Visual::EllipseParams params){
//     _construct_visual(Visual::Ellipse,params);
// };

// void LayerStyle::add(Visual::BitmapParams params){
//     _construct_visual(Visual::Bitmap,params);
// };

// void LayerStyle::add(Visual::TextParams params){
//     _construct_visual(Visual::Text,params);
// };

// void LayerStyle::setBrush(unsigned id,const Core::SharedPtr<Brush> & new_brush){
//     auto visual = visuals[id];
//     switch (visual->type) {
//         case Visual::Rect :{
//             Visual::RectParams *params = (Visual::RectParams *)visual->params;
//             params->brush.reset(new Brush(*new_brush));
//             break;
//         }
//         default:
//             break;
//     }
// };

// void LayerStyle::addEffect(SharedHandle<LayerEffect> & effect){
//     effects.push_back(effect);
// };

// LayerStyle::~LayerStyle(){
//     auto visual_it = visuals.begin();
//     while(visual_it != visuals.end()){
//         delete *visual_it;
//         ++visual_it;
//     };
// };

// WindowStyle::~WindowStyle(){
//     // auto visual_it = visuals.begin();
//     // while(visual_it != visuals.end()){
//     //     delete *visual_it;
//     //     ++visual_it;
//     // };
// };

}
