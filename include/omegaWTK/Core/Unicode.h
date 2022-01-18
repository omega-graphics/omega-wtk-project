/// @file ICU C++ Bindings!


 #include <unicode/calendar.h>
 #include <unicode/ustring.h>
 #include <unicode/unistr.h>
 #include <unicode/coll.h>
 #include <unicode/brkiter.h>
 #include <unicode/dtfmtsym.h>
 #include <unicode/uchar.h>
 #include <unicode/gender.h>
 #include <unicode/ucnv.h>


#ifndef OMEGAWTK_CORE_UNICODE_H
#define OMEGAWTK_CORE_UNICODE_H



namespace OmegaWTK {

    // namespace Unicode = icu;
    typedef UChar UnicodeChar;
    typedef UChar32 Unicode32Char;
     typedef icu::UnicodeString UniString;

    
}

#endif