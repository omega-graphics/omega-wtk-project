#include "omegaWTK/Media/ImgCodec.h"

#include <zlib.h>

#include <png.h>

//#include <tiff.h>
#include <tiffio.hxx>

// #include <jpeglib.h>
// #include <turbojpeg.h>

#include <iostream>
#include <fstream>
#include <memory>



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


    
class PNGCodec : public ImgCodec {
    int srgb_intent;
    bool hasPalette;
    int num_palette;
    png_colorp palette;
    static void userReadData(png_structp png_ptr, png_bytep data, png_size_t length){
        png_voidp stream = png_get_io_ptr(png_ptr);
        ((std::istream *)stream)->read((char *)data,length);
     };
    #define SIG_SIZE 8
    bool validate_signature(){


       png_byte sig[SIG_SIZE];
       
       in.read((char *)sig,SIG_SIZE);
       if(!in.good()){
           return false;
       };
       
       return png_check_sig(sig,SIG_SIZE);
    };
    
    Core::UniquePtr<ImgHeader> read_header(png_structp png_ptr,png_infop info_ptr){
        BitmapImage::ColorFormat colorFormat;
        BitmapImage::AlphaFormat alphaFormat;
        
        uint32 width;
        uint32 height;
        int bitDepth;
        int channels;
        int color_type;
        int filter_method;
        int compression_method;
        int interlace_type;
        png_get_IHDR(png_ptr,info_ptr,&width,&height,&bitDepth,&color_type,&interlace_type,&compression_method,&filter_method);
        channels = png_get_channels(png_ptr,info_ptr);
        
        if(bitDepth == 16){
            png_set_strip_16(png_ptr);
            bitDepth = 8;
        };
        
        size_t rowBytes = (width * channels * bitDepth)/8;
        
        switch (color_type) {
            case PNG_COLOR_TYPE_RGBA: {
                colorFormat = BitmapImage::ColorFormat::RGBA;
                alphaFormat = BitmapImage::AlphaFormat::Straight;
                break;
            }
            case PNG_COLOR_TYPE_RGB : {
                png_bytep trans_alpha;
                int num_trans;
                png_color_16p trans_color;
                if(png_get_tRNS(png_ptr,info_ptr,&trans_alpha,&num_trans,&trans_color) == PNG_INFO_tRNS){
                    colorFormat = BitmapImage::ColorFormat::RGBA;
                    png_set_tRNS_to_alpha(png_ptr);
                };
                
                break;
            }
            case PNG_COLOR_TYPE_GRAY : {
                png_set_gray_to_rgb(png_ptr);
                break;
            };
            case PNG_COLOR_TYPE_PALETTE : {
                if(png_get_PLTE(png_ptr,info_ptr,&palette,&num_palette) == PNG_INFO_PLTE){
                    png_set_palette_to_rgb(png_ptr);
                };
                break;
            }
            default:
                break;
        }
        std::cout << std::endl;
        std::stringstream ss;
        ss << "IHDR Chunk:{Width:" << width << ", Height:" << height << ", bitDepth:" << bitDepth << ", channels:" << channels << ", colorType:" << int(color_type) << ", filterType:" << filter_method << ", compressiomMethod:" << compression_method << ", interlaceType:" << interlace_type << ", rowBytes:" << rowBytes << "}" << std::endl;
        std::cout << ss.str();
        // MessageBoxA(GetForegroundWindow(),ss.str().c_str(),"NOTE",MB_OK);
        
        return std::make_unique<ImgHeader>(ImgHeader({width,height,channels,bitDepth,compression_method,interlace_type,colorFormat,alphaFormat,rowBytes}));
    };
        
    Core::UniquePtr<ImgProfile> read_profile(png_structp png_ptr,png_infop info_ptr){
        png_charp name;
        int compression_ty;
        png_bytep profile;
        png_uint_32 length;
        
        if(png_get_iCCP(png_ptr,info_ptr,&name,&compression_ty,&profile,&length) == PNG_INFO_iCCP){
            std::cout << "iCCP Chunk:{" << "name:" << name << ", compression:" << compression_ty << ", length:" << length << "}" << std::endl;
            storage->sRGB = false;
            return std::make_unique<ImgProfile>(ImgProfile({name,compression_ty}));
        }
        /// Else use SRGB!
        int srgb_intent;
        if (png_get_sRGB(png_ptr,info_ptr,&srgb_intent) == PNG_INFO_sRGB) {
            std::cout << "SRGB Chunk:{" << "srgbIntent:" << srgb_intent << "}" << std::endl;
            storage->sRGB = true;
            this->srgb_intent = srgb_intent;
            // png_set_sRGB_gAMA_and_cHRM(png_ptr,info_ptr,srgb_intent);
        }
        return nullptr;
    };


