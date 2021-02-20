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
        bool newTarget;
        Native::Win::HWNDItem *hwndItem;
        friend class DXBDCompositionImage;
        public:
        DXBDCompositionRenderTarget(DXBDCompositionDevice * device,Native::Win::HWNDItem *hwndItem);
        bool needsSwapChain(){
            return recreateSwapChain;
        };
        bool needsDeviceContext(){
            return recreateDeviceContext;
        };
        bool targetIsFresh(){
            return newTarget;
        };
        void redoSwapChain();
        void redoDeviceContext();
        ~DXBDCompositionRenderTarget();
        static Core::SharedPtr<BDCompositionRenderTarget> Create(DXBDCompositionDevice *device,Native::Win::HWNDItem *item);
        void clear(Color &clearColor);
        void frameRect(Core::Rect &rect, Core::SharedPtr<Brush> &brush, unsigned int width);
        void fillRect(Core::Rect &rect, Core::SharedPtr<Brush> &brush);
        void frameRoundedRect(Core::RoundedRect &rect, Core::SharedPtr<Brush> &brush, unsigned int width);
        void fillRoundedRect(Core::RoundedRect &rect, Core::SharedPtr<Brush> &brush);
        Core::SharedPtr<BDCompositionImage> createImageFromBitmapImage(Core::SharedPtr<Core::BitmapImage> &img,Core::Rect &newSize,unsigned v_id);
        void drawImage(Core::SharedPtr<BDCompositionImage> &img,Core::Position pos);
        void drawText(Core::SharedPtr<BDCompositionFont> &font, Core::String &string, Core::Rect &textRect,Core::SharedPtr<Brush> & brush);
        void commit();
    };
}

#endif