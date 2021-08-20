#include "omegaWTK/Core/XML.h"

#include <libxml/xmlversion.h>
#include <libxml/parser.h>
#include <libxml/xmlIO.h>

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

        auto * buffer = new char[fileSize];

        in.read(buffer,fileSize);
        
        auto doc =  xmlParseMemory(buffer,fileSize);
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