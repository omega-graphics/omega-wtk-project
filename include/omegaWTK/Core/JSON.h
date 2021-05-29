#include "Core.h"

#ifndef OMEGAWTK_CORE_JSON_H
#define OMEGAWTK_CORE_JSON_H

namespace OmegaWTK {

    class JSON;

     /// @interface JSONSerializable
    class JSONSerializable {
        virtual void toJSON(JSON & json_out) = 0;
        virtual void toObject(JSON &json_in) = 0;
    };
    
    class JSON {

    public:
        static JSON parse(Core::String str);
        template<class _Ty,std::enable_if_t<std::is_base_of_v<JSONSerializable,_Ty>> = 0>
        static _Ty parse(Core::String str){
            _Ty obj;
            auto json = parse(str);
            obj.toObject(json);
            return obj;
        }; 
        static Core::String serialize(JSONSerializable &ser);
        static Core::String serialize(JSON &json);
    };
};

#endif