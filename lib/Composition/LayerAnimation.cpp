#include "omegaWTK/Composition/LayerAnimation.h"

namespace OmegaWTK::Composition {
    void LayerAnimationController::setAnim(SharedHandle<LayerAnimationTy> &anim){
        this->anim = anim;
    };

    void LayerAnimationController::playForward(){

    };
    void LayerAnimationController::playReverse(){

    };
    void LayerAnimationController::pause(){

    };

    void LayerAnimationGroupController::addAnimation(SharedHandle<LayerAnimationTy> &anim){
        anims.push_back(anim);
    };

    void LayerAnimationGroupController::removeAnimation(SharedHandle<LayerAnimationTy> &anim){
        auto anim_it = anims.begin();
        while(anim_it != anims.end()){
            auto _anim = *anim_it;
            if(_anim == anim){
                anims.erase(anim_it);
                return;
                break;
            };
            ++anim_it;
        };
    };

    void LayerAnimationGroupController::playForward(){

    };
    void LayerAnimationGroupController::playReverse(){
        
    };

    void LayerAnimationGroupController::pause(){
        
    };
};
