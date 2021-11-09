#include "NativePrivate/android/AndroidItem.h"

#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <jni.h>

namespace OmegaWTK::Mobile::Native {

class AndroidItem : public NativeItem {
    ANativeWindow *window;
    jobject nativeItem;
    jclass cl;
    JNIEnv *env;
public:
    explicit AndroidItem(){
        cl = env->FindClass("AndroidItemJava");
        nativeItem = env->CallStaticObjectMethod(cl,env->GetStaticMethodID(cl,"Create",""));
        window = ANativeWindow_fromSurface(env,env->CallObjectMethod(nativeItem,env->GetMethodID(cl,"getSurface","")));
    }

    void enable() override {
        env->CallVoidMethod(nativeItem,env->GetMethodID(cl,"enable",""));
    }
    void disable() override {
        env->CallVoidMethod(nativeItem,env->GetMethodID(cl,"disable",""));
    }
    void addChildNativeItem(SharedHandle<NativeItem> &child) override {
        auto item = std::dynamic_pointer_cast<AndroidItem>(child);
        env->CallVoidMethod(nativeItem,env->GetMethodID(cl,"addChildNativeItem",""),item->nativeItem);
    }
    void removeChildNativeItem(SharedHandle<NativeItem> &child) override {
        auto item = std::dynamic_pointer_cast<AndroidItem>(child);
        env->CallVoidMethod(nativeItem,env->GetMethodID(cl,"removeChildNativeItem",""),item->nativeItem);
    }
};

}