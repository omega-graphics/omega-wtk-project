#import "NativePrivate/ios/IOSItem.h"
#include <memory>

@interface OmegaWTKMobileUIView : UIView 
@property(readonly) OmegaWTK::Mobile::Native::IOSItem *delegate;
-(instancetype)initWithFrame:(CGRect)frame andDelegate:(OmegaWTK::Mobile::Native::IOSItem *) delegate;
@end

@interface OmegaWTKMobileUIViewController : UIViewController
-(instancetype)initWithFrame:(CGRect)frame andDelegate:(OmegaWTK::Mobile::Native::IOSItem *) delegate;
@end

namespace OmegaWTK::Mobile::Native {
    IOSItem::IOSItem(const Core::Rect &rect): NativeItem(rect) {
        viewC = [[OmegaWTKMobileUIViewController alloc]initWithFrame:CGRectMake(0,0,0,0) andDelegate:this];
    }

    void IOSItem::enable(){
        if(viewC.view.isHidden == YES){
            [viewC.view setHidden:NO];
        }
    }

     void IOSItem::disable(){
         if(viewC.view.isHidden == NO){
            [viewC.view setHidden:YES];
        }
    }

    void IOSItem::resize(const Core::Rect &newRect){
        [viewC.view setBounds:CGRectMake(0,0,newRect.w,newRect.h)];
        [viewC.view setFrame:CGRectMake(newRect.pos.x,newRect.pos.y,newRect.w,newRect.h)];
    }

    void IOSItem::addChildNativeItem(SharedHandle<NativeItem> &child){
        auto item = std::dynamic_pointer_cast<IOSItem>(child);
        [viewC.view addSubview:item->viewC.view];
    }

    void IOSItem::removeChildNativeItem(SharedHandle<NativeItem> &child){
        auto item = std::dynamic_pointer_cast<IOSItem>(child);
        [item->viewC.view removeFromSuperview];
    }
    NativeItemPtr NativeItem::Create(const Core::Rect &rect){
        return (NativeItemPtr)new IOSItem(rect);
    }
}


@implementation  OmegaWTKMobileUIView
- (instancetype)initWithFrame:(CGRect)frame andDelegate:(OmegaWTK::Mobile::Native::IOSItem *)delegate {
    if(self = [super initWithFrame:frame]){
        _delegate = delegate;
        self.layer.contentsScale = [UIScreen mainScreen].scale;
    }
    return self;
}
@end

@implementation OmegaWTKMobileUIViewController 
- (instancetype)initWithFrame:(CGRect)frame andDelegate:(OmegaWTK::Mobile::Native::IOSItem *)delegate {
    if(self = [super init]){
        self.view = [[OmegaWTKMobileUIView alloc] initWithFrame:frame andDelegate:delegate];
    }
    return self;
}
- (void)viewDidLoad {

}
@end