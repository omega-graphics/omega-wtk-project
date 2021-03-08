#include "../BDCompositionDevice.h"

#include <dcomp.h>
#ifdef DIRECT3D_12
#include <d3d12.h>
#include <d3d11on12.h>
#include <dxgi1_4.h>
#pragma comment(lib, "d3d12.lib")
#else
#include <d3d11.h>
#endif
#include <d2d1.h>
#include <d2d1_1.h>
#include <d2d1_1helper.h>
#include <d2d1effects.h>
#include <dwrite.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "runtimeobject.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dcomp.lib")
#pragma comment(lib, "dwrite.lib")

#ifndef OMEGAWTK_COMPOSITION_DX_DXBDCOMPOSITIONDEVICE_H
#define OMEGAWTK_COMPOSITION_DX_DXBDCOMPOSITIONDEVICE_H



namespace OmegaWTK::Composition {
     class None {};
    class DXBDCompositionViewRenderTarget;
    class DXBDCompositonImageRenderTarget;
    
    class DXBDCompositionDevice : public BDCompositionDevice {
        #ifdef DIRECT3D_12
        Core::UniqueComPtr<ID3D12Device> direct3d_device;
        Core::UniqueComPtr<ID3D11Device> direct3d_device_11_priv;
        Core::UniqueComPtr<ID3D12CommandQueue> direct3d_command_queue;
        #else
        Core::UniqueComPtr<ID3D11Device> direct3d_device;
        #endif
        Core::UniqueComPtr<ID2D1Factory1> direct2d_factory;
        Core::UniqueComPtr<ID2D1Device> direct2d_device;
        #ifdef DIRECT3D_12
        Core::UniqueComPtr<IDXGIFactory4> dxgi_factory;
        #else
        Core::UniqueComPtr<IDXGIFactory2> dxgi_factory;
        #endif
        Core::UniqueComPtr<IDXGIDevice1> dxgi_device;
        Core::UniqueComPtr<IDXGIAdapter> dxgi_adapter;
        Core::UniqueComPtr<IDCompositionDevice> dcomp_device_1;
        Core::UniqueComPtr<IDCompositionDevice2> dcomp_device_2;
        friend class DXBDCompositionViewRenderTarget;
        friend class DXBDCompositionImageRenderTarget;
        friend class DCVisualTree;
        public:
        DXBDCompositionDevice();
        ~DXBDCompositionDevice();
        static Core::SharedPtr<BDCompositionDevice> Create();
        // Core::SharedPtr<BDCompositionViewRenderTarget> makeViewRenderTarget(Layer *layer);
        Core::SharedPtr<BDCompositionImageRenderTarget> makeImageRenderTarget(Core::Rect &size);
        Core::SharedPtr<BDCompositionImageRenderTarget> makeImageRenderTarget(Core::SharedPtr<BDCompositionImage> &img);
        Core::SharedPtr<BDCompositionFontFactory> createFontFactory();
        Core::SharedPtr<BDCompositionVisualTree> createVisualTree();
        void destroyTarget(Layer *layer, Core::SharedPtr<BDCompositionRenderTarget> &target);
        void renderVisualTreeToView(Core::SharedPtr<BDCompositionVisualTree> &visualTree, ViewRenderTarget *view,bool updatePass);
    };
};

#endif