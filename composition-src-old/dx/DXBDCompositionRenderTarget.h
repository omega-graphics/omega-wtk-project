#include "../BDCompositionRenderTarget.h"
#include "DXBDCompositionDevice.h"

#include "NativePrivate/win/HWNDItem.h"

#ifndef OMEGAWTK_COMPOSITION_DX_DXBDCOMPOSITIONRENDERTARGET_H
#define OMEGAWTK_COMPOSITION_DX_DXBDCOMPOSITIONRENDERTARGET_H

namespace OmegaWTK::Composition {
    template<class _Parent>
    class IDXBDCompositionRenderTarget : public _Parent {
        protected:
        UINT dpi;
        HRESULT hr;
        DXBDCompositionDevice *device;
        Core::UniqueComPtr<ID2D1DeviceContext> direct2d_device_context;
        bool recreateDeviceContext;
        bool newTarget;
        friend class DXBDCompositionImage;
        public:
        bool targetIsFresh(){
            return newTarget;
        };
        bool needsDeviceContext(){
            return recreateDeviceContext;
        };
        IDXBDCompositionRenderTarget(DXBDCompositionDevice *device):device(device){
            dpi = GetDpiFromDpiAwarenessContext(GetThreadDpiAwarenessContext());
        };
        ~IDXBDCompositionRenderTarget();
        void clear(Color &clearColor);
        void setNewDpi(UINT dpi){ this->dpi = dpi;};
        virtual void frameRect(Core::Rect &rect, Core::SharedPtr<Brush> &brush, unsigned int width) = 0;
        virtual void fillRect(Core::Rect &rect, Core::SharedPtr<Brush> &brush) = 0;
        virtual void frameRoundedRect(Core::RoundedRect &rect, Core::SharedPtr<Brush> &brush, unsigned int width) = 0;
        virtual void fillRoundedRect(Core::RoundedRect &rect, Core::SharedPtr<Brush> &brush) = 0;
        virtual Core::SharedPtr<BDCompositionImage> createImageFromBitmapImage(Core::SharedPtr<Media::BitmapImage> &img,Core::Rect &newSize,unsigned v_id) = 0;
        virtual void drawImage(Core::SharedPtr<BDCompositionImage> &img,Core::Position pos) = 0;
         virtual void drawText(Core::SharedPtr<TextRect> & textRect,Core::SharedPtr<Brush> & brush) = 0;
        virtual void commit() = 0;
    };

    class DXBDCompositionViewRenderTarget : public IDXBDCompositionRenderTarget<BDCompositionViewRenderTarget> {
        bool recreateSwapChain;
        Native::Win::HWNDItem *hwndItem;
        #ifdef DIRECT3D_12
        Core::UniqueComPtr<IDXGISwapChain2> dxgi_swap_chain;
        #else
        Core::UniqueComPtr<IDXGISwapChain1> dxgi_swap_chain;
        #endif
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
         Core::SharedPtr<BDCompositionImage> createImageFromBitmapImage(Core::SharedPtr<Media::BitmapImage> &img, Core::Rect &newSize, unsigned int v_id);
         void drawImage(Core::SharedPtr<BDCompositionImage> &img, Core::Position pos);
        void resizeBuffers(Core::Rect & newRect);
        DXBDCompositionViewRenderTarget(DXBDCompositionDevice * device,Native::Win::HWNDItem *hwndItem);
        static Core::SharedPtr<BDCompositionViewRenderTarget> Create(DXBDCompositionDevice *device,Native::Win::HWNDItem *item);
        ~DXBDCompositionViewRenderTarget();
        void drawText(Core::SharedPtr<TextRect> &textRect, Core::SharedPtr<Brush> &brush);
        void commit();
    };
    class DXBDCompositionImageRenderTarget : public IDXBDCompositionRenderTarget<BDCompositionImageRenderTarget> {
        typedef unsigned char Byte;
        Byte *imgData;
        bool recreateSwapChain;
        Core::Rect rect;
        LayerEffect *dropShadow = nullptr;
        Core::Queue<LayerEffect *> effectBuffer;
         bool needsSwapChain(){
            return recreateSwapChain;
        };
        #ifdef DIRECT3D_12
        Core::UniqueComPtr<IDXGISwapChain3> dxgi_swap_chain_3;
        Core::UniqueComPtr<IDXGISwapChain1> dxgi_swap_chain_1;
        #else
        Core::UniqueComPtr<IDXGISwapChain1> dxgi_swap_chain;
        #endif
        Core::UniqueComPtr<IDCompositionSurface> dcomp_surface;
        Core::UniqueComPtr<ID2D1Bitmap1> first_target;
        Core::UniqueComPtr<ID2D1Bitmap1> direct2d_bitmap;
        Core::UniqueComPtr<IDXGISurface> dxgi_surface;
        Core::UniqueComPtr<ID3D12Resource> direct3d_res;
        friend class DCVisualTree;
        friend class DXBDCompositionDeviceContext;
    public:
         void redoDeviceContext();
          void redoSwapChain();
          void redoSwapChainWithNewSize(Core::Rect & newRect);
          void resizeBuffers(Core::Rect & newRect);
         void frameRect(Core::Rect &rect, Core::SharedPtr<Brush> &brush, unsigned int width);
         void frameRoundedRect(Core::RoundedRect &rect, Core::SharedPtr<Brush> &brush, unsigned int width);
         void fillRect(Core::Rect &rect, Core::SharedPtr<Brush> &brush);
         void fillRoundedRect(Core::RoundedRect &rect, Core::SharedPtr<Brush> &brush);
         Core::SharedPtr<BDCompositionImage> createImageFromBitmapImage(Core::SharedPtr<Media::BitmapImage> &img, Core::Rect &newSize, unsigned int v_id);
         void drawImage(Core::SharedPtr<BDCompositionImage> &img, Core::Position pos);
        void drawText(Core::SharedPtr<TextRect> &textRect, Core::SharedPtr<Brush> &brush);
        DXBDCompositionImageRenderTarget(DXBDCompositionDevice * device,Core::Rect & rect);
        static Core::SharedPtr<BDCompositionImageRenderTarget> Create(DXBDCompositionDevice *device,Core::Rect & rect);
        ~DXBDCompositionImageRenderTarget();
        void commit();
        void applyEffect(LayerEffect *effect);
        void commitEffects();
        // Core::SharedPtr<BDCompositionImage> getImg();
    };

    template<class _Ty>
    IDXBDCompositionRenderTarget<_Ty>::~IDXBDCompositionRenderTarget(){
        // Core::SafeRelease(&dcomp_target);
        Core::SafeRelease(&direct2d_device_context);
    };
    template<class _Ty>
    void IDXBDCompositionRenderTarget<_Ty>::clear(Color &clearColor){
        direct2d_device_context->BeginDraw();
        direct2d_device_context->SetTransform(D2D1::IdentityMatrix());
        direct2d_device_context->Clear(color_to_d2d1_color(clearColor));
    };

    typedef IDXBDCompositionRenderTarget<None> DXBDCompositionRenderTarget;
    D2D1::ColorF color_to_d2d1_color(Color & color);
}

#endif