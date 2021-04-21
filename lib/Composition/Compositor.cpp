#include "omegaWTK/Composition/Compositor.h"
#include "omegaWTK/Composition/Backend.h"
#include "omegaWTK/Composition/Layer.h"
namespace OmegaWTK::Composition {

// RenderCommandExecutionScheduler::RenderCommandExecutionScheduler(){
//     instance = this;
//     run();
// };

// void RenderCommandExecutionScheduler::run(){
//     t = new std::thread([&](){
        
//     });
// };

// RenderCommandExecutionScheduler::~RenderCommandExecutionScheduler(){
//     t->join();
//     delete t;
// };


Compositor::Compositor():backend(make_backend()){
    
};

void Compositor::__drawChildLimbs(LayerTree::Limb *limb,LayerTree *layerTree){
    auto count = layerTree->getParentLimbChildCount(limb);
    // std::string message = std::string("LimbSize:") + std::to_string(count);
    // MessageBoxA(HWND_DESKTOP,message.c_str(),"NOTE",MB_OK);
    auto size = count;
    while(count > 0){
        unsigned idx = size - count;
        auto childLimb = layerTree->getLimbAtIndexFromParent(idx,limb);
        backend->setCurrentJob(childLimb);
        backend->doWork();
        __drawChildLimbs(childLimb,layerTree);
        --count;
    };
};


void Compositor::prepareDraw(LayerTree *layerTree){
    
    /// Draw LayerTree
    auto rootLimb = layerTree->getTreeRoot();
    backend->setCurrentJob(rootLimb);
    // MessageBoxA(HWND_DESKTOP,"Preparing a Draw on Root Limb","NOTE",MB_OK);
    backend->doWork();
    __drawChildLimbs(rootLimb,layerTree);

    allowUpdates = true;
};

void Compositor::__updateChildLimbs(LayerTree::Limb *limb,LayerTree *layerTree){
    auto count = layerTree->getParentLimbChildCount(limb);
    // std::string message = std::string("LimbSize:") + std::to_string(count);
    // MessageBoxA(HWND_DESKTOP,message.c_str(),"NOTE",MB_OK);
    auto size = count;
    while(count > 0){
        unsigned idx = size - count;
        auto childLimb = layerTree->getLimbAtIndexFromParent(idx,limb);
        backend->setCurrentJob(childLimb);
        backend->doUpdate();
        __updateChildLimbs(childLimb,layerTree);
        --count;
    };
};

void Compositor::updateLayerTree(LayerTree *tree){
    /// Draw LayerTree
    auto rootLimb = tree->getTreeRoot();
    backend->setCurrentJob(rootLimb);
    // MessageBoxA(HWND_DESKTOP,"Preparing a Draw on Root Limb","NOTE",MB_OK);
    backend->doUpdate();
    //  MessageBoxA(HWND_DESKTOP,"Updated Limb","NOTE",MB_OK);
    // __updateChildLimbs(rootLimb,tree);
    allowUpdates = true;
};


void Compositor::prepareCleanup(){
    allowUpdates = false;
};

void Compositor::updateRequestedLayerTreeLimb(LayerTree::Limb *limb){
    backend->setCurrentJob(limb);
    // backend->doUpdate();
};

void Compositor::layoutLayerTreeLimb(LayerTree::Limb *limb){
    backend->setCurrentJob(limb);
    backend->redoLayout();
};

Compositor::~Compositor(){
    prepareCleanup();
};


};
