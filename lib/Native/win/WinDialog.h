#include <windows.h>
#include <ShlObj_core.h>
#include <CommCtrl.h>
#include <ShObjIdl.h>
#include <atlbase.h>

#include "omegaWTK/Native/NativeDialog.h"

#ifndef OMEGAWTK_NATIVE_WIN_WINDIALOG_H
#define OMEGAWTK_NATIVE_WIN_WINDIALOG_H

namespace OmegaWTK::Native::Win {
    class WinFSDialog : public NativeFSDialog {
        bool read_or_write;
        ATL::CComPtr<IFileOpenDialog> dialog_ty_1;
        ATL::CComPtr<IFileSaveDialog> dialog_ty_2;
        WinFSDialog(bool read_or_write,NWH nativeWindow);
        ~WinFSDialog();
        void close() override;
        void show() override;
    public:
        static SharedHandle<NativeFSDialog> Create(const Descriptor & desc,NWH nativeWindow);
    };
}

#endif