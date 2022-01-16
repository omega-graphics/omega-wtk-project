#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTKM_NATIVE_NATIVEITEM_H
#define OMEGAWTKM_NATIVE_NATIVEITEM_H

namespace OmegaWTK::Mobile::Native {


INTERFACE NativeItem;
typedef SharedHandle<NativeItem> NativeItemPtr;

INTERFACE NativeItem {
    Core::Rect rect;
protected:
    explicit NativeItem(const Core::Rect & rect):rect(rect){ };
public:

    INTERFACE_METHOD void enable() ABSTRACT;

    INTERFACE_METHOD void disable() ABSTRACT;

    INTERFACE_METHOD void resize(const Core::Rect & newRect) ABSTRACT;

    INTERFACE_METHOD void addChildNativeItem(SharedHandle<NativeItem> &child) ABSTRACT;

    INTERFACE_METHOD void removeChildNativeItem(SharedHandle<NativeItem> &child) ABSTRACT;

    INTERFACE_METHOD ~NativeItem() FALLTHROUGH;

   static NativeItemPtr Create(const Core::Rect & rect);
};




}

#endif