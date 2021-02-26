#include "omegaWTK/Composition/GeometricGraphicsPath.h"

namespace OmegaWTK::Composition {
using namespace Core::Math;
    GeometricGraphicsPath::GeometricGraphicsPath(VectorPath2D & path_a,VectorPath2D & path_b):path_a(path_a),path_b(path_b){
        
    };

    Core::SharedPtr<GeometricGraphicsPath> EllipseFrame(Core::Ellipse ellipse,unsigned width){
        auto delta_w = width/2;
        
        VectorPath2D path_a ({ellipse.pos.x + ellipse.radius_x + delta_w,ellipse.pos.y});
        VectorPath2D path_b ({ellipse.pos.x + ellipse.radius_x - delta_w,ellipse.pos.y});
        float step = 0.05;
        float idx = 0;
        idx += step;
        while(idx <= 2 * PI){
            float _cos = std::cosf(idx);
            float _sin = std::sinf(idx);
            
            path_a.append({static_cast<unsigned int>(((ellipse.radius_x + delta_w)*_cos) + ellipse.pos.x),static_cast<unsigned int>(((ellipse.radius_y +delta_w)*_sin)+ ellipse.pos.y)});
            path_b.append({static_cast<unsigned int>(((ellipse.radius_x - delta_w)*_cos) + ellipse.pos.x),static_cast<unsigned int>(((ellipse.radius_y -delta_w)*_sin)+ ellipse.pos.y)});
            
            idx += step;
        };
        std::cout << path_a.toStr();
        std::cout << path_b.toStr();
        return std::make_shared<GeometricGraphicsPath>(path_a,path_b);
    };
};
