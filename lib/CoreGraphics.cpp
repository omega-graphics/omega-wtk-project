#include "omegaWTK/CoreGraphics.h"

namespace OmegaWTK {
    HRESULT CoreGraphics::Init(HWND hwnd){
        tg_hwnd = hwnd;

        HRESULT hr = S_OK;

        D3D_DRIVER_TYPE driverTypes[] =
        {
            D3D_DRIVER_TYPE_HARDWARE,
            D3D_DRIVER_TYPE_WARP,
        };

        D3D_FEATURE_LEVEL featureLevelSupported;

        Microsoft::WRL::ComPtr<ID3D11Device> _d3d11dev;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> _d3d11dev_cntxt;

         for (int i = 0; i < sizeof(driverTypes) / sizeof(driverTypes[0]); ++i)
        {
                hr = D3D11CreateDevice(
                    nullptr,
                    driverTypes[i],
                    NULL,
                    D3D11_CREATE_DEVICE_BGRA_SUPPORT,
                    NULL,
                    0,
                    D3D11_SDK_VERSION,
                    &_d3d11dev,
                    &featureLevelSupported,
                    &_d3d11dev_cntxt);
                if(SUCCEEDED(hr)){
                    d3d11_dev = _d3d11dev.Detach();
                    d3d11_dev_cntxt = _d3d11dev_cntxt.Detach();

                    break;
                };
        }   

        Microsoft::WRL::ComPtr<IDXGIDevice> dxgi_dev;
        hr = d3d11_dev->QueryInterface(dxgi_dev.GetAddressOf());
        if(SUCCEEDED(hr)){
            hr = DCompositionCreateDevice(dxgi_dev.Get(),IID_PPV_ARGS(comp_device.GetAddressOf()));
        };

        if(SUCCEEDED(hr)){
            hr = (comp_device.Get() == nullptr) || (tg_hwnd == NULL) ? E_UNEXPECTED : S_OK;

            if(SUCCEEDED(hr)){
                hr = comp_device->CreateTargetForHwnd(tg_hwnd,FALSE,comp_target.GetAddressOf());
            };
        };

        return hr;
    };
}