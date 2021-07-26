#import "CocoaDialog.h"
#import "NativePrivate/macos/CocoaUtils.h"
#import "CocoaAppWindow.h"

#import <Cocoa/Cocoa.h>

namespace OmegaWTK::Native::Cocoa {
    class CocoaFSDialog : public NativeFSDialog {
        NSOpenPanel *openPanel;
        NSSavePanel *savePanel;
    public:
        void show(){
            NSWindow *parentWindow = ((CocoaAppWindow *)this->parentWindow)->getWindow();
            if(!openPanel){
                /// If is Save Panel
                // __block NSURL *url = nil;
                [savePanel beginSheetModalForWindow:parentWindow completionHandler:^(NSModalResponse response){
                    if(response == NSModalResponseOK){
                        // url = [savePanel URL];
                    };
                }];

                // return [url fileSystemRepresentation];

            }
            else if(!savePanel){
                 /// If is Save Panel
                // __block NSURL *url = nil;
                [openPanel beginSheetModalForWindow:parentWindow completionHandler:^(NSModalResponse response){
                    if(response == NSModalResponseOK){
                        // url = [[openPanel URLs] firstObject];
                    };
                }];

                // return [url fileSystemRepresentation];
            };
        };
        void close(){

        };
        CocoaFSDialog(const Descriptor &desc,NWH nativeWindow):NativeFSDialog(nativeWindow){
           if(desc.type == Read){
               openPanel = [NSOpenPanel openPanel];
               savePanel = nil;
           }
           else if(desc.type == Write) {
               savePanel = [NSSavePanel savePanel];
               openPanel = nil;
           };
        };
    };

    class CocoaNoteDialog : public NativeNoteDialog {
        NSAlert *dialog;
    public:
        void show(){
            NSWindow *parentWindow = ((CocoaAppWindow *)this->parentWindow)->getWindow();
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
            dialog.messageText = Cocoa::common_string_to_ns_string(desc.title);
            dialog.informativeText = Cocoa::common_string_to_ns_string(desc.str);
        };
    };

    SharedHandle<NativeFSDialog> make_cocoa_fs_dialog(const NativeFSDialog::Descriptor &desc,NWH nativeWindow){
//        return std::make_shared<CocoaFSDialog>();
          return std::make_shared<CocoaFSDialog>(desc,nativeWindow);
    };

    SharedHandle<NativeNoteDialog> make_cocoa_note_dialog(const NativeNoteDialog::Descriptor &desc,NWH nativeWindow){
        return std::make_shared<CocoaNoteDialog>(desc,nativeWindow);
    };
};
