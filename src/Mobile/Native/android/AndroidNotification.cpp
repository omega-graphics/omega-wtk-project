#include "omegaWTKM/Native/Notification.h"

#include <jni.h>
#include <android_native_app_glue.h>

#include <unicode/ustring.h>

namespace OmegaWTK::Mobile::Native {
    class AndroidNotificationCenter : public NotificationCenter {
        JNIEnv *env;
        jobject object;
        jclass clazz;
    public:
        explicit AndroidNotificationCenter(){
            clazz = env->FindClass("org/omegagraphics/OmegaWTKAndroid/AndroidNotificationCenterJava");
            object = env->NewObject(clazz,env->GetStaticMethodID(clazz,"AndroidNotificationCenterJava","Lorg/omegagraphics/OmegaWTKAndroid/AndroidNotificationCenterJava;"));
        }
        void sendNotification(OmegaCommon::StrRef title, OmegaCommon::StrRef description) override {
            jstring string;
            UChar *uChar;
            int32_t outLen;
            UErrorCode err;
            u_strFromUTF8(uChar,title.size(),&outLen,title.data(),title.size(),&err);
            string = env->NewString((jchar *)uChar,title.size());
            env->CallVoidMethod(object,env->GetMethodID(clazz,"sendNotification","(Ljava/lang/String;,Ljava/lang/String;)V"));
        }
    };

    SharedHandle<NotificationCenter> NotificationCenter::Create() {
        return (SharedHandle<NotificationCenter>)new AndroidNotificationCenter();
    }
}