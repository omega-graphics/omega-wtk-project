#include "omegaWTK/Composition/GeometricGraphicsPath.h"

namespace OmegaWTK::Composition {
using namespace OmegaGTE;
    GeometricGraphicsPath::GeometricGraphicsPath(GVectorPath2D & path_a,GVectorPath2D & path_b):path_a(path_a),path_b(path_b){
        
    };

    void GeometricGraphicsPath::write(OmegaGTE::GPoint2D pt_a,OmegaGTE::GPoint2D pt_b){
        path_a.append(pt_a);
        path_b.append(pt_b);
    };

Core::SharedPtr<GeometricGraphicsPath> RoundedRectFrame(Core::RoundedRect rect,unsigned width){
    float delta_w = width/2.f;
        
    GVectorPath2D path_a({rect.pos.x + delta_w,rect.pos.y + rect.rad_y});
    GVectorPath2D path_b({rect.pos.x - delta_w,rect.pos.y + rect.rad_y});
    /// Left Side of Rect
    float y_2 = rect.pos.y + rect.h - rect.rad_y;
    path_a.append({rect.pos.x + delta_w,y_2});
    path_b.append({rect.pos.x - delta_w,y_2});
    
    /// Upper Left Corner
    float step = 0.05;
    float idx = PI;
    idx -= step;
    
    float arc_center_x_1 = rect.pos.x + rect.rad_x;
    float arc_center_y_1 = rect.pos.y + rect.h - rect.rad_y;
    
    while(idx >= (PI / 2.f)){
        float _cos = std::cosf(idx);
        float _sin = std::sinf(idx);
        path_a.append({(_cos * (rect.rad_x -delta_w)) + arc_center_x_1,(_sin * (rect.rad_y - delta_w)) + arc_center_y_1});
        path_b.append({(_cos * (rect.rad_x + delta_w)) + arc_center_x_1,(_sin * (rect.rad_y + delta_w)) + arc_center_y_1});
        
        idx -= step;
    };
    /// Top Side of Rect
    auto x_2 = rect.pos.x + rect.rad_x;
    path_a.append({x_2,rect.pos.y + rect.h - delta_w});
    path_b.append({x_2,rect.pos.y + rect.h + delta_w});
    /// Upper Right Corner
    idx = PI / 2.f;
    idx -= step;
    float arc_center_x_2 = rect.pos.x + rect.w - rect.rad_x;
    float arc_center_y_2 = rect.pos.y + rect.h - rect.rad_y;
    while(idx >= 0.f){
        float _cos = std::cosf(idx);
        float _sin = std::sinf(idx);
        path_a.append({(_cos * (rect.rad_x -delta_w)) + arc_center_x_2,(_sin * (rect.rad_y - delta_w)) + arc_center_y_2});
        path_b.append({(_cos * (rect.rad_x + delta_w)) + arc_center_x_2,(_sin * (rect.rad_y + delta_w)) + arc_center_y_2});
        
        idx -= step;
    };
    /// Right Side of Rect
    auto x_3 = rect.pos.x + rect.w;
    auto y_3 = rect.pos.y + rect.h - rect.rad_y;
    path_a.append({x_3 - delta_w,y_3});
    path_b.append({x_3 + delta_w,y_3});
    /// Lower Right Corner
    idx = 2 * PI;
    idx -= step;
    float arc_center_x_3 = rect.pos.x + rect.w - rect.rad_x;
    float arc_center_y_3 = rect.pos.y + rect.rad_y;
    while(idx >= ((3 * PI) / 2.f)){
        float _cos = std::cosf(idx);
        float _sin = std::sinf(idx);
        path_a.append({(_cos * (rect.rad_x -delta_w)) + arc_center_x_3,(_sin * (rect.rad_y - delta_w)) + arc_center_y_3});
        path_b.append({(_cos * (rect.rad_x + delta_w)) + arc_center_x_3,(_sin * (rect.rad_y + delta_w)) + arc_center_y_3});
        
        idx -= step;
    };
    
    auto x_4 = rect.pos.x + rect.w - rect.rad_x;
    /// Bottom Side
    path_a.append({x_4,rect.pos.y - delta_w});
    path_b.append({x_4,rect.pos.y + delta_w});
    /// Lower Left Corner
    idx = ((3 * PI) / 2.f);
    idx -= step;
    float arc_center_x_4 = rect.pos.x + rect.rad_x;
    float arc_center_y_4 = rect.pos.y + rect.rad_y;
    while(idx >= PI){
        float _cos = std::cosf(idx);
        float _sin = std::sinf(idx);
        path_a.append({(_cos * (rect.rad_x -delta_w)) + arc_center_x_4,(_sin * (rect.rad_y - delta_w)) + arc_center_y_4});
        path_b.append({(_cos * (rect.rad_x + delta_w)) + arc_center_x_4,(_sin * (rect.rad_y + delta_w)) + arc_center_y_4});
        
        idx -= step;
    };
    std::cout << path_a.toStr();
    std::cout << path_b.toStr();
    return std::make_shared<GeometricGraphicsPath>(path_a,path_b);
};

    Core::SharedPtr<GeometricGraphicsPath> EllipseFrame(Core::Ellipse ellipse,unsigned width){
        float delta_w = width/2.f;
        
        GVectorPath2D path_a ({ellipse.x + ellipse.rad_x + delta_w,ellipse.y});
        GVectorPath2D path_b ({ellipse.x + ellipse.rad_x - delta_w,ellipse.y});
        float step = 0.05;
        float idx = 0;
        idx += step;
        while(idx <= 2 * PI){
            float _cos = std::cosf(idx);
            float _sin = std::sinf(idx);
            
            path_a.append({((ellipse.rad_x + delta_w)*_cos) + ellipse.x,((ellipse.rad_y +delta_w)*_sin)+ ellipse.y});
            path_b.append({((ellipse.rad_x - delta_w)*_cos) + ellipse.x,((ellipse.rad_y -delta_w)*_sin)+ ellipse.y});
            
            idx += step;
        };
        std::cout << path_a.toStr();
        std::cout << path_b.toStr();
        return std::make_shared<GeometricGraphicsPath>(path_a,path_b);
    };
};
