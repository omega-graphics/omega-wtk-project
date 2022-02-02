#include "omegaWTK/Composition/FontEngine.h"
#include "NativePrivate/macos/CocoaUtils.h"

#include "omegaWTK/Core/Unicode.h"

#import <CoreText/CoreText.h>
#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>

namespace OmegaWTK::Composition {

 CFStringRef core_string_to_cf_string_ref(OmegaCommon::String & str){
     return CFStringCreateWithCString(kCFAllocatorDefault,str.c_str(),CFStringGetSystemEncoding());
 };

 FontEngine * FontEngine::instance;

 FontEngine::FontEngine(){
    
 };

 FontEngine::~FontEngine() = default;

FontEngine *FontEngine::inst() {
    return instance;
}

 void FontEngine::Create(){
         instance = new FontEngine();
     };

     void FontEngine::Destroy(){
         delete instance;
     };



 class CoreTextFont : public Font {
     CTFontRef native;
     friend class CTTextRect;
 public:
     CoreTextFont(FontDescriptor & desc,CTFontRef ref):Font(desc),native(ref){};
     void * getNativeFont(){
         return (void *)native;
     };
 };

class CTGlyphRun : public GlyphRun {
public:
    NSAttributedString *str;
    Core::SharedPtr<CoreTextFont> font;

    Core::Rect getBoundingRectOfGlyphAtIndex(size_t glyphIdx) override {
        return {};
    }

};

Core::SharedPtr<GlyphRun>
GlyphRun::fromUStringAndFont(const OmegaWTK::UniString &str, Core::SharedPtr<Font> &font) {
    auto run = new CTGlyphRun();
    run->str = [[NSAttributedString alloc] initWithString:[NSString stringWithCharacters:(const unichar *)str.getBuffer() length:str.length()]];
    run->font = std::dynamic_pointer_cast<CoreTextFont>(font);
    return (Core::SharedPtr<GlyphRun>)run;
}

