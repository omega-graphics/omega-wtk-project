#include "omegaWTK/Native/NativeDialog.h"

#ifdef TARGET_WIN32
#include "win/WinDialog.h"
#endif

#ifdef TARGET_MACOS
#include "macos/CocoaDialog.h"
#endif

#ifdef TARGET_GTK
#include "gtk/GTKDialog.h"
#endif

namespace OmegaWTK::Native {

    NativeDialog::NativeDialog(NWH nativeWindow):parentWindow(nativeWindow){

    };

    NativeFSDialog::NativeFSDialog(NWH nativeWindow):NativeDialog(nativeWindow){};

    NativeNoteDialog::NativeNoteDialog(NWH nativeWindow):NativeDialog(nativeWindow){};
    

     SharedHandle<NativeNoteDialog> NativeNoteDialog::Create(const Descriptor &desc,NWH nativeWindow){
        #ifdef TARGET_MACOS
         return Cocoa::make_cocoa_note_dialog(desc,nativeWindow);
        #endif
         // return Win::WinNoteDialog::Create(desc,nativeWindow);
     };

    SharedHandle<NativeFSDialog> NativeFSDialog::Create(const Descriptor &desc,NWH nativeWindow){
        #ifdef TARGET_WIN32
        return Win::WinFSDialog::Create(desc,nativeWindow);
        #endif
        
        #ifdef TARGET_MACOS
        return Cocoa::make_cocoa_fs_dialog(desc,nativeWindow);
        #endif
    };

};
