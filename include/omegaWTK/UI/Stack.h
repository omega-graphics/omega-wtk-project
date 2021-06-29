#include "Widget.h"

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
        OmegaCommon::Vector<SharedHandle<Widget>> childWidgets;
        friend class StackItemObserver<VStack>;
        struct ObserverPair {
            SharedHandle<VStackItemObserver> above;
            SharedHandle<VStackItemObserver> below;
        };
        OmegaCommon::Map<Widget *,ObserverPair> observers;
    public:
        void addChildWidget(SharedHandle<Widget> widget);
        void removeChildAtIndex(unsigned idx);
    };

    class OMEGAWTK_EXPORT HStack : public Widget {
        OmegaCommon::Vector<SharedHandle<Widget>> childWidgets;
        friend class StackItemObserver<HStack>;
        struct ObserverPair {
            SharedHandle<HStackItemObserver> left;
            SharedHandle<HStackItemObserver> right;
        };
        OmegaCommon::Map<Widget *,ObserverPair> observers;
    public:
        void addChildWidget(SharedHandle<Widget> widget);
        void removeChildAtIndex(unsigned idx);
    };
};

#endif