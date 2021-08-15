#include "omegaWTK/UI/Widget.h"

#ifndef OMEGAWTK_UI_STACK_H
#define OMEGAWTK_UI_STACK_H

namespace OmegaWTK::UI {
    template<class _Ty>
    class StackItemObserver;

    class HStack;
    class VStack;

    typedef StackItemObserver<HStack> HStackItemObserver;
    typedef StackItemObserver<VStack> VStackItemObserver;


    class OMEGAWTK_EXPORT VStack : public Widget {
        friend class StackItemObserver<VStack>;
        struct ObserverPair {
            SharedHandle<VStackItemObserver> above;
            SharedHandle<VStackItemObserver> below;
        };
        OmegaCommon::Map<Widget *,ObserverPair> observers;
    };

    class OMEGAWTK_EXPORT HStack : public Widget {
        friend class StackItemObserver<HStack>;
        struct ObserverPair {
            SharedHandle<HStackItemObserver> left;
            SharedHandle<HStackItemObserver> right;
        };
        OmegaCommon::Map<Widget *,ObserverPair> observers;
    };
};

#endif