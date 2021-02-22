#import "MTLBDTriangulator.h"

namespace OmegaWTK::Composition {
MTLBDTriangulator::MTLBDTriangulator(Core::Rect &targetFrame):targetFrame(targetFrame){};

Core::Math::Point2D MTLBDTriangulator::getRenderTargetCenter(){
    return {targetFrame.dimen.minWidth/2,targetFrame.dimen.minHeight/2};
};

void MTLBDTriangulator::triangulateRect(Core::Rect & rect,SolidColor2DMesh &res){
    SolidColorTriangle tri1;
    SolidColorTriangle tri2;
    
    auto center = getRenderTargetCenter();
    NSLog(@"Target Center: x:%i , y:%i",center.x,center.y);
    float mtl_coord_x = (float(rect.pos.x) / float(center.x)) - 1.f;
    float mtl_coord_y = (float(rect.pos.y) / float(center.y)) - 1.f;
    
    float mtl_height = float(rect.dimen.minHeight)/float(targetFrame.dimen.minHeight);
    float mtl_width = float(rect.dimen.minWidth)/float(targetFrame.dimen.minWidth);
    
    NSLog(@"Metal Coords: x:%f , y:%f, w:%f , h:%f",mtl_coord_x,mtl_coord_y,mtl_width,mtl_height);
    
    tri2.a.position = tri1.a.position = {mtl_coord_x,mtl_coord_y + mtl_height};
    
    tri1.b.position = {mtl_coord_x + mtl_width,mtl_coord_y + mtl_height};
    tri2.b.position = {mtl_coord_x,mtl_coord_y};
    
    tri2.c.position = tri1.c.position = {mtl_coord_x + mtl_width,mtl_coord_y};
    
    res.push_back(std::move(tri1));
    res.push_back(std::move(tri2));
};

Core::UniquePtr<MTLBDTriangulator::SolidColor2DMesh> MTLBDTriangulator::triangulateToSolidColorMesh(Core::Rect &rect){
    auto rc = std::make_unique<MTLBDTriangulator::SolidColor2DMesh>();
    triangulateRect(rect,*rc);
    return rc;
};

};
