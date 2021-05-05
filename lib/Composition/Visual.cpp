#include "omegaWTK/Composition/Visual.h"

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
LayerStyle::LayerStyle(){
    
};

void LayerStyle::add(Visual::RectParams params){
    _construct_visual(Visual::Rect,params);
};

void LayerStyle::add(Visual::RoundedRectParams params){
    _construct_visual(Visual::RoundedRect,params);
};

void LayerStyle::add(Visual::EllipseParams params){
    _construct_visual(Visual::Ellipse,params);
};

void LayerStyle::add(Visual::BitmapParams params){
    _construct_visual(Visual::Bitmap,params);
};

void LayerStyle::add(Visual::TextParams params){
    _construct_visual(Visual::Text,params);
};

void LayerStyle::setBrush(unsigned id,const Core::SharedPtr<Brush> & new_brush){
    auto visual = visuals[id];
    switch (visual->type) {
        case Visual::Rect :{
            Visual::RectParams *params = (Visual::RectParams *)visual->params;
            params->brush.reset(new Brush(*new_brush));
            break;
        }
        default:
            break;
    }
};

void LayerStyle::addEffect(SharedHandle<LayerEffect> & effect){
    effects.push_back(effect);
};

LayerStyle::~LayerStyle(){
    auto visual_it = visuals.begin();
    while(visual_it != visuals.end()){
        delete *visual_it;
        ++visual_it;
    };
};

WindowStyle::~WindowStyle(){
    // auto visual_it = visuals.begin();
    // while(visual_it != visuals.end()){
    //     delete *visual_it;
    //     ++visual_it;
    // };
};

}
