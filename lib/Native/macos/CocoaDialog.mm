#import "CocoaDialog.h"

#import <Cocoa/Cocoa.h>

namespace OmegaWTK::Native::Cocoa {
//    class CocoaFSDialog : public NativeFSDialog {
//        
//    };
//
//    class CocoaNoteDialog : public NativeNoteDialog {
//        
//    };

    SharedHandle<NativeFSDialog> make_cocoa_fs_dialog(){
//        return std::make_shared<CocoaFSDialog>();
    };

    SharedHandle<NativeNoteDialog> make_cocoa_note_dialog(){
//        return std::make_shared<CocoaNoteDialog>();
    };
};
