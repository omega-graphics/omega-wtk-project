#include "Core.h"

#ifndef OMEGAWTK_CORE_FS_H
#define OMEGAWTK_CORE_FS_H 

namespace OmegaWTK {

namespace FS {

    class OMEGAWTK_EXPORT  Path {
        Core::String _str;
            struct Token {
                typedef enum : OPT_PARAM {
                    ID,
                    Dot,
                    Slash,
                } Type;
                Type type;
                Core::String str;
            };
            Core::Vector<Token> tokens;
            void parse(const Core::String & str);
        public:
            const unsigned getTokenCount(){ return tokens.size();};
            Core::String debugString(){
                std::ostringstream out;
                auto it = tokens.begin();
                while(it != tokens.end()){
                    out << "{Type:" << int(it->type) << ",Content:" << it->str << "}, " << std::flush;
                    ++it;
                };
                return out.str();
            };
            Core::String &str();
            Core::String filename();
            Core::String & ext();
            Core::String absPath();
            bool exists();
            Path(const char * str);
            Path(const Core::String & str);
            ~Path();
            bool isFile();
            bool isDirectory();
            bool isSymLink();
        };

        StatusCode changeCWD(const Path & newPath);

        StatusCode createSymLink(const Path & file,const Path & symlinkDest);

        StatusCode createDirectory(const Path & path);

        StatusCode deleteDirectory(const Path & path);
    };
};

#endif