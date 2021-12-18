#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTK_NATIVE_NATIVENOTE_H
#define OMEGAWTK_NATIVE_NATIVENOTE_H

namespace OmegaWTK::Native {

    class NativeNoteCenter {
        
    };

    typedef SharedHandle<NativeNoteCenter> NNCP;
    NNCP make_native_note_center();

}

#endif