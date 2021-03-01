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

void Visual::setColor(const Color &new_color) {
  // VISUAL_SET_PARAMS(params->color = std::move(new_color);)
};
void Visual::setFont(const Text::Font &new_font){
    if(type == Text){
        TextParams *params = (TextParams *)this->params;
        params->text.setFont(std::move(new_font));
    };
};
void Visual::setRect(const Core::Rect &new_rect){
    
};

// VPVR Visual::getRect(){

// };

// VPVR Visual::getColor(){

// };

// VPVR Visual::getFont(){

// };
Style::Style(){
    
};

void Style::add(Visual::RectParams params,std::initializer_list<VisualEffect> effects){
    _construct_visual(Visual::Rect,params,effects);
};

void Style::add(Visual::RoundedRectParams params,std::initializer_list<VisualEffect> effects){
    _construct_visual(Visual::RoundedRect,params,effects);
};

void Style::add(Visual::EllipseParams params,std::initializer_list<VisualEffect> effects){
    _construct_visual(Visual::Ellipse,params,effects);
};

void Style::add(Visual::BitmapParams params,std::initializer_list<VisualEffect> effects){
    _construct_visual(Visual::Bitmap,params,effects);
};

void Style::add(Visual::TextParams params, std::initializer_list<VisualEffect> effects){
    _construct_visual(Visual::Text,params,effects);
};

void Style::setBrush(unsigned id,const Core::SharedPtr<Brush> & new_brush){
    auto visual = visuals[id].get();
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

}
