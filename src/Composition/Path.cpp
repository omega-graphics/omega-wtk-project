#include "omegaWTK/Composition/Path.h"

namespace OmegaWTK::Composition {

    Path::Segment::Segment(OmegaGTE::GPoint2D startPoint):path(startPoint), final_path_a(startPoint), final_path_b(startPoint){

    };

    Path::Path(OmegaGTE::GPoint2D startPoint,unsigned initialStroke):
    segments(),
    currentStroke(initialStroke),
    arcPrecision(1.f/100.f),
    pathBrush(nullptr){
        goTo(startPoint);
    }

    void Path::goTo(OmegaGTE::GPoint2D pt) {
        segments.emplace_back(pt);
    }

    void Path::addLine(OmegaGTE::GPoint2D dest_pt) {
        auto & current = segments.back();

        float delta_x;
        float delta_y;


        auto & first = current.path.lastPt();
        /// m = tan(a)
        auto m = -(dest_pt.x - first.x)/(dest_pt.y - first.y);
        auto m_length = (float)currentStroke/2;
        auto cos_p = 1.f/std::sqrt(1 + (m * m));
        delta_x = cos_p * m_length;
        delta_y = std::sqrt((m * m) - (delta_x * delta_x));

        first = current.final_path_a.lastPt();
        first.x += delta_x;
        first.y += delta_y;

        first = current.final_path_b.lastPt();

        first.x -= delta_x;
        first.y -= delta_y;


        current.path.append(dest_pt);

        OmegaGTE::GPoint2D pt = dest_pt;
        pt.x += delta_x;
        pt.y += delta_y;

        current.final_path_a.append(pt);

        pt = dest_pt;

        pt.x -= delta_x;
        pt.y -= delta_y;

        current.final_path_b.append(pt);

    }

    void Path::addArc(OmegaGTE::GRect bounds,float startAngle,float endAngle) {
        auto & current = segments.back();

        float angle;

        if(current.path.size() == 0){
            angle = 0;
        }
        else {
            auto & pt = current.path.lastPt();
            auto & pt_other = current.final_path_a.lastPt();
            /// Get Angle of Path. (Perpendicular slope scalar from center point to path edge.)
            angle = std::atan(-(pt_other.x - pt.x)/(pt_other.y - pt.y));
        }

        auto pivot_x = bounds.pos.x + (bounds.w/2.f);
        auto pivot_y = bounds.pos.y + (bounds.h/2.f);

        auto width = float(currentStroke)/2.f;

        auto rad_x = bounds.w/2.f;
        auto rad_y = bounds.h/2.f;

        auto angle_it = startAngle;


        auto transform = [&](float x,float y,float * x_res,float * y_res){
            *x_res = (x * std::cos(angle) - y * std::sin(angle)) + pivot_x;
            *y_res = (x * std::sin(angle) + y * std::cos(angle)) + pivot_y;
        };

        while(angle_it <= endAngle){
            auto _cos = std::cos(angle_it);
            auto _sin = std::sin(angle_it);

            auto _x = rad_x * _cos;
            auto _y = rad_y * _sin;

            auto _x_l = (rad_x - width) * _cos;
            auto _x_r = (rad_x + width) * _cos;

            auto _y_l = (rad_y - width) * _sin;
            auto _y_r = (rad_y + width) * _sin;

            float _x_f,_y_f;
            transform(_x,_y,&_x_f,&_y_f);
            current.path.append(OmegaGTE::GPoint2D {_x_f,_y_f});

            transform(_x_l,_y_l,&_x_f,&_y_f);
            current.final_path_a.append(OmegaGTE::GPoint2D {_x_f,_y_f});

            transform(_x_r,_y_r,&_x_f,&_y_f);
            current.final_path_b.append(OmegaGTE::GPoint2D {_x_f,_y_f});

            angle_it += arcPrecision;
        }

    }

    Path::Path(OmegaGTE::GVectorPath2D & path,unsigned stroke):segments(),
                                                               currentStroke(stroke),
                                                               arcPrecision(1.f/100.f),
                                                               pathBrush(nullptr){
        auto path_it = path.begin();
        for(;path_it != path.end();path_it.operator++()){
            auto segment = *path_it;
            if(path_it == path.begin()){
                goTo(*segment.pt_A);
            }
            addLine(*segment.pt_B);
        }
    }

    void Path::setStroke(unsigned int newStroke) {
        assert(newStroke > 0 && "Stroke must be greater than 0");
        currentStroke = newStroke;
    };

    void Path::setArcPrecision(unsigned int newPrecision) {
        assert(newPrecision > 1 && "Precison must be greater than 1");
        arcPrecision = 1.f/float(newPrecision);
    }