 class CTTextRect : public TextRect {
     CTFramesetterRef framesetterRef;
     CTFrameRef frame;
     NSAttributedString *strData;
     void _updateStrInternal(){
        
     };
 public:
     CTTextRect(Core::Rect & rect,const TextLayoutDescriptor &layoutDesc):TextRect(rect){
         NSLog(@"CTTextRect Create With W: %f H: %f",rect.w,rect.h);
         NSMutableParagraphStyle *paragraphStyle = [[NSMutableParagraphStyle alloc] init];
        
         /// Initalize Text with Settings!
         // [paragraphStyle setParagraphStyle:[NSParagraphStyle defaultParagraphStyle]];
         // NSTextAlignment hAlignment;
         // NSTextBlockVerticalAlignment vAlignment;
         // switch (font->desc.textAlignment) {
         //     case FontDescriptor::LeftUpper: {
         //         hAlignment = NSTextAlignmentLeft;
         //         vAlignment = NSTextBlockTopAlignment;
         //         break;
         //     }
         //     case FontDescriptor::LeftCenter : {
         //         hAlignment = NSTextAlignmentLeft;
         //         vAlignment = NSTextBlockMiddleAlignment;
         //         break;
         //     };
         //     case FontDescriptor::LeftLower : {
         //         hAlignment = NSTextAlignmentLeft;
         //         vAlignment = NSTextBlockBottomAlignment;
         //         break;
         //     };
         //     case FontDescriptor::MiddleUpper: {
         //         hAlignment = NSTextAlignmentCenter;
         //         vAlignment = NSTextBlockTopAlignment;
         //         break;
         //     }
         //     case FontDescriptor::MiddleCenter : {
         //         hAlignment = NSTextAlignmentCenter;
         //         vAlignment = NSTextBlockMiddleAlignment;
         //         break;
         //     };
         //     case FontDescriptor::MiddleLower : {
         //         hAlignment = NSTextAlignmentCenter;
         //         vAlignment = NSTextBlockBottomAlignment;
         //         break;
         //     };
         //     case FontDescriptor::RightUpper: {
         //         hAlignment = NSTextAlignmentRight;
         //         vAlignment = NSTextBlockTopAlignment;
         //         break;
         //     }
         //     case FontDescriptor::RightCenter : {
         //         hAlignment = NSTextAlignmentRight;
         //         vAlignment = NSTextBlockMiddleAlignment;
         //         break;
         //     };
         //     case FontDescriptor::RightLower : {
         //         hAlignment = NSTextAlignmentRight;
         //         vAlignment = NSTextBlockBottomAlignment;
         //         break;
         //     };
         //     default:
         //         break;
         // }
         // NSLineBreakMode lineBreakOpts;
         // switch (font->desc.wrapping) {
         //     case FontDescriptor::WrapByWord : {
         //         lineBreakOpts = NSLineBreakByWordWrapping;
         //         break;
         //     }
         //     case FontDescriptor::WrapByCharacter : {
         //         lineBreakOpts = NSLineBreakByCharWrapping;
         //         break;
         //     };
         //     case FontDescriptor::None : {
         //         lineBreakOpts = NSLineBreakByClipping;
         //         break;
         //     };
         //     default:
         //         break;
         // }
        
         // [paragraphStyle setAlignment:hAlignment];
         // [paragraphStyle setLineBreakMode:lineBreakOpts];
         // for(NSTextBlock *textBlock in paragraphStyle.textBlocks){
         //     [textBlock setVerticalAlignment:vAlignment];
         // };
        
         // float scaleFactor = [NSScreen mainScreen].backingScaleFactor;
         // auto ftextRect = OmegaWTK::Core::Rect {{float(rect.pos.x) * scaleFactor,float(rect.pos.y) * scaleFactor},float(rect.w) * scaleFactor,float(rect.h) * scaleFactor};
         // CoreTextFont *fontRef = (CoreTextFont *)font.get();

        
         // strData = [[NSAttributedString alloc] initWithString:[[NSString alloc] initWithCharacters:(const unichar *)text_val.getBuffer() length:text_val.length()] attributes:@{NSParagraphStyleAttributeName:paragraphStyle,NSFontAttributeName:(__bridge id)fontRef->native}];
         // // Draw Text to CGBitmap!
         // framesetterRef = CTFramesetterCreateWithAttributedString((__bridge CFAttributedStringRef)strData);
         // frame = CTFramesetterCreateFrame(framesetterRef,CFRangeMake(0,0),CGPathCreateWithRect(CGRectMake(ftextRect.pos.x,ftextRect.pos.y, ftextRect.w, ftextRect.h),NULL),NULL);
         // CFRetain(frame);
     };
     void drawRun(Core::SharedPtr<GlyphRun> &glyphRun, const Composition::Color &color) override {
          // Draw Text to CGBitmap!
          framesetterRef = CTFramesetterCreateWithAttributedString((__bridge CFAttributedStringRef)strData);
          frame = CTFramesetterCreateFrame(framesetterRef,CFRangeMake(0,0),CGPathCreateWithRect(CGRectMake(rect.pos.x,rect.pos.y,rect.w,rect.h),NULL),NULL);
          CFRetain(frame);
     }
     void * getNative(){
         return (void *)frame;
     };
     void getGlyphBoundingBoxes(Core::Rect **rects, unsigned * count){
         *count = 0;
         CoreTextFont *fontRef = nullptr;
         CFArrayRef lines = CTFrameGetLines(frame);
         for(unsigned idx = 0;idx < CFArrayGetCount(lines);idx++){
             CTLineRef line = (CTLineRef)CFArrayGetValueAtIndex(lines,idx);
             CFArrayRef runs =  CTLineGetGlyphRuns(line);
             for(unsigned j = 0;j < CFArrayGetCount(runs);j++){
                 CTRunRef run = (CTRunRef)CFArrayGetValueAtIndex(runs,j);
                 const CGGlyph *ptr = CTRunGetGlyphsPtr(run);
                 CFIndex count = CTRunGetGlyphCount(run);
                 CGRect *rect = new CGRect[count];
                 CTFontGetBoundingRectsForGlyphs(fontRef->native,kCTFontOrientationDefault,ptr,rect,count);
             };
         }
     };
     // void reload() {
        
