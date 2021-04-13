#include "MTLBDCompositionDevice.h"
#include "omegaWTK/Core/Math.h"

#include "MetalBridgeHeader.h"

#ifndef OMEGAWTK_COMPOSITION_MTL_MTLBDTRIANGULATOR_H
#define OMEGAWTK_COMPOSITION_MTL_MTLBDTRIANGULATOR_H

namespace OmegaWTK::Composition {
/**
 Converts Geometric Shapes/VectorPaths into 2D Meshes. (Triangle Vectors)
 NOTE: Rect Position Inputs are NOT scaled to the backingScaleFactor however the Dimenensions are.
 */
class MTLBDTriangulator {
    Core::Rect & targetFrame;
    Core::Math::Point2D getRenderTargetCenter();
    CGFloat scaleFactor;
public:
    bool isImageTarget = false;
    template<typename _Vertex_Ty>
    struct Triangle {
        _Vertex_Ty a;
        _Vertex_Ty b;
        _Vertex_Ty c;
    };
    /**
     @name Type Definitions
     @{
     */
    typedef Triangle<MTLBDSolidColorVertex> SolidColorTriangle;
    typedef Triangle<MTLBDTexture2DVertex> TexturedTriangle;
    
    template<typename _Triangle_Ty>
    using Mesh = Core::Vector<_Triangle_Ty>;
    typedef Mesh<SolidColorTriangle> SolidColor2DMesh;
    typedef Mesh<TexturedTriangle> Textured2DMesh;
    /// Mutli Mesh Returns!
    template<typename Mesh_Ty>
    using TriangulationResult = Core::Vector<Mesh_Ty *>;
    /// @}
    /**
      @name Triangulation Methods
      NOTE: The Geometric Primitive argument given by the composition backend MUST be already DPI scaled!
     @{
     */
private:
    void triangulateRect(Core::FRect rect,SolidColor2DMesh & res);
    void triangulateRect(Core::FRect rect,Textured2DMesh & res);
    
    void triangulateArc(Core::Math::Arc & arc,SolidColor2DMesh &res,float startAngle);
public:
    void setScaleFactor(CGFloat scaleFactor){ this->scaleFactor = scaleFactor;};
    Core::UniquePtr<SolidColor2DMesh> triangulateToSolidColorMesh(Core::FRect & rect,bool frame = false,unsigned frameWidth = 0);
    TriangulationResult<SolidColor2DMesh> triangulateToSolidColorMeshes(Core::FRoundedRect &rect,bool frame = false,unsigned frameWidth = 0);
    Core::UniquePtr<SolidColor2DMesh> triangulateToSolidColorMesh(Core::FEllipse &ellipse,bool frame = false,unsigned frameWidth = 0);
//    Core::UniquePtr<SolidColor2DMesh> triangulateToSolidColorMesh(Core::RoundedRect & rect);
    
    Core::UniquePtr<Textured2DMesh> triangulateToTexturedMesh(Core::FRect & rect);
    /// @}
    MTLBDTriangulator(Core::Rect & targetFrame);
};

};

#endif
