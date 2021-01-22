#include "omegaWTK/Composition/Compositor.h"
#include "NativePrivate/win/HWNDItem.h"

#include <wrl.h>
#include <dwrite.h>
#include <d2d1_1.h>
#include <dcomp.h>
#include <dcompanimation.h>

#ifndef OMEGAWTK_COMPOSITION_PRIVATE_INCLUDE_COMPOSITOR_WIN32_H
#define OMEGAWTK_COMPOSITION_PRIVATE_INCLUDE_COMPOSITOR_WIN32_H

namespace OmegaWTK::Composition {

    class WinCanvas : public Canvas {
        Native::Win::HWNDItem * native_item;
        Microsoft::WRL::ComPtr<ID2D1Layer> d2_layer;
        Microsoft::WRL::ComPtr<IDXGISurface> dxgi_surface;
        Microsoft::WRL::ComPtr<IDCompositionSurface> dcomp_surface;
        Native::NativeItemPtr getNativeItem();
    };
    class WinCompositor : public Compositor {
        Microsoft::WRL::ComPtr<ID2D1Factory1> d2_factory;
        Microsoft::WRL::ComPtr<ID2D1Device> d2_device;
        Microsoft::WRL::ComPtr<IDCompositionDevice> comp_dev;
        Microsoft::WRL::ComPtr<IDXGIFactory> dxgi_factory;
        Microsoft::WRL::ComPtr<IDWriteFactory> d_write_factory;

        WinCanvas * current_canvas;
        void setCanvas(Canvas *canvas);
    public:
        WinCompositor(){
            HRESULT hr;
            hr = CreateDXGIFactory1(IID_PPV_ARGS(dxgi_factory.GetAddressOf()));
            if(SUCCEEDED(hr)){
                hr = DCompositionCreateDevice(nullptr,IID_PPV_ARGS(comp_dev.GetAddressOf()));
                hr = d2_factory->CreateDevice(nullptr,d2_device.GetAddressOf());
                hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,__uuidof(IDWriteFactory),(IUnknown **)d_write_factory.GetAddressOf());
            };
        };
        ~WinCompositor();
    };
};

#endif
