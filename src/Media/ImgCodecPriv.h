#include "omegaWTK/Media/ImgCodec.h"

namespace OmegaWTK::Media {

    #ifdef TARGET_WIN32
#define DEFAULT_SCREEN_GAMMA 2.2
#endif

#ifdef TARGET_MACOS
#define DEFAULT_SCREEN_GAMMA 2.2
#endif

    class ImgCodec {
    protected:
        Core::IStream & in;
        BitmapImage *storage;
    public:
        virtual void readToStorage() = 0;
        ImgCodec(Core::IStream & _in,BitmapImage *res):in(_in),storage(res){};
        virtual ~ImgCodec(){};
    };


}