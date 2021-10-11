#include <windows.h>
#include <ShlObj_core.h>
#include <CommCtrl.h>
#include <ShObjIdl.h>
#include <atlbase.h>
#include <windowsx.h>

#include "omegaWTK/Native/NativeDialog.h"

#ifndef OMEGAWTK_NATIVE_WIN_WINDIALOG_H
#define OMEGAWTK_NATIVE_WIN_WINDIALOG_H

namespace OmegaWTK::Native::Win {
    class WinFSDialog : public NativeFSDialog {
        bool read_or_write;
        ATL::CComPtr<IFileOpenDialog> dialog_ty_1;
        ATL::CComPtr<IFileSaveDialog> dialog_ty_2;
        void close();
        void show();
    public:
        WinFSDialog(bool read_or_write,NWH nativeWindow);
        ~WinFSDialog();
        static SharedHandle<NativeFSDialog> Create(const Descriptor & desc,NWH nativeWindow);
        OmegaCommon::Async<OmegaCommon::String> getResult() override;
    };

    class WinNoteDialog : public NativeNoteDialog {
        static INT_PTR DlgProc(HWND , UINT, WPARAM, LPARAM);
        HGLOBAL hgbl;
        public:
        WinNoteDialog(const Descriptor & desc,NWH nativeWindow);
        ~WinNoteDialog();
        void show();
        void close();
        static SharedHandle<NativeNoteDialog> Create(const Descriptor & desc,NWH nativeWindow);
    };
}

#endif