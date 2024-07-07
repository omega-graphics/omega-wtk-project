#include "omegaWTK/UI/Notification.h"

namespace OmegaWTK {
    void NotificationCenter::send(NotificationDesc desc){
        Native::NativeNote note;
        note.title = desc.title;
        note.body = desc.body;
        nativeNoteCenter->sendNativeNote(note);
    };
};