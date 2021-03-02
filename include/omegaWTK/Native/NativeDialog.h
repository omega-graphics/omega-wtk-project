#include "omegaWTK/Core/Core.h"
#include "NativeWindow.h"

#ifndef OMEGAWTK_NATIVE_NATIVEDIALOG_H
#define OMEGAWTK_NATIVE_NATIVEDIALOG_H

namespace OmegaWTK::Native {
    class NativeDialog {
        protected:
        NWH parentWindow;
        public:
        virtual void close() = 0;
    };

    class NativeFSDialog : public NativeDialog {
        
    };

    class NativeNoteDialog : public NativeDialog {

    };
};

#endif