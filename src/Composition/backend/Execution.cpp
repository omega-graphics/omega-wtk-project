#include "../Compositor.h"

namespace OmegaWTK::Composition {



std::future<RenderCommandStatus> Compositor::executeCurrentRenderCommand(){

    

    auto found = renderTargetStore.store.find(currentCommand->renderTarget);
    if(found == renderTargetStore.store.end()){

        

    };

    OmegaCommon::ArrayRef<VisualCommand> commands {currentCommand->_visuals,currentCommand->_visuals + currentCommand->visual_count};
    
    std::promise<RenderCommandStatus> status_promise;
    
    for(auto & com : commands){
        

    };

    auto commandStatus = status_promise.get_future();

    return commandStatus;
};


};