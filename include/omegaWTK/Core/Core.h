

#ifndef OMEGAWTK_CORE_CORE_H
#define OMEGAWTK_CORE_CORE_H

namespace OmegaWTK {
    #define STATIC_ENUM static constexpr unsigned char
    #define ENUM(name,args...) enum class name :unsigned char { args }; 
    struct CoreDimensions {
        unsigned x;
        unsigned y;
        unsigned minWidth;
        unsigned minHeight;
    };
    struct CoreRect {
        ENUM(Type,
            Regular,
            Rounded
        )
        CoreDimensions dimen;
       
    };
};

#endif