    bool load_png_from_file(){
           if(validate_signature()){
               png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
               if(!png_ptr){
                   std::cerr << "Failed to Create Read Struct!" << std::endl;
                   return false;
               };

               png_infop info_ptr = png_create_info_struct(png_ptr);
               if(!info_ptr){
                   std::cerr << "Failed to Create Info Struct" << std::endl;
                   png_destroy_read_struct(&png_ptr,0,0);
                   return false;
               };

               png_bytep* rowPtrs = NULL;
               char* data = NULL;

               if(setjmp(png_jmpbuf(png_ptr))){

                   png_destroy_read_struct(&png_ptr, &info_ptr,0);
                   if (rowPtrs != NULL) delete [] rowPtrs;
                   if (data != NULL) delete [] data;

                   return false;

               };

               png_set_read_fn(png_ptr,(png_voidp)&in,userReadData);

               png_set_sig_bytes(png_ptr,SIG_SIZE);

               png_read_info(png_ptr,info_ptr);
               auto log_png_color_16p = [&](png_color_16p color){
                   std::cout << "{" << "r:" << color->red << ", g:" << color->green << ", b:" << color->blue << ", index:" << color->index << "}" << std::endl;
               };
               /// Set Info!
               auto header = read_header(png_ptr,info_ptr);
               auto profile = read_profile(png_ptr,info_ptr);
    //           png_read_update_info(png_ptr,info_ptr);
               /// Background Chunk!
               png_color_16p background_color;
               if(png_get_bKGD(png_ptr,info_ptr,&background_color) == PNG_INFO_bKGD){
                   /// Log Background Chunk!
                   std::cout << "bKGD Chunk:{" << "backgroundColor:" << std::flush;
                   log_png_color_16p(background_color);
                   std::cout << std::endl;
               };
               int unit;
               double phys_width;
               double phys_height;
               /// SCAL Chunk!
               if(png_get_sCAL(png_ptr,info_ptr,&unit,&phys_width,&phys_height) == PNG_INFO_sCAL){
                   std::cout << "sCAL Chunk:{" << "unit:" << unit << ", width:" << phys_width << ", height:" << phys_height << " }" << std::endl;
               };
               /// Gamma Chunk!
               double file_gamma;
               if(png_get_gAMA(png_ptr,info_ptr,&file_gamma) == PNG_INFO_gAMA){
                   storage->hasGamma = true;
                   storage->gamma = file_gamma;
                   std::cout << "gAMA Chunk:{" << "fileGamma:" << file_gamma << " }" << std::endl;
                   if(!storage->sRGB){
                       char * gamma_str;
                       double screen_gamma;
                       png_set_gamma(png_ptr,DEFAULT_SCREEN_GAMMA,file_gamma);
                   };
               }
               else {
                   storage->hasGamma = false;
               };
               png_bytep trans_alpha;
               int num_trans;
               png_color_16p trans_color;
               if(png_get_tRNS(png_ptr,info_ptr,&trans_alpha,&num_trans,&trans_color) == PNG_INFO_tRNS){
                   std::cout << "tRNS Chunk:{" << "transAlpha:" << trans_alpha << ", numTrans:" << num_trans << ", transColor:" << std::flush;
                   log_png_color_16p(trans_color);
                   std::cout << " }" << std::endl;
               };

               /// Chroma Chunk!
               double chrm_white_x , chrm_white_y;
               double chrm_red_x , chrm_red_y;
               double chrm_green_x , chrm_green_y;
               double chrm_blue_x , chrm_blue_y;
               if(png_get_cHRM(png_ptr,info_ptr,&chrm_white_x,&chrm_white_y,&chrm_red_x, &chrm_red_y, &chrm_green_x, &chrm_green_y, &chrm_blue_x,&chrm_blue_y) == PNG_INFO_cHRM){
                   std::cout << "cHRM Chunk:{" << "whiteX:" << chrm_white_x << ", whiteY:" << chrm_white_y << ", redX:" << chrm_red_x << ", redY:" << chrm_red_y << ", greenX:" << chrm_green_x << ", greenY:" << chrm_green_y << ", blueX:" << chrm_blue_x << ", blueY:" << chrm_blue_y << " }" << std::endl;
               };
               /// If SRGB colorspace is used!
               if(storage->sRGB){
                   if(png_get_valid(png_ptr,info_ptr,PNG_INFO_cHRM) && png_get_valid(png_ptr,info_ptr,PNG_INFO_gAMA)) {
                       png_set_sRGB_gAMA_and_cHRM(png_ptr,info_ptr,srgb_intent);
                       storage->hasGamma = false;
                   }
                   else
                       png_set_sRGB(png_ptr,info_ptr,srgb_intent);
                   storage->sRGB = false;
               };
               png_charp purpose;
               png_int_32 X0;
               png_int_32 X1;
               int type;
               int nparams;
               png_charp units;
               png_charpp params;
               if(png_get_pCAL(png_ptr,info_ptr,&purpose,&X0,&X1,&type, &nparams, &units,&params) == PNG_INFO_pCAL){
                   std::cout << "pCAL Chunk:{" << "purpose:" << purpose << ", x0:" << X0 << ", x1:" << X1 << ", type:" << type << ", nParams:" << nparams << ", units:" << units << ", params:" << params << " }" << std::endl;
               };
               auto log_png_color_8p = [&](png_color_8p color){
                   std::cout << "{" << "r:" << color->red << ", g:" << color->green << ", b:" << color->blue << ", a:" << color->alpha << "}" << std::flush;
               };
               png_color_8p sig_bit;
               if(png_get_sBIT(png_ptr,info_ptr,&sig_bit) == PNG_INFO_sBIT){
                   std::cout << "sBIT Chunk:{sigBit:" << std::flush;
                   log_png_color_8p(sig_bit);
                   std::cout << " }" << std::endl;
               };
               png_uint_32 res_x;
               png_uint_32 res_y;
               int unit_type;
               if(png_get_pHYs(png_ptr,info_ptr,&res_x, &res_y,&unit_type) == PNG_INFO_pHYs){
                   std::cout << "pHYS Chunk:{" << "resX:" << res_x << ", resY:" << res_y << ", unitType:" << unit_type << " }" << std::endl;
               };
//
               png_timep time;
               
               if(png_get_tIME(png_ptr,info_ptr,&time) == PNG_INFO_tIME){
                   std::cout << "tIME Chunk:{" << "time:" << std::endl;
               };
               
               rowPtrs = new png_bytep[header->height];
               data = new char[(header->width * header->height * header->bitDepth * header->channels)/8];
               unsigned int stride = (header->width * header->bitDepth * header->channels)/8;
               
//               #ifdef TARGET_MACOS
//               for(size_t i = 0;i < header->height;i++){
//                   png_uint_32 ptr = (header->height - i - 1) * stride;
//                    /// Read from bottom to Top!
//                   rowPtrs[i] = (png_bytep)data + ptr;
//               };
//               #endif

               // #ifdef TARGET_WIN32
               for(size_t i = 0;i < header->height;i++){
                   png_uint_32 ptr = (header->height - i - 1) * stride;
                   /// Read from top to bottom!
                   rowPtrs[header->height - 1 - i] = (png_bytep)data + ptr;
               };
               // #endif

               png_read_image(png_ptr,rowPtrs);
               
               png_read_end(png_ptr,info_ptr);
               
               delete [] rowPtrs;
               png_destroy_read_struct(&png_ptr, &info_ptr,(png_infopp)0);

            //    if(header->bitDepth == 16){
            //        header->bitDepth = 8;
            //    };
               
               storage->data = data;
               storage->header = std::move(header);
               storage->profile = std::move(profile);
               
               return true;

           }
           else {
                   // MessageBox(GetForegroundWindow(),"Invalid Signature!","NOTE",MB_OK);
               return false;
           }
};
public:
    void readToStorage() {
        if(!load_png_from_file()){
            storage->data = nullptr;
            storage->header = nullptr;
            storage->profile = nullptr;
        };
    };
    PNGCodec(Core::IStream & stream,BitmapImage *res):ImgCodec(stream,res){};
    ~PNGCodec(){};
};

