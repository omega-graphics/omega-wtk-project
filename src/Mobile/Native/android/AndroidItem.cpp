#include "NativePrivate/android/AndroidItem.h"

#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <jni.h>

namespace OmegaWTK::Mobile::Native {


   AndroidItem::AndroidItem(){
        cl = env->FindClass("AndroidItemJava");
        nativeItem = env->CallStaticObjectMethod(cl,env->GetStaticMethodID(cl,"Create","()Lorg/omegagraphics/OmegaWTKAndroid/AndroidItemJava;"));
        window = ANativeWindow_fromSurface(env,env->CallObjectMethod(nativeItem,env->GetMethodID(cl,"getSurface","")));
    }

    void AndroidItem::enable() {
        env->CallVoidMethod(nativeItem,env->GetMethodID(cl,"enable","V"));
    }
    void AndroidItem::disable(){
        env->CallVoidMethod(nativeItem,env->GetMethodID(cl,"disable","V"));
    }
    void AndroidItem::addChildNativeItem(SharedHandle<NativeItem> &child) {
        auto item = std::dynamic_pointer_cast<AndroidItem>(child);
        env->CallVoidMethod(nativeItem,env->GetMethodID(cl,"addChildNativeItem",""),item->nativeItem);
    }
    void AndroidItem::removeChildNativeItem(SharedHandle<NativeItem> &child) {
        auto item = std::dynamic_pointer_cast<AndroidItem>(child);
        env->CallVoidMethod(nativeItem,env->GetMethodID(cl,"removeChildNativeItem",""),item->nativeItem);
    }
};

}