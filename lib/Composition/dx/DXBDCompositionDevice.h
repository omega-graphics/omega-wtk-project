#include "../BDCompositionDevice.h"

#include <dcomp.h>
#include <d3d11.h>
#include <d2d1.h>
#include <d2d1_1.h>
#include <d2d1_1helper.h>
#include <d2d1effects.h>
#include <dwrite.h>

#pragma comment(lib, "runtimeobject.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dcomp.lib")
#pragma comment(lib, "dwrite.lib")

#ifndef OMEGAWTK_COMPOSITION_DX_DXBDCOMPOSITIONDEVICE_H
#define OMEGAWTK_COMPOSITION_DX_DXBDCOMPOSITIONDEVICE_H



namespace OmegaWTK::Composition {
     class None {};
    class DXBDCompositionLayerRenderTarget;
    class DXBDCompositonImageRenderTarget;
    class DXBDCompositionDevice : public BDCompositionDevice {
        Core::UniqueComPtr<ID3D11Device> direct3d_device;
        Core::UniqueComPtr<ID2D1Factory1> direct2d_factory;
        Core::UniqueComPtr<ID2D1Device> direct2d_device;
        Core::UniqueComPtr<IDXGIFactory2> dxgi_factory;
        Core::UniqueComPtr<IDXGIDevice1> dxgi_device;
        Core::UniqueComPtr<IDXGIAdapter> dxgi_adapter;
        Core::UniqueComPtr<IDCompositionDevice3> dcomp_device;
        friend class DXBDCompositionLayerRenderTarget;
        friend class DXBDCompositionImageRenderTarget;
        public:
        DXBDCompositionDevice();
        ~DXBDCompositionDevice();
        static Core::SharedPtr<BDCompositionDevice> Create();
        Core::SharedPtr<BDCompositionLayerRenderTarget> makeLayerRenderTarget(Layer *layer);
        Core::SharedPtr<BDCompositionImageRenderTarget> makeImageRenderTarget(Core::Rect &size);
        Core::SharedPtr<BDCompositionImageRenderTarget> makeImageRenderTarget(Core::SharedPtr<BDCompositionImage> &img);
        Core::SharedPtr<BDCompositionFontFactory> createFontFactory();
        void destroyTarget(Layer *layer, Core::SharedPtr<BDCompositionRenderTarget> &target);
    };
};

#endif