     // };
     OmegaGTE::SharedHandle<OmegaGTE::GETexture> toBitmap() override{
         CGFloat scaleFactor = [NSScreen mainScreen].backingScaleFactor;
         void *data = new unsigned char[rect.w * rect.h * scaleFactor * scaleFactor];
         CGContextRef context = CGBitmapContextCreate(data,rect.w * scaleFactor,rect.h * scaleFactor,8,rect.w * 4 * 8,CGColorSpaceCreateDeviceRGB(),0);
         CGContextSetTextMatrix(context,CGAffineTransformIdentity);
         CGContextScaleCTM(context,1,-1);
         CTFrameDraw(frame,context);
         CGContextRelease(context);

         OmegaGTE::TextureDescriptor desc {};
         desc.type = OmegaGTE::GETexture::Texture2D;
         desc.width = rect.w;
         desc.height = rect.h;
         desc.storage_opts = OmegaGTE::Shared;

         auto texture = gte.graphicsEngine->makeTexture(desc);
     
         texture->copyBytes(data,rect.w * rect.h * scaleFactor * scaleFactor);
      
         return texture;
     };
     ~CTTextRect(){
         // CFRelease(frame);
         // CFRelease(framesetterRef);
         // [strData release];
     };
 };

 Core::SharedPtr<TextRect> TextRect::Create(Core::Rect rect,const TextLayoutDescriptor & layoutDesc){
     return Core::SharedPtr<TextRect>(new CTTextRect(rect,layoutDesc));
 };

 Core::SharedPtr<Font> FontEngine::CreateFont(FontDescriptor & desc){
     CTFontRef ref = CTFontCreateWithNameAndOptions(core_string_to_cf_string_ref(desc.family),CGFloat(desc.size),NULL,kCTFontOptionsPreferSystemFont);
     CTFontSymbolicTraits fontTraits;
    
     switch (desc.style) {
         case FontDescriptor::Bold:
             fontTraits = kCTFontTraitBold;
             break;
         case FontDescriptor::Italic:
             fontTraits = kCTFontTraitItalic;
             break;
         case FontDescriptor::BoldAndItalic:
             fontTraits = kCTFontTraitBold | kCTFontTraitItalic;
             break;
         case FontDescriptor::Regular :
             fontTraits = 0;
             break;
         default:
             break;
     }
    
     auto scaleFactor = [NSScreen mainScreen].backingScaleFactor;
    
     CTFontRef _font_final = CTFontCreateCopyWithSymbolicTraits(ref,CGFloat(desc.size) * scaleFactor,NULL,kCTFontTraitBold | kCTFontTraitItalic, fontTraits);
     CFRelease(ref);
     return SharedHandle<Font>(new CoreTextFont(desc,_font_final));
 };

 Core::SharedPtr<Font> FontEngine::CreateFontFromFile(OmegaCommon::FS::Path path, FontDescriptor &desc){
     CTFontSymbolicTraits fontTraits;
    
     switch (desc.style) {
         case FontDescriptor::Bold:
             fontTraits = kCTFontTraitBold;
             break;
         case FontDescriptor::Italic:
             fontTraits = kCTFontTraitItalic;
             break;
         case FontDescriptor::BoldAndItalic:
             fontTraits = kCTFontTraitBold | kCTFontTraitItalic;
             break;
         case FontDescriptor::Regular :
             fontTraits = 0;
             break;
         default:
             break;
     }

     NSURL *url = [NSURL fileURLWithPath:Native::Cocoa::common_string_to_ns_string(path.absPath()) isDirectory:NO];
     CFArrayRef fontDescriptors = CTFontManagerCreateFontDescriptorsFromURL((__bridge CFURLRef)url);
     CTFontDescriptorRef idealFont;
     for(unsigned i = 0;i < CFArrayGetCount(fontDescriptors);i++){
         CTFontDescriptorRef fd = (CTFontDescriptorRef)CFArrayGetValueAtIndex(fontDescriptors,i);
         CFStringRef name = (CFStringRef)CTFontDescriptorCopyAttribute(fd,kCTFontNameAttribute);
         CFNumberRef symbolicTraits = (CFNumberRef)CTFontDescriptorCopyAttribute(fd,kCTFontSymbolicTrait);
         NSString *str = (__bridge id)name;
         int val;
         CFNumberGetValue(symbolicTraits,kCFNumberIntType,&val);
         if(desc.family == str.UTF8String && fontTraits == val){
             idealFont = fd;
         };
     };

     auto scaleFactor = [NSScreen mainScreen].backingScaleFactor;

     CTFontRef f = CTFontCreateWithFontDescriptor(idealFont,CGFloat(desc.size) * scaleFactor,NULL);
     return std::make_shared<CoreTextFont>(desc,f);
 };
};
