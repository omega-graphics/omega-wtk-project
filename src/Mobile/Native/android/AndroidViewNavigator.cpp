#include "omegaWTKM/Native/NativeViewNavigator.h"

#include "NativePrivate/android/AndroidItem.h"

#include <jni.h>

namespace OmegaWTK::Mobile::Native {

    class AndroidViewNavigator : public NativeViewNavigator {
        jobject native;
        jclass clazz;
        JNIEnv *env;
    public:
      void addView(NativeItemPtr view) override {
          auto item = std::dynamic_pointer_cast<AndroidItem>(view);
          env->CallVoidMethod(native,env->GetMethodID(clazz,"addView","(Lorg/omegagraphics/OmegaWTKAndroid/AndroidItemJava;)V"),item->nativeItem);
      }
      void removeView(NativeItemPtr view) override {
          auto item = std::dynamic_pointer_cast<AndroidItem>(view);
          env->CallVoidMethod(native,env->GetMethodID(clazz,"removeView","(Lorg/omegagraphics/OmegaWTKAndroid/AndroidItemJava;)V"),item->nativeItem);
      }
    };

}