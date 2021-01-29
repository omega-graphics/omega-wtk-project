#include "omegaWTK/Composition/View.h"

namespace OmegaWTK::Composition {
    View::View(const Core::Rect & rect):Layer(rect){};
    bool View::hasDelegate(){
        return delegate != nullptr;
    };
    void View::setDelegate(ViewDelegate *_delegate){
        delegate = _delegate;
        delegate->view = this;
    };
};