#include "omegaWTKM/Native/NativeItem.h"

#include <jni.h>
#include <android/native_window.h>

#ifndef OMEGAWTK_MOBILE_NATIVE_NATIVEPRIVATE_ANDROID_ITEM_H
#define OMEGAWTK_MOBILE_NATIVE_NATIVEPRIVATE_ANDROID_ITEM_H


namespace OmegaWTK::Mobile::Native {
    class AndroidItem : public NativeItem {
    public:
        ANativeWindow *window;
        jobject nativeItem;
        jclass cl;
        JNIEnv *env;
        explicit AndroidItem();
        void enable() override;
        void disable() override;
        void addChildNativeItem(SharedHandle<NativeItem> &child) override;
        void removeChildNativeItem(SharedHandle<NativeItem> &child) override;
    };
}

#endif