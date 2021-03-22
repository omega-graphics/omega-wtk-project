#include "omegaWTK/Native/NativeDialog.h"

#ifndef OMEGAWTK_NATIVE_COCOA_COCOADIALOG_H
#define OMEGAWTK_NATIVE_COCOA_COCOADIALOG_H

namespace OmegaWTK::Native::Cocoa {
SharedHandle<NativeFSDialog> make_cocoa_fs_dialog();
SharedHandle<NativeNoteDialog> make_cocoa_note_dialog();
};

#endif
