#include "omegaWTK/Composition/Bitmap.h"

#include <fstream>
#include <iostream>
#include <png.h>


namespace OmegaWTK::Composition {

namespace PNG {
    bool validate_signature(std::ifstream & in){
        #define SIG_SIZE 8

        png_byte sig[SIG_SIZE];
        
        if(in.is_open()){
            in.read((char *)sig,8);
            if(!in.good()){
                return false;
            };
            int is_png = 0;
            is_png = png_sig_cmp(sig,0,SIG_SIZE);
            if(is_png == 0){
                return true;
            }
            else {
                return false;
            };
        }
        else
            return false;
    };
    void userReadData(png_structp png_ptr, png_bytep data, png_size_t length){
        png_voidp stream = png_get_io_ptr(png_ptr);
        ((std::ifstream *)stream)->read((char *)data,length);
    };

    bool load_png_from_file(const Core::String & name,BitmapImage *img_res){
        std::ifstream in(name,std::ios::in);
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

            png_set_sig_bytes(png_ptr, 8);

            png_read_info(png_ptr,info_ptr);

            /// Set Info!
            
            auto width = png_get_image_width(png_ptr,info_ptr);
            auto height = png_get_image_height(png_ptr,info_ptr);

            auto bitDepth = png_get_bit_depth(png_ptr,info_ptr);
            auto channels = png_get_channels(png_ptr,info_ptr);
            auto colorTy = png_get_color_type(png_ptr,info_ptr);
            
            rowPtrs = new png_bytep[height];
            data = new char[width * height * bitDepth * channels / 8];
            unsigned int stride = width * bitDepth * channels / 8;
            
            for(auto i = 0;i < height;i++){
                png_uint_32 ptr = (height - i - 1) * stride;
                rowPtrs[i] = (png_bytep)data + ptr;
            };
            
            png_read_image(png_ptr,rowPtrs);
            
            png_read_end(png_ptr,info_ptr);
            
            delete [] rowPtrs;
            png_destroy_read_struct(&png_ptr, &info_ptr,(png_infopp)0);
            
            img_res->data = data;
            img_res->stride = stride;
            img_res->width = width;
            img_res->height = height;
            
            return true;

        }
        else 
            return false;
    };
};

BitmapImage loadImageFromFile(const Core::String & name) {
    BitmapImage img;
    if(PNG::load_png_from_file(name,&img)){
        return img;
    }
    else {
        return {};
    };
};

}
