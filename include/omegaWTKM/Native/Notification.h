#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTKM_NOTIFICATION_H
#define OMEGAWTKM_NOTIFICATION_H

namespace OmegaWTK::Mobile::Native {

    INTERFACE OMEGAWTK_EXPORT NotificationCenter {
        static SharedHandle<NotificationCenter> Create();
        INTERFACE_METHOD void sendNotification(OmegaCommon::StrRef title,OmegaCommon::StrRef description) ABSTRACT;
    };

}

#endif