#include "omegaWTK/pch.h"

#ifndef OMEGAWTK_COREGRAPHICS_H
#define OMEGAWTK_COREGRAPHICS_H

namespace OmegaWTK {
    class CoreGraphics {
        Microsoft::WRL::ComPtr<IDCompositionTarget> comp_target;
        Microsoft::WRL::ComPtr<IDCompositionDevice> comp_device;
        Microsoft::WRL::ComPtr<ID3D11Device> d3d11_dev;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3d11_dev_cntxt;
        HWND tg_hwnd;
        public:
        HRESULT Init(HWND hwnd);
        CoreGraphics();
        ~CoreGraphics();
    };
};

#endif