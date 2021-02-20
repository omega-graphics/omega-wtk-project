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
}