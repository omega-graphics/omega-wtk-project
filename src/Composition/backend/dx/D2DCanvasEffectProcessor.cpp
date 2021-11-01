#include "../RenderTarget.h"

#include <d3d11on12.h>
#include <d2d1_1.h>
#include <d2d1effects_1.h>

#pragma comment(lib,"d2d1.lib")

namespace OmegaWTK::Composition {

    class D2DCanvasEffectProcessor : public BackendCanvasEffectProcessor {
        Core::UniqueComPtr<ID3D11On12Device> d3d11_device;
        ID3D12CommandQueue *commandQueue;
    public:
        explicit D2DCanvasEffectProcessor(OmegaGTE::SharedHandle<OmegaGTE::GEFence> & fence): BackendCanvasEffectProcessor(fence),
        d3d11_device(nullptr),
        commandQueue(nullptr){

        };
        void applyEffects(OmegaGTE::SharedHandle<OmegaGTE::GETexture> &dest, SharedHandle<OmegaGTE::GETextureRenderTarget> &textureTarget, OmegaCommon::Vector<std::pair<CanvasEffect::Type, void *>> &effects) override {
            if(d3d11_device.get() == nullptr){
                const D3D_FEATURE_LEVEL levels[]= {D3D_FEATURE_LEVEL_11_0};
                commandQueue = (ID3D12CommandQueue *)textureTarget->nativeCommandQueue();
                auto * dev = (ID3D12Device *)gte.graphicsEngine->underlyingNativeDevice();
                D3D11On12CreateDevice(dev,0,levels,1,(IUnknown *const *)&commandQueue,1,dev->GetNodeCount(),(ID3D11Device **)&d3d11_device,nullptr,nullptr);
            }

           auto src = textureTarget->underlyingTexture();

           ID3D11Texture2D *resource,*outRes;
           D3D11_RESOURCE_FLAGS flags;
            d3d11_device->CreateWrappedResource((IUnknown *)src->native(),&flags,D3D12_RESOURCE_STATE_RENDER_TARGET,D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,IID_PPV_ARGS(&resource));
            d3d11_device->CreateWrappedResource((IUnknown *)dest->native(),&flags,D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,IID_PPV_ARGS(&outRes));
            Core::UniqueComPtr<IDXGISurface> surface,surface2;
            resource->QueryInterface(IID_PPV_ARGS(&surface));
            outRes->QueryInterface(IID_PPV_ARGS(&surface2));
            Core::UniqueComPtr<ID2D1DeviceContext> deviceContext;
            D2D1CreateDeviceContext(surface2.get(),D2D1::CreationProperties(D2D1_THREADING_MODE_SINGLE_THREADED,D2D1_DEBUG_LEVEL_WARNING,D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS),&deviceContext);
            OmegaCommon::Vector<std::pair<ID2D1Effect *,ID2D1Image*>> effectPairs;
            ID2D1Bitmap1 *firstImg;
            deviceContext->CreateBitmapFromDxgiSurface(surface.get(),D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_CANNOT_DRAW),&firstImg);
            for(auto & eff : effects){
                ID2D1Image *input,*output;
                if(effectPairs.empty()){
                    input = firstImg;
                }
                else {
                    input = effectPairs.back().second;
                }
                ID2D1Effect *effect;
                switch (eff.first) {
                    case CanvasEffect::DirectionalBlur : {
                        auto _params = (CanvasEffect::DirectionalBlurParams *)eff.second;
                        deviceContext->CreateEffect(CLSID_D2D1DirectionalBlur,&effect);
                        effect->SetInput(0,input);
                        effect->SetValue(D2D1_DIRECTIONALBLUR_PROP_ANGLE,_params->angle);
                        effect->SetValue(D2D1_DIRECTIONALBLUR_PROP_STANDARD_DEVIATION,_params->radius);
                        effect->GetOutput(&output);
                        break;
                    }
                    case CanvasEffect::GaussianBlur : {
                        auto _params = (CanvasEffect::GaussianBlurParams *)eff.second;
                        deviceContext->CreateEffect(CLSID_D2D1GaussianBlur,&effect);
                        effect->SetInput(0,input);
                        effect->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION,_params->radius);
                        effect->GetOutput(&output);
                        break;
                    }
                }
                effectPairs.push_back(std::make_pair(effect,output));
            }
            deviceContext->BeginDraw();
            deviceContext->DrawImage(effectPairs.back().second);
            deviceContext->EndDraw();
            deviceContext->Flush();
            commandQueue->Signal((ID3D12Fence *)fence->native(),1);
            Core::SafeRelease(&firstImg);
            for(auto eff : effectPairs){
                Core::SafeRelease(&eff.first);
                Core::SafeRelease(&eff.second);
            }
            d3d11_device->ReleaseWrappedResources((ID3D11Resource *const *)&resource,1);
            d3d11_device->ReleaseWrappedResources((ID3D11Resource *const *)&outRes,1);
        }
        ~D2DCanvasEffectProcessor() override = default;
    };

    SharedHandle<BackendCanvasEffectProcessor>
    BackendCanvasEffectProcessor::Create(OmegaGTE::SharedHandle<OmegaGTE::GEFence> &fence) {
        return (SharedHandle<BackendCanvasEffectProcessor>)new D2DCanvasEffectProcessor(fence);
    }
}