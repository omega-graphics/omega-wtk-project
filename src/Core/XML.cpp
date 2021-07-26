#include "omegaWTK/Core/XML.h"

#include <libxml2/libxml/tree.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/xmlIO.h>

namespace OmegaWTK::Core {

    XMLDocument XMLDocument::parseFromString(OmegaCommon::String str){
        std::istringstream is(str);
        return parseFromStream(is);
    };

    XMLDocument XMLDocument::parseFromStream(std::istream &in){
        size_t fileSize;
        in.seekg(0,std::ios::end);
        fileSize = in.tellg();
        in.seekg(0,std::ios::beg);
        char buffer[fileSize];

        in.read(buffer,fileSize);

        xmlDocPtr doc =  xmlParseMemory(buffer,fileSize);
        XMLDocument ret;
        ret.data = doc;
        return ret;
    };

    OmegaCommon::String XMLDocument::serialize(){
        std::ostringstream out;
        xmlChar *buffer;
        int _size;
        auto doc = (xmlDocPtr)data;
        xmlDocDumpMemory(doc,&buffer,&_size);
        out.write((char *)buffer,_size);
        return out.str();
    };

}