class TiffCodec : public ImgCodec {
    bool load_tiff_from_file(){
        TIFF *tiff = TIFFStreamOpen("r",&in);

        uint32_t width,height,bitDepth,channelCount,compression;
        
        ImgHeader header;

        TIFFGetField(tiff,TIFFTAG_IMAGEWIDTH,&width);
        TIFFGetField(tiff,TIFFTAG_IMAGELENGTH,&height);
        TIFFGetField(tiff,TIFFTAG_BITSPERSAMPLE,&bitDepth);
        TIFFGetField(tiff,TIFFTAG_SAMPLESPERPIXEL,&channelCount);
        TIFFGetField(tiff,TIFFTAG_COMPRESSION,&compression);
        
        header.color_format = BitmapImage::ColorFormat::RGBA;
        header.bitDepth = bitDepth;
        header.channels = channelCount;
        header.width = width;
        header.height = height;
        
        bool rc = false;

        size_t pixelCount = width * height;
        uint32_t * buffer = (uint32_t *)_TIFFmalloc(pixelCount * sizeof(uint32_t));
        if(buffer != nullptr){
            if(TIFFReadRGBAImage(tiff,width,height,buffer,0)){
                storage->data = buffer;
                storage->header =std::make_unique<ImgHeader>(std::move(header));
                storage->profile = nullptr;
                rc = true;
            };
//            _TIFFfree(buffer);
        };
        TIFFClose(tiff);
        return rc;
    };
public:
    void readToStorage(){
        if(!load_tiff_from_file()){
            storage->data = nullptr;
            storage->header = nullptr;
            storage->profile = nullptr;
        };
    };
    TiffCodec(Core::IStream & stream,BitmapImage *res):ImgCodec(stream,res){};
};


