#import "MTLBDTriangulator.h"

#import <Cocoa/Cocoa.h>

namespace OmegaWTK::Composition {

MTLBDTriangulator::MTLBDTriangulator(Core::Rect &targetFrame):targetFrame(targetFrame){};

Core::Math::Point2D MTLBDTriangulator::getRenderTargetCenter(){
    return {static_cast<float>(targetFrame.dimen.minWidth/2),static_cast<float>(targetFrame.dimen.minHeight/2)};
};

void MTLBDTriangulator::triangulateRect(Core::FRect rect,SolidColor2DMesh &res){
    SolidColorTriangle tri1;
    SolidColorTriangle tri2;
    
//    auto center = getRenderTargetCenter();
//    NSLog(@"Target Center: x:%i , y:%i",center.x,center.y);
    auto scaleFactor = [NSScreen mainScreen].backingScaleFactor;
    NSLog(@"Scale Factor:%f",scaleFactor);
    NSLog(@"Coords: x:%f, y:%f, w:%f, h:%f",rect.pos.x,rect.pos.y,rect.dimen.minWidth,rect.dimen.minHeight);
    float mtl_coord_x = (rect.pos.x)/(float(targetFrame.dimen.minWidth)) - 1.f;
    float mtl_coord_y = (rect.pos.y)/(float(targetFrame.dimen.minHeight)) - 1.f;
    
    float mtl_height = (rect.dimen.minHeight)/(float(targetFrame.dimen.minHeight));
    float mtl_width = (rect.dimen.minWidth)/(float(targetFrame.dimen.minWidth));
    
    NSLog(@"Metal Coords: x:%f , y:%f, w:%f , h:%f",mtl_coord_x,mtl_coord_y,mtl_width,mtl_height);
    
    tri2.a.position = tri1.a.position = {mtl_coord_x,mtl_coord_y + mtl_height};
    
    tri1.b.position = {mtl_coord_x + mtl_width,mtl_coord_y + mtl_height};
    tri2.b.position = {mtl_coord_x,mtl_coord_y};
    
    tri2.c.position = tri1.c.position = {mtl_coord_x + mtl_width,mtl_coord_y};
    
    res.push_back(std::move(tri1));
    res.push_back(std::move(tri2));
};

void MTLBDTriangulator::triangulateRect(Core::FRect rect,Textured2DMesh & res){
    TexturedTriangle tri1;
    TexturedTriangle tri2;
    
//    auto center = getRenderTargetCenter();
//    NSLog(@"Target Center: x:%i , y:%i",center.x,center.y);
    auto scaleFactor = [NSScreen mainScreen].backingScaleFactor;
    NSLog(@"Scale Factor:%f",scaleFactor);
//    NSLog(@"Coords: x:%i, y:%i, w:%u, h:%u",rect.pos.x,rect.pos.y,rect.dimen.minWidth,rect.dimen.minHeight);
    float mtl_coord_x = (rect.pos.x)/(float(targetFrame.dimen.minWidth)) - 1.f;
    float mtl_coord_y = (rect.pos.y)/(float(targetFrame.dimen.minHeight)) - 1.f;
    
    float mtl_height = (rect.dimen.minHeight)/(float(targetFrame.dimen.minHeight));
    float mtl_width = (rect.dimen.minWidth)/(float(targetFrame.dimen.minWidth));
    
//    NSLog(@"Metal Coords: x:%f , y:%f, w:%f , h:%f",mtl_coord_x,mtl_coord_y,mtl_width,mtl_height);
    
    tri2.a.position = tri1.a.position = {mtl_coord_x,mtl_coord_y + mtl_height};
    
    tri1.b.position = {mtl_coord_x + mtl_width,mtl_coord_y + mtl_height};
    tri2.b.position = {mtl_coord_x,mtl_coord_y};
    
    tri2.c.position = tri1.c.position = {mtl_coord_x + mtl_width,mtl_coord_y};
    
    res.push_back(std::move(tri1));
    res.push_back(std::move(tri2));
};

void MTLBDTriangulator::triangulateArc(Core::Math::Arc & arc,SolidColor2DMesh &res,float startAngle){
    float step = 0.005;
    float angle = 0;
    auto scaleFactor = [NSScreen mainScreen].backingScaleFactor;
    
    auto translateCoordinate = [&](float x,float y){
        simd_float2 pt;
        pt.x = (x)/(float(targetFrame.dimen.minWidth)) - 1.f;
        pt.y = (y)/(float(targetFrame.dimen.minHeight)) - 1.f;
        return pt;
    };
    
    NSLog(@"Start Point: x:%f, y:%f",float(arc.radius_x) * std::cos(startAngle) + float(arc.center.x),float(arc.radius_y) * std::sin(startAngle) + float(arc.center.y));
    
    auto mtl_center_coord = translateCoordinate(arc.center.x, arc.center.y);
    while(arc.radians >= angle){
        float x1 = float(arc.radius_x) * std::cos(startAngle) + float(arc.center.x);
        float y1 = float(arc.radius_y) * std::sin(startAngle) + float(arc.center.y);
        
        auto mtl_coord_a = translateCoordinate(x1, y1);
        
        angle += step;
        startAngle += step;
        
        float x2 = float(arc.radius_x) * std::cos(startAngle) + float(arc.center.x);
        float y2 = float(arc.radius_y) * std::sin(startAngle) + float(arc.center.y);
        
        auto mtl_coord_b = translateCoordinate(x2, y2);
        
        SolidColorTriangle tri;
        tri.a.position = mtl_coord_a;
        tri.b.position = mtl_coord_b;
        tri.c.position = mtl_center_coord;
        
        res.push_back(std::move(tri));
        angle += step;
        startAngle += step;
    };
};

Core::UniquePtr<MTLBDTriangulator::SolidColor2DMesh> MTLBDTriangulator::triangulateToSolidColorMesh(Core::FRect &rect,bool frame,unsigned frameWidth){
    auto rc = std::make_unique<SolidColor2DMesh>();
    if(frame){
        /// Bottom Rect
        triangulateRect(FRect(rect.pos.x,rect.pos.y,rect.dimen.minWidth,frameWidth),*rc);
        /// Left Rect
        triangulateRect(FRect(rect.pos.x,rect.pos.y + frameWidth,frameWidth,rect.dimen.minHeight - (frameWidth * 2)),*rc);
//        /// Top Rect
        triangulateRect(FRect(rect.pos.x,rect.pos.y + rect.dimen.minHeight - frameWidth,rect.dimen.minWidth,frameWidth),*rc);
//        /// Right Rect
        triangulateRect(FRect(rect.pos.x + rect.dimen.minWidth - frameWidth,rect.pos.y + frameWidth,frameWidth,rect.dimen.minHeight - (frameWidth * 2)),*rc);
    }
    else
        triangulateRect(rect,*rc);
    return rc;
};

Core::UniquePtr<MTLBDTriangulator::SolidColor2DMesh> MTLBDTriangulator::triangulateToSolidColorMesh(Core::FEllipse &ellipse,bool frame,unsigned frameWidth){
    auto rc = std::make_unique<SolidColor2DMesh>();
    if(frame){
        
    }
    else {
        Core::Math::Arc arc;
        arc.center.x = ellipse.pos.x;
        arc.center.y = ellipse.pos.y;
        arc.radius_x = ellipse.radius_x;
        arc.radius_y = ellipse.radius_y;
        arc.radians =  Core::Math::PI * 2.f;
        triangulateArc(arc,*rc,0);
    };
    return rc;
};

MTLBDTriangulator::TriangulationResult<MTLBDTriangulator::SolidColor2DMesh>  MTLBDTriangulator::triangulateToSolidColorMeshes(Core::FRoundedRect &rect,bool frame,unsigned frameWidth){
    auto rc = new SolidColor2DMesh();
    auto arcs = new SolidColor2DMesh();
    if(frame){
        
    }
    else {
        // Middle Rect
        auto middle = FRect(rect.pos.x + rect.radius_x,rect.pos.y + rect.radius_y,rect.dimen.minWidth - (2 * rect.radius_x),rect.dimen.minHeight - (2 * rect.radius_y));
        triangulateRect(middle,*rc);
        /// Bottom Rect
        auto bottom = FRect(rect.pos.x + rect.radius_x,rect.pos.y,rect.dimen.minWidth - (2 * rect.radius_x),rect.radius_y);
        triangulateRect(bottom,*rc);
//        /// Left Rect
        auto left = FRect(rect.pos.x,rect.pos.y + rect.radius_y,rect.radius_x,rect.dimen.minHeight - (2 * rect.radius_y));
        triangulateRect(left,*rc);
//        /// Top Rect
        auto top = FRect(rect.pos.x + rect.radius_x,rect.pos.y + rect.dimen.minHeight - rect.radius_y,rect.dimen.minWidth - (2 * rect.radius_x),rect.radius_y);
        triangulateRect(top,*rc);
        /// Right Rect
        auto right = FRect(rect.pos.x + rect.dimen.minWidth - rect.radius_x,rect.pos.y + rect.radius_y,rect.radius_x,rect.dimen.minHeight - (2 * rect.radius_y));
        triangulateRect(right,*rc);
        // Lower Left Arc
        Core::Math::Arc arc;
        arc.center.x = rect.pos.x + rect.radius_x;
        arc.center.y = rect.pos.y + rect.radius_y;
        arc.radius_x = rect.radius_x;
        arc.radius_y = rect.radius_y;
        arc.radians = Core::Math::PI / 2.f;
        triangulateArc(arc,*arcs,Core::Math::PI);
        // Upper Left Arc
        arc.center.y = rect.pos.y + rect.dimen.minHeight - rect.radius_y;
        triangulateArc(arc,*arcs,Core::Math::PI / 2.f);
        /// Upper Right Arc
        arc.center.x = rect.pos.x + rect.dimen.minWidth - rect.radius_x;
        triangulateArc(arc,*arcs,0.f);
        /// Lower Right Arc
        arc.center.y = rect.pos.y + rect.radius_y;
        triangulateArc(arc,*arcs,(3.f * Core::Math::PI) / 2.f);
    };
    
    
    return {rc,arcs};
};

Core::UniquePtr<MTLBDTriangulator::Textured2DMesh> MTLBDTriangulator::triangulateToTexturedMesh(Core::FRect & rect){
    auto rc = std::make_unique<Textured2DMesh>();
    triangulateRect(rect,*rc);
    return rc;
};

};
