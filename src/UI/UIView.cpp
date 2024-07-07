#include "omegaWTK/UI/UIView.h"

namespace OmegaWTK {

    StyleSheetPtr StyleSheet::Create(){
        return make<StyleSheet>();
    }

    StyleSheetPtr StyleSheet::border(UIViewTag tag, bool use){

    }

    UIView::UIView(const Core::Rect & rect,Composition::LayerTree *layerTree,ViewPtr parent,UIViewTag tag):CanvasView(rect,layerTree,parent),tag(tag){

    }

    void UIView::useLayout(UIViewLayout user){

    }

    void UIView::useStyleSheet(StyleSheetPtr user){

    }

    void UIView::update(){
        
    }
};