#include "../BDCompositionRenderTarget.h"
#include "DXBDCompositionDevice.h"

#include "NativePrivate/win/HWNDItem.h"

#ifndef OMEGAWTK_COMPOSITION_DX_DXBDCOMPOSITIONRENDERTARGET_H
#define OMEGAWTK_COMPOSITION_DX_DXBDCOMPOSITIONRENDERTARGET_H

namespace OmegaWTK::Composition {
    template<class _Parent>
    class IDXBDCompositionRenderTarget : public _Parent {
        protected:
        HRESULT hr;
        DXBDCompositionDevice *device;
        Core::UniqueComPtr<ID2D1DeviceContext> direct2d_device_context;
        bool recreateDeviceContext;
        Core::UniqueComPtr<IDCompositionTarget> dcomp_target;
        bool newTarget;
        friend class DXBDCompositionImage;
        public:
        bool targetIsFresh(){
            return newTarget;
        };
        bool needsDeviceContext(){
            return recreateDeviceContext;
        };
        IDXBDCompositionRenderTarget(DXBDCompositionDevice *device):device(device){};
        ~IDXBDCompositionRenderTarget();
        void clear(Color &clearColor);
        virtual void frameRect(Core::Rect &rect, Core::SharedPtr<Brush> &brush, unsigned int width) = 0;
        virtual void fillRect(Core::Rect &rect, Core::SharedPtr<Brush> &brush) = 0;
        virtual void frameRoundedRect(Core::RoundedRect &rect, Core::SharedPtr<Brush> &brush, unsigned int width) = 0;
        virtual void fillRoundedRect(Core::RoundedRect &rect, Core::SharedPtr<Brush> &brush) = 0;
        virtual Core::SharedPtr<BDCompositionImage> createImageFromBitmapImage(Core::SharedPtr<Core::BitmapImage> &img,Core::Rect &newSize,unsigned v_id) = 0;
        virtual void drawImage(Core::SharedPtr<BDCompositionImage> &img,Core::Position pos) = 0;
        virtual void drawText(Core::SharedPtr<BDCompositionFont> &font, Core::String &string, Core::Rect &textRect,Core::SharedPtr<Brush> & brush)= 0;
        virtual void commit() = 0;
    };

    class DXBDCompositionLayerRenderTarget : public IDXBDCompositionRenderTarget<BDCompositionLayerRenderTarget> {
        bool recreateSwapChain;
          Native::Win::HWNDItem *hwndItem;
        Core::UniqueComPtr<IDXGISwapChain1> dxgi_swap_chain;
        Core::UniqueComPtr<ID2D1Bitmap1> direct2d_bitmap;
        Core::UniqueComPtr<IDXGISurface> dxgi_surface;
    public:
        bool needsSwapChain(){
            return recreateSwapChain;
        };
         void redoSwapChain();
         void redoDeviceContext();
         void frameRect(Core::Rect &rect, Core::SharedPtr<Brush> &brush, unsigned int width);
         void frameRoundedRect(Core::RoundedRect &rect, Core::SharedPtr<Brush> &brush, unsigned int width);
         void fillRect(Core::Rect &rect, Core::SharedPtr<Brush> &brush);
         void fillRoundedRect(Core::RoundedRect &rect, Core::SharedPtr<Brush> &brush);
         Core::SharedPtr<BDCompositionImage> createImageFromBitmapImage(Core::SharedPtr<Core::BitmapImage> &img, Core::Rect &newSize, unsigned int v_id);
         void drawImage(Core::SharedPtr<BDCompositionImage> &img, Core::Position pos);
         void drawText(Core::SharedPtr<BDCompositionFont> &font, Core::String &string, Core::Rect &textRect, Core::SharedPtr<Brush> &brush);
        DXBDCompositionLayerRenderTarget(DXBDCompositionDevice * device,Native::Win::HWNDItem *hwndItem);
        static Core::SharedPtr<BDCompositionLayerRenderTarget> Create(DXBDCompositionDevice *device,Native::Win::HWNDItem *item);
        ~DXBDCompositionLayerRenderTarget();
        void commit();
    };
    class DXBDCompositionImageRenderTarget : public IDXBDCompositionRenderTarget<BDCompositionImageRenderTarget> {
        Core::Rect rect;
        ID2D1Image *img;
    public:
         void redoDeviceContext();
         void frameRect(Core::Rect &rect, Core::SharedPtr<Brush> &brush, unsigned int width);
         void frameRoundedRect(Core::RoundedRect &rect, Core::SharedPtr<Brush> &brush, unsigned int width);
         void fillRect(Core::Rect &rect, Core::SharedPtr<Brush> &brush);
         void fillRoundedRect(Core::RoundedRect &rect, Core::SharedPtr<Brush> &brush);
         Core::SharedPtr<BDCompositionImage> createImageFromBitmapImage(Core::SharedPtr<Core::BitmapImage> &img, Core::Rect &newSize, unsigned int v_id);
         void drawImage(Core::SharedPtr<BDCompositionImage> &img, Core::Position pos);
         void drawText(Core::SharedPtr<BDCompositionFont> &font, Core::String &string, Core::Rect &textRect, Core::SharedPtr<Brush> &brush);
        DXBDCompositionImageRenderTarget(DXBDCompositionDevice * device,Core::Rect & rect,ID2D1Image *img,ID2D1DeviceContext *device_context);
        static Core::SharedPtr<BDCompositionImageRenderTarget> Create(DXBDCompositionDevice *device,Core::Rect & rect,ID2D1Image *img,ID2D1DeviceContext *device_context);
        ~DXBDCompositionImageRenderTarget();
        void commit();
        Core::SharedPtr<BDCompositionImage> getImg();
    };

    template<class _Ty>
    IDXBDCompositionRenderTarget<_Ty>::~IDXBDCompositionRenderTarget(){
        Core::SafeRelease(&dcomp_target);
        Core::SafeRelease(&direct2d_device_context);
    };
    template<class _Ty>
    void IDXBDCompositionRenderTarget<_Ty>::clear(Color &clearColor){
        direct2d_device_context->BeginDraw();
        direct2d_device_context->SetTransform(D2D1::IdentityMatrix());
        direct2d_device_context->Clear(color_to_d2d1_color(clearColor));
    };

    typedef IDXBDCompositionRenderTarget<None> DXBDCompositionRenderTarget;
}

#endif