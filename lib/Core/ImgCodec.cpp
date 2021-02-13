#include "omegaWTK/Core/ImgCodec.h"

#include <zlib.h>

#include <png.h>

#include <tiff.h>
#include <tiffio.h>


#include <iostream>
#include <fstream>

namespace OmegaWTK::Core {

    class ImgCodec {
    protected:
        const Core::String & file;
        BitmapImage *storage;
    public:
        virtual void readToStorage();
        ImgCodec(const Core::String & f,BitmapImage *res):file(f),storage(res){};
    };


    
class PNGCodec : public ImgCodec {
static void userReadData(png_structp png_ptr, png_bytep data, png_size_t length){
    png_voidp stream = png_get_io_ptr(png_ptr);
    ((std::ifstream *)stream)->read((char *)data,length);
 };
#define SIG_SIZE 8
bool validate_signature(std::ifstream & in){


   png_byte sig[SIG_SIZE];
   
   in.read((char *)sig,SIG_SIZE);
   if(!in.good()){
       return false;
   };
   
   return png_check_sig(sig,SIG_SIZE);
};


bool load_png_from_file(){
   std::ifstream in(file,std::ios::binary);
   if(in.is_open()) {
       if(validate_signature(in)){
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

           /// Set Info!
           
           auto width = png_get_image_width(png_ptr,info_ptr);
           auto height = png_get_image_height(png_ptr,info_ptr);

           auto bitDepth = png_get_bit_depth(png_ptr,info_ptr);
           auto channels = png_get_channels(png_ptr,info_ptr);
           // auto colorTy = png_get_color_type(png_ptr,info_ptr);
           
           rowPtrs = new png_bytep[height];
           data = new char[width * height * bitDepth * channels / 8];
           unsigned int stride = width * bitDepth * channels / 8;
           
           for(size_t i = 0;i < height;i++){
               png_uint_32 ptr = (height - i - 1) * stride;
               rowPtrs[i] = (png_bytep)data + ptr;
           };
           
           png_read_image(png_ptr,rowPtrs);
           
           png_read_end(png_ptr,info_ptr);
           
           delete [] rowPtrs;
           png_destroy_read_struct(&png_ptr, &info_ptr,(png_infopp)0);
           
           storage->data = data;
           storage->stride = stride;
           storage->width = width;
           storage->height = height;
           storage->bitDepth = bitDepth;
           storage->channelCount = channels;
           
           return true;

       }
       else {
//                MessageBox(GetForegroundWindow(),"Invalid Signature!","NOTE",MB_OK);
           return false;
       }
   }
   else {
      // MessageBox(GetForegroundWindow(),(std::string("Cannot Find File:") + name).c_str(),"NOTE",MB_OK);
       return false;
   };
};
public:
    void readToStorage()  {
        if(!load_png_from_file()){
            storage->height = 0;
            storage->bitDepth = 0;
            storage->data = nullptr;
            storage->stride = 0;
            storage->width = 0;
        };
    };
    PNGCodec(const Core::String & name,BitmapImage *res):ImgCodec(name,res){};
};

class TiffCodec : public ImgCodec {
    bool load_tiff_from_file(){
        TIFF *tiff = TIFFOpen(file.c_str(),"r");
        
        uint32_t width,height,bitDepth,channelCount;
        
        
        TIFFGetField(tiff,TIFFTAG_IMAGEWIDTH,&width);
        TIFFGetField(tiff,TIFFTAG_IMAGELENGTH,&height);
        TIFFGetField(tiff,TIFFTAG_BITSPERSAMPLE,&bitDepth);
        TIFFGetField(tiff,TIFFTAG_SAMPLESPERPIXEL,&channelCount);
        
        size_t pixelCount = width * height;
        uint32_t * buffer = (uint32_t *)_TIFFmalloc(pixelCount * sizeof(uint32_t));
        if(buffer != nullptr){
            if(TIFFReadRGBAImage(tiff,width,height,buffer,0)){
                
            };
            _TIFFfree(buffer);
        };
        TIFFClose(tiff);
        
    };
public:
    void readToStorage(){
        if(!load_tiff_from_file()){
            storage->height = 0;
            storage->bitDepth = 0;
            storage->data = nullptr;
            storage->stride = 0;
            storage->width = 0;
        };
    };
    TiffCodec(const Core::String & name,BitmapImage *res):ImgCodec(name,res){};
};

ImgCodec obtainCodecForImageFormat(FSPath & path,BitmapImage *img){
    if(path.dir() == "png"){
        return PNGCodec(path.serialize(),img);
    }
    else if(path.dir() == "tiff"){
        return TiffCodec(path.serialize(),img);
    };
};
    
    BitmapImage loadImageFromFile(FSPath path) {
        BitmapImage img;
        auto codec = obtainCodecForImageFormat(path,&img);
        codec.readToStorage();
        return img;

    };
};

