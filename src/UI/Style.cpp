#include "omegaWTK/UI/Style.h"
#include "omegaWTK/Composition/Brush.h"
#include "omegaWTK/UI/View.h"
#include <memory>

namespace OmegaWTK {

    union Style::Entry {
        struct {
            Composition::Color c;
            bool animate;
            float duration;
        } bkgrdC;

        struct {
            Composition::Color c;
            bool animate;
            float duration;
        } mainC;

         struct {
            bool use;
            Composition::Color c;
             bool animateC;
            float durationC;
            float width;
            bool animateW;
            float durationW;
        } border;
    };

    Style::Style() :data(std::make_unique<OmegaCommon::Map<unsigned,Entry>>()){

    }

    GenericStyledWidget::GenericStyledWidget(const Core::Rect &rect,Widget *parent) : Widget(rect,parent) {

    }

    void GenericStyledWidget::switchStyle(Style &style){

    }

    void GenericStyledWidget::init(){
        performStylingOnView(rootView);
    }

    void GenericStyledWidget::performStylingOnView(SharedHandle<View> &v){
        if(OmegaCommon::is<TextView>(v)){
            auto _v = std::dynamic_pointer_cast<TextView>(v);
            
        }
        else if(OmegaCommon::is<ScrollView>(v)){

        }
        else {

        }
    }
}