#include "omegaWTKM/Native/NativeItem.h"

#import <UIKit/UIKit.h>

#ifndef OMEGAWTK_MOBILE_NATIVE_NATIVEPRIVATE_ANDROID_ITEM_H
#define OMEGAWTK_MOBILE_NATIVE_NATIVEPRIVATE_ANDROID_ITEM_H

@class OmegaWTKMobileUIViewController;

namespace OmegaWTK::Mobile::Native {
    class IOSItem : public NativeItem {
        OmegaWTKMobileUIViewController *viewC;
    public:
        explicit IOSItem(const Core::Rect & rect);
        void enable() override;
        void disable() override;
        void resize(const Core::Rect &newRect) override;
        void addChildNativeItem(SharedHandle<NativeItem> &child) override;
        void removeChildNativeItem(SharedHandle<NativeItem> &child) override;
    };
}

#endif