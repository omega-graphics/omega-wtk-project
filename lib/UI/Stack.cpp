#include "omegaWTK/UI/Stack.h"

namespace OmegaWTK::UI {
    template<class _Stack_Ty>
    class StackItemObserver : public WidgetObserver {
        _Stack_Ty *parent;
        Widget *target;
    public:
        StackItemObserver(_Stack_Ty *parent,Widget *target = nullptr):parent(parent),target(target){

        };
        void setTarget(Widget *t){
            target = t;
        };
        Widget *getTarget(){
            return target;
        };
        void onWidgetChangeSize(Core::Rect oldRect, Core::Rect &newRect){

        };
        void onWidgetDidShow(){

        };
        void onWidgetDidHide(){

        };
    };

    typedef StackItemObserver<HStack> HStackItemObserver;
    typedef StackItemObserver<VStack> VStackItemObserver;

    void HStack::addChildWidget(SharedHandle<Widget> widget){
        Widget *w_l;
        if(childWidgets.empty())
            w_l = nullptr;
        else 
            w_l = childWidgets.back().get();
        auto left = std::make_shared<HStackItemObserver>(this,w_l);
        auto right = std::make_shared<HStackItemObserver>(this);
        ObserverPair pair;

        pair.left = left;
        pair.right = right;

        observers.insert(std::make_pair(widget.get(),std::move(pair)));
       if(childWidgets.size() >= 1) {
           auto prev = childWidgets.back().get();
           observers[prev].right->setTarget(widget.get());
       }
       childWidgets.push_back(widget);
    };

    void HStack::removeChildAtIndex(unsigned int idx){
        auto w = childWidgets[idx];
        auto obs_w = observers[w.get()];
        if(idx == 0){
            auto & obs_r_w = observers[obs_w.right->getTarget()];
            obs_r_w.left->setTarget(nullptr);
        }
        else if(idx == (childWidgets.size()-1)) {
            auto & obs_l_w = observers[obs_w.left->getTarget()];
            obs_l_w.right->setTarget(nullptr);
        }
        else {
            auto & obs_l_w = observers[obs_w.left->getTarget()];
            auto & obs_r_w = observers[obs_w.right->getTarget()];
            obs_l_w.right->setTarget(obs_w.right->getTarget());
            obs_r_w.left->setTarget(obs_w.left->getTarget());
        };
        observers.erase(w.get());
        childWidgets.erase(childWidgets.begin() + idx);
        
    };

    void VStack::addChildWidget(SharedHandle<Widget> widget){
        Widget *w_a;
        if(childWidgets.empty())
            w_a = nullptr;
        else 
            w_a = childWidgets.back().get();
        auto above = std::make_shared<VStackItemObserver>(this,w_a);
        auto below = std::make_shared<VStackItemObserver>(this);
        ObserverPair pair;
        pair.above = above;
        pair.below = below;
        observers.insert(std::make_pair(widget.get(),std::move(pair)));
        if(childWidgets.size() >= 1) {
           auto prev = childWidgets.back().get();
           observers[prev].below->setTarget(widget.get());
        }
         childWidgets.push_back(widget);
    };

    void VStack::removeChildAtIndex(unsigned int idx){
        auto w = childWidgets[idx];
        auto obs_w = observers[w.get()];
        if(idx == 0){
            auto & obs_b_w = observers[obs_w.below->getTarget()];
            obs_b_w.above->setTarget(nullptr);
        }
        else if(idx == (childWidgets.size()-1)) {
            auto & obs_a_w = observers[obs_w.above->getTarget()];
            obs_a_w.below->setTarget(nullptr);
        }
        else {
            auto & obs_a_w = observers[obs_w.above->getTarget()];
            auto & obs_b_w = observers[obs_w.below->getTarget()];
            obs_a_w.below->setTarget(obs_w.below->getTarget());
            obs_b_w.above->setTarget(obs_w.above->getTarget());
        };
        observers.erase(w.get());
        childWidgets.erase(childWidgets.begin() + idx);
    };





};