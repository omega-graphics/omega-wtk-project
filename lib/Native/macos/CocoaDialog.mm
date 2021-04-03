#import "CocoaDialog.h"
#import "NativePrivate/macos/CocoaUtils.h"
#import "CocoaAppWindow.h"

#import <Cocoa/Cocoa.h>

namespace OmegaWTK::Native::Cocoa {
    class CocoaFSDialog : public NativeFSDialog {
        void show();
        void close();
    };

    class CocoaNoteDialog : public NativeNoteDialog {
        NSAlert *dialog;
    public:
        void show(){
            NSWindow *parentWindow = ((CocoaAppWindow *)this->parentWindow)->window;
            [dialog beginSheetModalForWindow:parentWindow completionHandler:^(NSModalResponse response){
//                this->close();
            }];
        };
        void close(){
//            [dialog panel]
        };
        CocoaNoteDialog(const Descriptor &desc,NWH nativeWindow):NativeNoteDialog(nativeWindow){
            dialog = [[NSAlert alloc] init];
            dialog.showsHelp = NO;
            dialog.alertStyle = NSAlertStyleInformational;
            dialog.messageText = Cocoa::core_string_to_ns_string(desc.title);
            dialog.informativeText = Cocoa::core_string_to_ns_string(desc.str);
        };
    };

    SharedHandle<NativeFSDialog> make_cocoa_fs_dialog(const NativeFSDialog::Descriptor &desc,NWH nativeWindow){
//        return std::make_shared<CocoaFSDialog>();
    };

    SharedHandle<NativeNoteDialog> make_cocoa_note_dialog(const NativeNoteDialog::Descriptor &desc,NWH nativeWindow){
        return std::make_shared<CocoaNoteDialog>(desc,nativeWindow);
    };
};
