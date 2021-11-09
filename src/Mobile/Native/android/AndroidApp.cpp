#include "omegaWTKM/Native/NativeMobileApp.h"
#include "omegaWTK/Main.h"

#include <android_native_app_glue.h>

namespace OmegaWTK::Mobile::Native {
    class AndroidApp : public NativeApp {
    public:
        bool finish = false;
        android_app *app;
        jobject javaObj;
        jclass javaClass;
        jobject context;

        explicit AndroidApp(void *data){
            app = (android_app *)app;
            javaClass = app->activity->env->FindClass("AndroidAppJava");
            javaObj = app->activity->env->NewObject(javaClass,app->activity->env->GetMethodID(javaClass,"AndroidAppJava","LAndroidAppJava"),app->activity->clazz);
            context = app->activity->env->CallObjectMethod(javaObj,app->activity->env->GetMethodID(javaClass,"getContext","LContext"));
        }

        int run() override {
            int8_t cmd;
            while(true){
                cmd = android_app_read_cmd(app);
                android_app_pre_exec_cmd(app,cmd);
                if(cmd == APP_CMD_DESTROY){
                    break;
                }
                else {
                    android_app_post_exec_cmd(app,cmd);
                }
            }
            android_app_post_exec_cmd(app,cmd);
            terminate();
            return 0;
        }
        void terminate() override {
            app->activity->env->DeleteGlobalRef(javaObj);
        }
    };
}