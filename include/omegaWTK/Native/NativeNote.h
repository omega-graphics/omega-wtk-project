#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTK_NATIVE_NATIVENOTE_H
#define OMEGAWTK_NATIVE_NATIVENOTE_H

namespace OmegaWTK::Native {

    struct NativeNote {
        OmegaCommon::String title;
         OmegaCommon::String body;
    };

    INTERFACE NativeNoteCenter {
    public:
        INTERFACE_METHOD void sendNativeNote(NativeNote & note) ABSTRACT;
    };

    using NNCP = SharedHandle<NativeNoteCenter>;

    NNCP make_native_note_center();

}

#endif