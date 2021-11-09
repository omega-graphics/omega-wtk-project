#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTKM_NATIVE_NATIVEITEM_H
#define OMEGAWTKM_NATIVE_NATIVEITEM_H

namespace OmegaWTK::Mobile::Native {

struct NativeEvent {

};

typedef NativeEvent *NativeEventPtr;

class NativeEventProcessor;

class OMEGAWTK_EXPORT NativeEventEmitter {
    NativeEventProcessor *message_reciever;
    friend class NativeEventProcessor;
public:
    NativeEventEmitter();
    bool hasReciever();
    void setReciever(NativeEventProcessor *_responder);
    void emit(NativeEventPtr event);
    ~NativeEventEmitter();
};

/// Listens for native events! (Only some types!)
class OMEGAWTK_EXPORT NativeEventProcessor {
    public:
    virtual void onRecieveEvent(NativeEventPtr event) = 0;
    NativeEventProcessor();
    ~NativeEventProcessor();
};

INTERFACE NativeItem {
public:
    INTERFACE_METHOD void enable() ABSTRACT;
    INTERFACE_METHOD void disable() ABSTRACT;
    INTERFACE_METHOD void addChildNativeItem(SharedHandle<NativeItem> &child) ABSTRACT;
    INTERFACE_METHOD void removeChildNativeItem(SharedHandle<NativeItem> &child) ABSTRACT;
};

typedef SharedHandle<NativeItem> NativeItemPtr;
}

#endif