#include "omegaWTK/Composition/Color.h"

namespace OmegaWTK::Composition {
Color::Color(unsigned red,unsigned green,unsigned blue,unsigned alpha):r(red),g(green),b(blue),a(alpha){};

Color::Color(STDColor ty){
   a = 255;
   if(ty == Color::Red){
      r = 255;
      g = 0;
      b = 0;
   }
   else if(ty == Color::Blue){
      b = 255;
      g = 0;
      r = 0;
   }
   else if(ty == Color::Green){
      g = 255;
      r = 0;
      b = 0;
   };
};

bool Color::compare(const Color &other){
   return (r == other.r) && (g == other.g) &&(b == other.b) && (a == other.a); 
};


};
