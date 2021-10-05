#include "View.h"
#include "omegaWTK/Media/Video.h"
#include "omegaWTK/Media/VideoCodec.h"

#ifndef OMEGAWTK_UI_VIDEOVIEW_H
#define OMEGAWTK_UI_VIDEOVIEW_H

namespace OmegaWTK {

/**
 @brief 
*/
class OMEGAWTK_EXPORT VideoView :  public View, 
                                   public Media::VideoFrameSink {
   VideoView(const Core::Rect & rect,Composition::Compositor * comp,View *parent = nullptr);
};
/**
 @brief 
*/

};

#endif