// class JPEGCodec : public ImgCodec {
//     bool load_jpeg_from_file(){
        
//     };
// public:
//     void readToStorage(){
        
//     };
//     JPEGCodec(Core::IStream & stream,BitmapImage *res):ImgCodec(stream,res){};
// };



Core::UniquePtr<ImgCodec> obtainCodecForImageFormat(BitmapImage::Format &format,Core::IStream &in,BitmapImage *img){
    switch (format) {
        case BitmapImage::PNG:
        {
            return std::make_unique<PNGCodec>(in,img);
            break;
        }
        case BitmapImage::TIFF:
        {
            
//            return std::make_unique<Core::TIFFCodec>(in,img);
            return nullptr;
            break;
        }
        case BitmapImage::JPEG:
        {
            return nullptr;
            break;
        }
        default:
            return nullptr;
            break;
    }
   
};
    
    Core::SharedPtr<BitmapImage> loadImageFromFile(FSPath path) {
        BitmapImage img;
        auto ext = path.ext();
        BitmapImage::Format f;
        auto os_corrected_path = path.serialize();
        if(ext == "png"){
            f = BitmapImage::PNG;
        };
        std::ifstream in(os_corrected_path,std::ios::binary);
        if(in.is_open()){
            Core::UniquePtr<ImgCodec> codec = obtainCodecForImageFormat(f,in,&img);
            codec->readToStorage();
            // in.close();
            // MessageBox(GetForegroundWindow(),"Img has Been Read","NOTE",MB_OK);
            return std::make_shared<BitmapImage>(std::move(img));
        }
        else
            return std::make_shared<BitmapImage>(BitmapImage({nullptr,nullptr,nullptr}));
    };

    Core::SharedPtr<BitmapImage> loadImageFromBuffer(void *bufferData,size_t bufferSize,BitmapImage::Format f){
        BitmapImage img;
        std::istringstream in(std::string((char *)bufferData,bufferSize),std::ios::binary);
        Core::UniquePtr<ImgCodec> codec = obtainCodecForImageFormat(f,in,&img);
        codec->readToStorage();
        return std::make_shared<BitmapImage>(std::move(img));
    };
};

