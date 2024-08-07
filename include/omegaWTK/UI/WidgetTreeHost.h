#include "omegaWTK/Core/Core.h"
#include <type_traits>

#ifndef OMEGAWTK_UI_WIDGETTREEHOST_H
#define OMEGAWTK_UI_WIDGETTREEHOST_H

namespace OmegaWTK {

    namespace Composition {
        class Compositor;
    }


    class AppWindow;
    OMEGACOMMON_SHARED_CLASS(AppWindow);
    class Widget;
    OMEGACOMMON_SHARED_CLASS(Widget);
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
        Composition::Compositor * compositor;
        /// The Root Widget
        WidgetPtr root;

        bool attachedToWindow;

        WidgetTreeHost();

        friend class AppWindowManager;
        friend class AppWindow;
        friend class Widget;

        void initWidgetRecurse(Widget *parent);
        void initWidgetTree();
        Composition::Compositor *compPtr(){return compositor;};
    public:
        OMEGACOMMON_CLASS("OmegaWTK.UI.WidgetTreeHost")


        static SharedHandle<WidgetTreeHost> Create();

        /**
         @brief Set the root of the Widget tree.
         @param[in] widget The root of the Widget Tree
        */
        void setRoot(WidgetPtr widget);

        /**
         @brief Attach this host to an AppWindow
         @param[in] window The AppWindow to attach to. 
         @paragraph
         If this instance of this class is the first to be attached to the AppWindow specified,
         its Compositor will be used to manage composition for its WindowLayer.
        */
        void attachToWindow(AppWindow * window);

        ~WidgetTreeHost();
    };
};

#endif