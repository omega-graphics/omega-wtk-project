#include <iostream>
#include <fstream>
#include "assetc.h"
#include <string>
#include <vector>

using namespace OmegaWTK::assetc;

typedef unsigned char Byte;

int main(int argc,char * const argv[]){

    std::string val;
    std::string output_file;
    std::vector<std::string> asset_files;

    for(unsigned i = 1;i < argc;i++){
        val = argv[i];
        if(val == "--output"){
            output_file = argv[++i];
        }
        else {
            asset_files.push_back(argv[i]);
        }
    };

    if(output_file.empty() && asset_files.empty()){
        std::cerr << "No input files. Exiting..." << std::endl;
        return 1;
    }
    else {
        std::ofstream out(output_file,std::ofstream::binary);
        AssetsFileHeader header;
        header.asset_count = asset_files.size();
        out.write((const char *)&header,sizeof(AssetsFileHeader));

        auto file_it = asset_files.begin();
        while(file_it != asset_files.end()){
            std::string & file = *file_it;
            std::ifstream asset_in(file,std::ifstream::binary);
            if(asset_in.is_open()){
                asset_in.seekg(0,asset_in.end);
                size_t filesize = asset_in.tellg();
                asset_in.seekg(0,asset_in.beg);

                Byte *ptr = new Byte[filesize];
                asset_in.read((char *)ptr,filesize);
                asset_in.close();

                AssetsFileEntry ent;
                ent.string_size = file.size();
                ent.file_size = filesize;
                out.write((const char *)&ent,sizeof(AssetsFileEntry));
                /// Write the Filename
                out.write(file.c_str(),file.size());
                /// Write the Asset Data
                out.write((char *)ptr,filesize);
            }
            else {
                std::cerr << "ERROR: Cannot Open File: " << file << "\nExiting..." << std::endl;
                return 1;
            }

            ++file_it;
        };
        out.close();
        std::cout << "Successfully Compiled Assets to: " << output_file << std::endl;
    };
    
    return 0;
};