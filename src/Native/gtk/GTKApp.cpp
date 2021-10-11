
#include "omegaWTK/Native/NativeApp.h"

#include <gtk/gtk.h>

namespace OmegaWTK::Native::GTK {
    class GTKApp : public NativeApp {
        GtkApplication *native;
    public:
        void terminate(){
            g_object_unref(native);
        };
        explicit GTKApp(void *data){
            native = gtk_application_new("",G_APPLICATION_FLAGS_NONE);
//            g_signal_connect(native,"activate",NULL,NULL);
        };
        int runEventLoop() override {
            return g_application_run(G_APPLICATION(native),0,NULL);
        }
    };

}

namespace OmegaWTK::Native {
    NAP make_native_app(void *data){
        return (NAP)new GTK::GTKApp(data);
    };
}