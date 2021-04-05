#include "WinDialog.h"
#include "NativePrivate/win/HWNDItem.h"

namespace OmegaWTK::Native::Win {

    WinFSDialog::WinFSDialog(bool read_or_write,NWH nativeWindow):NativeFSDialog(nativeWindow),read_or_write(read_or_write){
        if(read_or_write)
            dialog_ty_1.CoCreateInstance(CLSID_FileOpenDialog);
        else 
            dialog_ty_2.CoCreateInstance(CLSID_FileSaveDialog);
    };

    void WinFSDialog::close(){
        HRESULT hr;
        if(read_or_write){
            hr = dialog_ty_1->Close(S_OK);
        }
    };


    void WinFSDialog::show(){
        HRESULT hr;
        if(read_or_write){
            hr = dialog_ty_1->Show(((HWNDItem *)parentWindow)->hwnd);
        }
        else {
             hr = dialog_ty_2->Show(((HWNDItem *)parentWindow)->hwnd);
        };
    };

    WinFSDialog::~WinFSDialog(){
        if(dialog_ty_1 != nullptr)
            dialog_ty_1.Release();
        else 
            dialog_ty_2.Release();
    };

    SharedHandle<NativeFSDialog> WinFSDialog::Create(const Descriptor &desc,NWH nativeWindow){
        HRESULT hr;
        if(desc.type == Read){
            return std::make_shared<WinFSDialog>(true,nativeWindow);
        }
        else {

            return std::make_shared<WinFSDialog>(false,nativeWindow);
        }
    };

    WinNoteDialog::WinNoteDialog(const Descriptor &desc,NWH nativeWindow):NativeNoteDialog(nativeWindow){
        
    };

    WinNoteDialog::~WinNoteDialog(){
        
    };

    SharedHandle<NativeNoteDialog> WinNoteDialog::Create(const Descriptor &desc, NWH nativeWindow){
        return std::make_shared<WinNoteDialog>(desc,nativeWindow);
    };

    void WinNoteDialog::show(){

    };

    void WinNoteDialog::close(){

    };
}