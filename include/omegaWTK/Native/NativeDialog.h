#include "omegaWTK/Core/Core.h"
#include "NativeWindow.h"

#ifndef OMEGAWTK_NATIVE_NATIVEDIALOG_H
#define OMEGAWTK_NATIVE_NATIVEDIALOG_H

namespace OmegaWTK::Native {
    class NativeDialog {
        protected:
         NativeDialog(NWH parentWindow);
        NWH parentWindow;
        public:
        virtual void show() = 0;
        virtual void close() = 0;
    };

    class NativeFSDialog : public NativeDialog {
    protected:
        NativeFSDialog(NWH parentWindow);
    public:
        typedef enum : OPT_PARAM {
            Read,
            Write
        } Type;
        struct Descriptor {
            Type type;
        };
        static SharedHandle<NativeFSDialog> Create(const Descriptor & desc,NWH nativeWindow);
        virtual void show() = 0;
        virtual void close() = 0;
    };

    class NativeNoteDialog : public NativeDialog {
    protected:
        NativeNoteDialog(NWH parentWindow);
    public:
        struct Descriptor {
            Core::String title;
            Core::String str;
        };
        static SharedHandle<NativeNoteDialog> Create(const Descriptor & desc,NWH nativeWindow);
         virtual void show() = 0;
         virtual void close() = 0;
    };

    
};

#endif
