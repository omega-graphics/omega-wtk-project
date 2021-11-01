#include "DCVisualTree.h"
#include <iostream>
#include <memory>
#include "NativePrivate/win/HWNDItem.h"



namespace OmegaWTK::Composition {

    IDCompositionDevice3 *comp_device = nullptr;
    IDCompositionDesktopDevice *comp_device_desktop = nullptr;
    // IDCompositionDevice3 *comp_device_2 = nullptr;

    SharedHandle<BackendVisualTree> BackendVisualTree::Create(SharedHandle<ViewRenderTarget> & view){
        return SharedHandle<BackendVisualTree>(new DCVisualTree(view));
    }

    
    DCVisualTree::DCVisualTree(SharedHandle<ViewRenderTarget> & view){
        if(comp_device == nullptr) {
            IDCompositionDesktopDevice *dev;
            auto hr = DCompositionCreateDevice3(NULL,IID_PPV_ARGS(&dev));
            if(FAILED(hr)){

            };
            dev->QueryInterface(IID_PPV_ARGS(&comp_device));
            comp_device_desktop = dev;
        }

        auto hwndItem = std::dynamic_pointer_cast<Native::Win::HWNDItem>(view->getNativePtr());
        HRESULT res = comp_device_desktop->CreateTargetForHwnd(hwndItem->hwnd,TRUE,&hwndTarget.comPtr);
    };

     DCVisualTree::Visual::Visual(Core::Position &pos,BackendRenderTargetContext & context, IDCompositionVisual2 * visual, IDXGISwapChain3 *swapChain):
     Parent::Visual(pos,context),
     swapChain(swapChain),
     visual(visual){
        
     };

    void DCVisualTree::Visual::resize(Core::Rect &newRect){
        swapChain->ResizeBuffers(2,(UINT)newRect.w,(UINT)newRect.h,DXGI_FORMAT_R8G8B8A8_UNORM,DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING);
    }

    void DCVisualTree::Visual::updateShadowEffect(LayerEffect::DropShadowParams &params) {
        if(shadowVisual == nullptr){
            comp_device->CreateShadowEffect(&shadowEffect);
            comp_device_desktop->CreateVisual(&shadowVisual);
            shadowVisual->SetOpacityMode(DCOMPOSITION_OPACITY_MODE_INHERIT);
            if(parent != nullptr){
                parent->AddVisual(shadowVisual,FALSE,visual);
            }
        }
        shadowEffect->SetColor(D2D1::Vector4F(params.color.r,params.color.g,params.color.b,params.color.a * params.opacity));
        shadowEffect->SetStandardDeviation(params.blurAmount);
        shadowVisual->SetOffsetX(params.x_offset);
        shadowVisual->SetOffsetY(params.y_offset);
        shadowVisual->SetEffect(shadowEffect);
        comp_device->Commit();
        comp_device->WaitForCommitCompletion();
    }

    void DCVisualTree::Visual::updateTransformEffect(LayerEffect::TransformationParams &params) {
        if(transformEffect == nullptr){
            comp_device->CreateMatrixTransform3D(&transformEffect);
        }
        D2D1_MATRIX_4X4_F matrix =
                D2D1::Matrix4x4F::Translation(params.translate.x,params.translate.y,params.translate.z)
                * D2D1::Matrix4x4F::RotationX(params.rotate.roll)
                * D2D1::Matrix4x4F::RotationY(params.rotate.yaw)
                * D2D1::Matrix4x4F::RotationZ(params.rotate.pitch)
                * D2D1::Matrix4x4F::Scale(params.scale.x,params.scale.y,params.scale.z);
        D3DMATRIX m;
        memcpy(m.m,matrix.m,sizeof(matrix.m));
        transformEffect->SetMatrix(m);
        visual->SetEffect(transformEffect);
        comp_device->Commit();
        comp_device->WaitForCommitCompletion();
    }

    DCVisualTree::Visual::~Visual(){
        visual->RemoveAllVisuals();
        Core::SafeRelease(&visual);
    };

    Core::SharedPtr<BackendVisualTree::Visual> DCVisualTree::makeVisual(Core::Rect & rect,Core::Position & pos){
        
        OmegaGTE::NativeRenderTargetDescriptor desc {};
        desc.isHwnd = false;
        desc.hwnd = nullptr;
        desc.height = (unsigned)rect.h;
        desc.width = (unsigned)rect.w;

        auto target = gte.graphicsEngine->makeNativeRenderTarget(desc);
        auto swapChain = (IDXGISwapChain3 *)target->getSwapChain();

        BackendRenderTargetContext context {rect,target};
        
        HRESULT hr;
        IDCompositionVisual2 *v;
        hr = comp_device->CreateVisual(&v);
        if(FAILED(hr)){

        };

        hr = v->SetContent(swapChain);
        if(FAILED(hr)){
            std::stringstream ss;
            ss << std::hex << hr;
            MessageBoxA(HWND_DESKTOP,(std::string("Failed to set Content of Visual. ERROR:") + ss.str()).c_str(),NULL,MB_OK);
        };
        // rc.visual = nullptr;
        return (SharedHandle<Parent::Visual>)new DCVisualTree::Visual {pos,context,v,swapChain};
    };

    void DCVisualTree::setRootVisual(Core::SharedPtr<Parent::Visual> & visual){
        root = visual;
        auto v = std::dynamic_pointer_cast<Visual>(visual);
        hwndTarget->SetRoot(v->visual);
        comp_device_desktop->Commit();
        comp_device_desktop->WaitForCommitCompletion();
    };


    void DCVisualTree::addVisual(Core::SharedPtr<Parent::Visual> & visual){
        body.push_back(visual);
        auto r = std::dynamic_pointer_cast<Visual>(root),v =  std::dynamic_pointer_cast<Visual>(visual);
        r->visual->AddVisual(v->visual,TRUE,nullptr);
        if(v->shadowVisual != nullptr){
            r->visual->AddVisual(v->shadowVisual,FALSE,v->visual);
        }
        v->parent = r->visual;
        comp_device_desktop->Commit();
        comp_device_desktop->WaitForCommitCompletion();
    };



};