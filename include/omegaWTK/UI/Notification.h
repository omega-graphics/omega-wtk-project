#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Native/NativeNote.h"

#ifndef OMEGAWTK_UI_NOTIFICATION_H
#define OMEGAWTK_UI_NOTIFICATION_H

namespace OmegaWTK {

    struct NotificationDesc {
        OmegaCommon::String title;
        OmegaCommon::String body;
    };

    class OMEGAWTK_EXPORT NotificationCenter {
        Native::NNCP nativeNoteCenter;
    public:
        OMEGACOMMON_CLASS("OmegaWTK.NotifcationCenter")
    
        void send(NotificationDesc desc);
    };
};

#endif