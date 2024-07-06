#include "NativePrivate/macos/CocoaUtils.h"
#include "omegaWTK/Native/NativeNote.h"

#import <UserNotifications/UserNotifications.h>

namespace OmegaWTK::Native::Cocoa {

    class CocoaNoteCenter : public NativeNoteCenter {
        __strong UNUserNotificationCenter *notificationCenter;
    public:
        CocoaNoteCenter():notificationCenter([UNUserNotificationCenter currentNotificationCenter]){

        }
        void sendNativeNote(NativeNote &note) override {
            UNMutableNotificationContent *noteContent = [UNMutableNotificationContent init];
            noteContent.title = common_string_to_ns_string(note.title);
            noteContent.body = common_string_to_ns_string(note.body);
            UNNotificationRequest *request = [UNNotificationRequest requestWithIdentifier:@"" content:noteContent trigger:nil];
            [notificationCenter addNotificationRequest:request withCompletionHandler:nil];
        };
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