#include "omegaWTK/Core/Core.h"
#include "Widget.h"
#include "AppWindow.h"
#include <type_traits>

#ifndef OMEGAWTK_UI_WIDGETTREEHOST_H
#define OMEGAWTK_UI_WIDGETTREEHOST_H

namespace OmegaWTK {
    /**
     @brief Owns a widget tree. (Owns the Widget tree's Compositor, and the Compositor's Scheduler)
     @paragraph An instance of this class gets attached to an AppWindow directly and the root widget
     the host is assigned to the AppWindow. 
     NOTE: An AppWindow also has a WindowLayer that can be drawn on by a Compositor, so in order
     to guarantee a fast runtime it uses the **first WidgetTreeHost's Compositor**.
    */
    class OMEGAWTK_EXPORT WidgetTreeHost {
        /** The Widget Tree's Compositor
         NOTE: The instance of this class that was first attached to an 
         AppWindow will be used for managing composition of the WindowLayer.
         */
        UniqueHandle<Composition::Compositor> compositor;
        /// The Root Widget
        SharedHandle<Widget> root;

        bool attachedToWindow;

        WidgetTreeHost();
    public:

        static WidgetTreeHost * Create();

        /**
         @brief Creates a Widget that will be added as a child of this WidgetTreeHost.
         @param[in] args The Widget's Constructor arguments
        */
        template<class T,class ...Args,std::enable_if_t<std::is_base_of_v<Widget,T>,int> = 0>
        SharedHandle<T> makeWidget(const Core::Rect & rect,SharedHandle<Widget> parent = nullptr,Args && ...args){
            return make<T>(rect,this,parent,args...);
        };

        /**
         @brief Set the root of the Widget tree.
         @param[in] widget The root of the Widget Tree
        */
        void setRoot(SharedHandle<Widget> widget);

        /**
         @brief Attach this host to an AppWindow
         @param[in] window The AppWindow to attach to. 
         @paragraph
         If this instance of this class is the first to be attached to the AppWindow specified,
         its Compositor will be used to manage composition for its WindowLayer.
        */
        void attachToWindow(SharedHandle<AppWindow> & window);

        ~WidgetTreeHost() = default;
    };
};

#endif