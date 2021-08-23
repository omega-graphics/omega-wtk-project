#include "../RenderTarget.h"

#include <d2d1_2.h>
#include <d2d1_2helper.h>

namespace OmegaWTK::Composition {

    template<class T>
    using ComPtr = Microsoft::WRL::ComPtr<T>;

    class D2DRenderTargetContext : public BackendRenderTargetContext {
        ComPtr<ID2D1DeviceContext1> renderTarget; 
    public:

        void start(){
            renderTarget->BeginDraw();
        }

        void renderToTarget(VisualCommand::Type type, void *params) override {
            switch (type) {
                case VisualCommand::Rect : {
                    // auto *p = (VisualCommand::RectParams *)params;
                    // renderTarget->FillRectangle(D2D1::RectF()) ;
                    break;
                }
                case VisualCommand::RoundedRect : {

                    break;
                }
            }
        }

        void applyEffectToTarget(CanvasLayerEffect::Type type, void *params) override {

        }

        void commit() override {
            HRESULT hr = renderTarget->EndDraw();

        }
    };

}