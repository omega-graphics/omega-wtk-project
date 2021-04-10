#include "omegaWTK/Composition/Color.h"

namespace OmegaWTK::Composition {
Color::Color(unsigned red,unsigned green,unsigned blue,unsigned alpha):r(red),g(green),b(blue),a(alpha){};

Color::Color(std::uint32_t hex_color,std::uint8_t alpha){
   uint8_t mask = 0xFF;
   /// BB
   b = hex_color & mask;
   hex_color = hex_color >> 8;
   /// GG
   g = hex_color & mask;
   hex_color = hex_color >> 8;
   /// RR
   r = hex_color & mask;

   a = alpha;
};

bool Color::compare(const Color &other){
   return (r == other.r) && (g == other.g) &&(b == other.b) && (a == other.a); 
};

const Color Color::Transparent = Color(Color::Black,0x00);

Gradient::GradientStop Gradient::CreateStop(float pos, Color color){
   return {pos,color};
};

Gradient Gradient::CreateLinear(std::initializer_list<GradientStop> stops){
   Gradient grad;
   grad.stops = stops;
   grad.type = Linear;
   return grad;
};

Gradient Gradient::CreateRadial(std::initializer_list<GradientStop> stops){
   Gradient grad;
   grad.stops = stops;
   grad.type = Radial;
   return grad;
};


};
