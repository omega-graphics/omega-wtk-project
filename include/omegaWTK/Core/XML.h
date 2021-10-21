#include "Core.h"

#ifndef OMEGAWTK_CORE_XML_H
#define OMEGAWTK_CORE_XML_H

namespace OmegaWTK {
    namespace Core {

        class XMLDocument {
            void *data;
        public:
            class Tag {
                void *data;
                explicit Tag(void *data);
                friend class XMLDocument;
            public:
                bool isElement() const;
                bool isText() const;
                OmegaCommon::StrRef name();
                OmegaCommon::StrRef content();
                OmegaCommon::StrRef attribute(const OmegaCommon::StrRef & name);
                OmegaCommon::Vector<Tag> children();
            };
            Tag root();
            OmegaCommon::String serialize();

            void serializeToStream(std::ostream & out);

            static XMLDocument parseFromStream(std::istream & in);
            static XMLDocument parseFromString(OmegaCommon::String str);
            ~XMLDocument();
        };

    }
}


#endif