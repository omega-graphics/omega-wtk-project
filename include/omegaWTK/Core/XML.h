#include "Core.h"

#ifndef OMEGAWTK_CORE_XML_H
#define OMEGAWTK_CORE_XML_H

namespace OmegaWTK {
    class XMLSerializable;

    struct XMLTag {
        struct Prop {
            Core::String name;
            Core::Optional<Core::String> value;
        };
        Core::String name;
        Core::Vector<Prop> props;
        Core::Vector<XMLTag *> children;
    };

    class XMLParser {
        void _parseFromStreamPriv(Core::IStream & is,XMLSerializable &obj);
    public:
        void parseFromFile(Core::IStream & is,XMLSerializable & output){
            _parseFromStreamPriv(is,output);
        };
       
        void parseFromBuffer(void *data,size_t size,XMLSerializable & output){
            std::ifstream is;
            is.flags(std::ios::in | std::ios::binary);
            is.rdbuf()->pubsetbuf((char *)data,size);
            _parseFromStreamPriv(is,output);
        };
    };

    class XMLSerializable {
    public:
        virtual void toXml(XMLTag **tag) = 0;
        virtual void fromXML(XMLTag &tag) = 0;
    };

    class XMLSerializer {
        void serializeToFile(Core::OStream &os,XMLSerializable & object);
        void serializeToBuffer(void *data,size_t *size,XMLSerializable &object);
    };

};

#endif