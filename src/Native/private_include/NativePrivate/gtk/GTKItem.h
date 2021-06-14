#include "omegaWTK/Native/NativeItem.h"

#include <gtk/gtk.h>
#include <gdk/gdkx.h>

#ifndef OMEGAWTK_NATIVE_GTK_GTKITEM_H
#define OMEGAWTK_NATIVE_GTK_GTKITEM_H

namespace OmegaWTK::Native::GTK {
    class GTKItem : public NativeItem {
        GdkWindow *window;
        GdkWindowClass * wnd_class;
        public:
        Display * getDisplay();
        Window getX11Window();
    };
};

#endif
