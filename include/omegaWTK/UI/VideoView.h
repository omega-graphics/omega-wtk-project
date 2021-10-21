#include "View.h"
#include "omegaWTK/Media/Video.h"

#ifndef OMEGAWTK_UI_VIDEOVIEW_H
#define OMEGAWTK_UI_VIDEOVIEW_H

namespace OmegaWTK {

/**
 @brief 
*/
class OMEGAWTK_EXPORT VideoView :  public View, 
                                   public Media::VideoFrameSink {
    friend class Widget;
   VideoView(const Core::Rect & rect,Composition::LayerTree * layerTree,View *parent = nullptr);
};
/**
 @brief 
*/

};

#endif
