#include "omegaWTK/UI/UIView.h"
// #include "omegaWTK/Composition/Canvas.h"

namespace OmegaWTK {

    void UIViewLayout::text(Composition::CanvasElementTag tag, OmegaCommon::UString content){
        auto taggedElementIt = std::find_if(_content.begin(),_content.end(),[&](Element & ele){
            return ele.tag == tag;
        });
        if(taggedElementIt != _content.end()){
            if(taggedElementIt->type == 1){
                std::cout << "CANNOT CHANGE TYPE OF ELEMENT." << taggedElementIt->tag;
            }
            taggedElementIt->str = content;
        }
        else {
            _content.push_back({0,tag,content,{}});
        }
        
    }

    void UIViewLayout::shape(Composition::CanvasElementTag tag,Shape & shape){
        auto taggedElementIt = std::find_if(_content.begin(),_content.end(),[&](Element & ele){
            return ele.tag == tag;
        });
        if(taggedElementIt != _content.end()){
            if(taggedElementIt->type == 0){
                std::cout << "CANNOT CHANGE TYPE OF ELEMENT." << taggedElementIt->tag;
            }
            taggedElementIt->shape = shape;
        }
        else {
            _content.push_back({1,tag,{},shape});
        }
    }

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