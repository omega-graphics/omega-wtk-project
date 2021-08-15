
#include "GTKApp.h"

#include <gtk/gtk.h>

namespace OmegaWTK::Native::GTK {
    class GTKApp : public NativeApp {
        GtkApplication *native;
    public:
        void terminate(){
            g_object_unref(native);
        };
        GTKApp(){
            native = gtk_application_new("",G_APPLICATION_FLAGS_NONE);
        };
    };

    NAP make_gtk_app(){
        return new GTKApp();
    };
}