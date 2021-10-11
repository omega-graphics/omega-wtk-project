#include "../Compositor.h"
#include "VisualTree.h"
#include <utility>

namespace OmegaWTK::Composition {

// OmegaGTE::NativeRenderTargetDescriptor * makeDescForViewRenderTarget(ViewRenderTarget *renderTarget);
// OmegaGTE::NativeRenderTargetDescriptor * makeDescForCanvasSurface(CanvasSurface *layer);

//void Compositor::layerHasResized(Layer *layer){
//
//};
//
//void Compositor::layerHasDisabled(Layer *layer){
//
//};
//
//void Compositor::layerHasEnabled(Layer *layer){
//
//};

void Compositor::executeCurrentCommand(){

    if(currentCommand->type == CompositorCommand::Render) {
        
        auto comm = (CompositionRenderCommand *)currentCommand.get();


        BackendCompRenderTarget *target;
        bool __buildVisualTree = false;

        auto found = renderTargetStore.store.find(comm->renderTarget);
        if (found == renderTargetStore.store.end()) {
            // __buildVisualTree = true;
            // auto renderTarget = (ViewRenderTarget *)currentCommand->renderTarget;
            // auto desc = makeDescForViewRenderTarget(renderTarget);
            // auto geTarget = gte.graphicsEngine->makeNativeRenderTarget(*desc);
            // BackendCompRenderTarget compTarget {new GERenderTargetContext{geTarget}};
            // compTarget.visualTree = CreateVisualTree();
            // renderTargetStore.store.insert(std::make_pair(renderTarget,compTarget));
            // target = &renderTargetStore.store[renderTarget];
        } else {
            target = &renderTargetStore.store[comm->renderTarget];
        };

        OmegaCommon::ArrayRef<VisualCommand> commands{comm->frame->currentVisuals};

        /// TODO: Process render commands!
    }

    currentCommand->status.set(CommandStatus::Ok);
};


};