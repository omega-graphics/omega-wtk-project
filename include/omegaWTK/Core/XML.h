#include "Core.h"

#ifndef OMEGAWTK_CORE_XML_H
#define OMEGAWTK_CORE_XML_H

namespace OmegaWTK {
    namespace Core {

        class XMLDocument {
            void *data;
        public:
            OmegaCommon::String serialize();

            void serializeToStream(std::ostream & out);

            static XMLDocument parseFromStream(std::istream & in);
            static XMLDocument parseFromString(OmegaCommon::String str);
        };

    }
}


#endif