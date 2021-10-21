#include "omegaWTK/Native/NativeDialog.h"
#import "NativePrivate/macos/CocoaUtils.h"
#import "CocoaAppWindow.h"

#import <Cocoa/Cocoa.h>

namespace OmegaWTK::Native::Cocoa {
    class CocoaFSDialog : public NativeFSDialog {
        NSOpenPanel *openPanel;
        NSSavePanel *savePanel;

        __block OmegaCommon::Promise<OmegaCommon::String> strVal;
    public:
        OmegaCommon::Async<OmegaCommon::String> getResult() override{
            NSWindow *parentWindow = std::dynamic_pointer_cast<CocoaAppWindow>(this->parentWindow)->getWindow();
            if(!openPanel){
                /// If is Save Panel
                // __block NSURL *url = nil;
                [savePanel beginSheetModalForWindow:parentWindow completionHandler:^(NSModalResponse response){
                        NSURL * url = [savePanel URL];
                        strVal.set([url fileSystemRepresentation]);
                }];

            }
            else if(!savePanel){
                 /// If is Save Panel
                // __block NSURL *url = nil;
                [openPanel beginSheetModalForWindow:parentWindow completionHandler:^(NSModalResponse response){
//                    if(response == NSModalResponseOK){
//                        // url = [[openPanel URLs] firstObject];
//                    };
                    NSURL * url = [savePanel URL];
                    strVal.set([url fileSystemRepresentation]);
                }];

            };
            return strVal.async();
        };
        explicit CocoaFSDialog(const Descriptor &desc,NWH & nativeWindow):NativeFSDialog(nativeWindow){
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
            NSWindow *parentWindow = std::dynamic_pointer_cast<CocoaAppWindow>(this->parentWindow)->getWindow();
            [dialog beginSheetModalForWindow:parentWindow completionHandler:^(NSModalResponse response){
                
            }];
        };
        explicit CocoaNoteDialog(const Descriptor &desc,NWH & nativeWindow):NativeNoteDialog(nativeWindow){
            dialog = [[NSAlert alloc] init];
            dialog.showsHelp = NO;
            dialog.alertStyle = NSAlertStyleInformational;
            dialog.messageText = Cocoa::common_string_to_ns_string(desc.title);
            dialog.informativeText = Cocoa::common_string_to_ns_string(desc.str);
            show();
        };
    };

};

namespace OmegaWTK::Native {
    SharedHandle<NativeFSDialog> NativeFSDialog::Create(const NativeFSDialog::Descriptor &desc,NWH nativeWindow){
        return SharedHandle<NativeFSDialog>(new Cocoa::CocoaFSDialog(desc,nativeWindow));
    };

    SharedHandle<NativeNoteDialog> NativeNoteDialog::Create(const NativeNoteDialog::Descriptor &desc,NWH nativeWindow){
        return SharedHandle<NativeNoteDialog>(new Cocoa::CocoaNoteDialog(desc,nativeWindow));
    };
}
