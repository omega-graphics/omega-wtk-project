#include "omegaWTK/Native/NativeNote.h"

#import <UserNotifications/UserNotifications.h>

namespace OmegaWTK::Native::Cocoa {

    class CocoaNoteCenter : public NativeNoteCenter {
        __strong UNUserNotificationCenter *notificationCenter;
    public:
        CocoaNoteCenter():notificationCenter([UNUserNotificationCenter currentNotificationCenter]){

        }
        ~CocoaNoteCenter(){
            [notificationCenter release];
        }
    };

    class CocoaNote {
        __strong UNNotification *notification;
    public:

    };

    NNCP make_native_note_center(){
        return (NNCP)new CocoaNoteCenter();
    }
}