    void Path::setPathBrush(Core::SharedPtr<Brush> &brush) {
        pathBrush = brush;
    }



Core::SharedPtr<Path> RoundedRectFrame(Core::RoundedRect rect,unsigned width){
    // float delta_w = width/2.f;
        
    // GVectorPath2D path_a({rect.pos.x + delta_w,rect.pos.y + rect.rad_y});
    // GVectorPath2D path_b({rect.pos.x - delta_w,rect.pos.y + rect.rad_y});
    // /// Left Side of Rect
    // float y_2 = rect.pos.y + rect.h - rect.rad_y;
    // path_a.append({rect.pos.x + delta_w,y_2});
    // path_b.append({rect.pos.x - delta_w,y_2});
    
    // /// Upper Left Corner
    // float step = 0.05;
    // float idx = PI;
    // idx -= step;
    
    // float arc_center_x_1 = rect.pos.x + rect.rad_x;
    // float arc_center_y_1 = rect.pos.y + rect.h - rect.rad_y;
    
    // while(idx >= (PI / 2.f)){
    //     float _cos = std::cosf(idx);
    //     float _sin = std::sinf(idx);
    //     path_a.append({(_cos * (rect.rad_x -delta_w)) + arc_center_x_1,(_sin * (rect.rad_y - delta_w)) + arc_center_y_1});
    //     path_b.append({(_cos * (rect.rad_x + delta_w)) + arc_center_x_1,(_sin * (rect.rad_y + delta_w)) + arc_center_y_1});
        
    //     idx -= step;
    // };
    // /// Top Side of Rect
    // auto x_2 = rect.pos.x + rect.rad_x;
    // path_a.append({x_2,rect.pos.y + rect.h - delta_w});
    // path_b.append({x_2,rect.pos.y + rect.h + delta_w});
    // /// Upper Right Corner
    // idx = PI / 2.f;
    // idx -= step;
    // float arc_center_x_2 = rect.pos.x + rect.w - rect.rad_x;
    // float arc_center_y_2 = rect.pos.y + rect.h - rect.rad_y;
    // while(idx >= 0.f){
    //     float _cos = std::cosf(idx);
    //     float _sin = std::sinf(idx);
    //     path_a.append({(_cos * (rect.rad_x -delta_w)) + arc_center_x_2,(_sin * (rect.rad_y - delta_w)) + arc_center_y_2});
    //     path_b.append({(_cos * (rect.rad_x + delta_w)) + arc_center_x_2,(_sin * (rect.rad_y + delta_w)) + arc_center_y_2});
        
    //     idx -= step;
    // };
    // /// Right Side of Rect
    // auto x_3 = rect.pos.x + rect.w;
    // auto y_3 = rect.pos.y + rect.h - rect.rad_y;
    // path_a.append({x_3 - delta_w,y_3});
    // path_b.append({x_3 + delta_w,y_3});
    // /// Lower Right Corner
    // idx = 2 * PI;
    // idx -= step;
    // float arc_center_x_3 = rect.pos.x + rect.w - rect.rad_x;
    // float arc_center_y_3 = rect.pos.y + rect.rad_y;
    // while(idx >= ((3 * PI) / 2.f)){
    //     float _cos = std::cosf(idx);
    //     float _sin = std::sinf(idx);
    //     path_a.append({(_cos * (rect.rad_x -delta_w)) + arc_center_x_3,(_sin * (rect.rad_y - delta_w)) + arc_center_y_3});
    //     path_b.append({(_cos * (rect.rad_x + delta_w)) + arc_center_x_3,(_sin * (rect.rad_y + delta_w)) + arc_center_y_3});
        
    //     idx -= step;
    // };
    
    // auto x_4 = rect.pos.x + rect.w - rect.rad_x;
    // /// Bottom Side
    // path_a.append({x_4,rect.pos.y - delta_w});
    // path_b.append({x_4,rect.pos.y + delta_w});
    // /// Lower Left Corner
    // idx = ((3 * PI) / 2.f);
    // idx -= step;
    // float arc_center_x_4 = rect.pos.x + rect.rad_x;
    // float arc_center_y_4 = rect.pos.y + rect.rad_y;
    // while(idx >= PI){
    //     float _cos = std::cosf(idx);
    //     float _sin = std::sinf(idx);
    //     path_a.append({(_cos * (rect.rad_x -delta_w)) + arc_center_x_4,(_sin * (rect.rad_y - delta_w)) + arc_center_y_4});
    //     path_b.append({(_cos * (rect.rad_x + delta_w)) + arc_center_x_4,(_sin * (rect.rad_y + delta_w)) + arc_center_y_4});
        
    //     idx -= step;
    // };
    // std::cout << path_a.toStr();
    // std::cout << path_b.toStr();
    // return std::make_shared<GeometricGraphicsPath>(path_a,path_b);
    return nullptr;
};

    Core::SharedPtr<Path> EllipseFrame(Core::Ellipse ellipse,unsigned width){
        // float delta_w = width/2.f;
        
        // GVectorPath2D path_a ({ellipse.x + ellipse.rad_x + delta_w,ellipse.y});
        // GVectorPath2D path_b ({ellipse.x + ellipse.rad_x - delta_w,ellipse.y});
        // float step = 0.05;
        // float idx = 0;
        // idx += step;
        // while(idx <= 2 * PI){
        //     float _cos = std::cosf(idx);
        //     float _sin = std::sinf(idx);
            
        //     path_a.append({((ellipse.rad_x + delta_w)*_cos) + ellipse.x,((ellipse.rad_y +delta_w)*_sin)+ ellipse.y});
        //     path_b.append({((ellipse.rad_x - delta_w)*_cos) + ellipse.x,((ellipse.rad_y -delta_w)*_sin)+ ellipse.y});
            
        //     idx += step;
        // };
        // std::cout << path_a.toStr();
        // std::cout << path_b.toStr();
        // return std::make_shared<GeometricGraphicsPath>(path_a,path_b);
        return nullptr;
    };
};
