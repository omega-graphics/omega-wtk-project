#include "../BDCompositionRenderTarget.h"
#include "DXBDCompositionDevice.h"

#include "NativePrivate/win/HWNDItem.h"

#ifndef OMEGAWTK_COMPOSITION_DX_DXBDCOMPOSITIONRENDERTARGET_H
#define OMEGAWTK_COMPOSITION_DX_DXBDCOMPOSITIONRENDERTARGET_H

namespace OmegaWTK::Composition {
    class DXBDCompositionRenderTarget : public BDCompositionRenderTarget {
        DXBDCompositionDevice *device;
        Core::UniqueComPtr<IDXGISwapChain1> dxgi_swap_chain;
        Core::UniqueComPtr<ID2D1Bitmap1> direct2d_bitmap;
        Core::UniqueComPtr<IDXGISurface> dxgi_surface;
        Core::UniqueComPtr<ID2D1DeviceContext> direct2d_device_context;
        Core::UniqueComPtr<IDCompositionTarget> dcomp_target;
        bool recreateDeviceContext;
        bool recreateSwapChain;
        Native::Win::HWNDItem *hwndItem;
        public:
        DXBDCompositionRenderTarget(DXBDCompositionDevice * device,Native::Win::HWNDItem *hwndItem);
        bool needsSwapChain(){
            return recreateSwapChain;
        };
        bool needsDeviceContext(){
            return recreateDeviceContext;
        };
        void redoSwapChain();
        void redoDeviceContext();
        ~DXBDCompositionRenderTarget();
        static Core::SharedPtr<BDCompositionRenderTarget> Create(DXBDCompositionDevice *device,Native::Win::HWNDItem *item);
        void clear(Color &clearColor);
        void frameRect(Core::Rect &rect, Core::SharedPtr<Brush> &brush, unsigned int width);
        void fillRect(Core::Rect &rect, Core::SharedPtr<Brush> &brush);
        void commit();
    };
}

#endif