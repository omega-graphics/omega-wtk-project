#include <iostream>
#include <fstream>
#include "assetc.h"
#include <string>
#include <vector>
#include <iomanip>

// #include <openssl/evp.h>

using namespace OmegaWTK::assetc;

typedef unsigned char Byte;

#define PROG_NAME "omegawtk-assetc"

void logHelp(){
    std::cout << 
    "OmegaWTK App Asset Compiler/Signer\n\n"

    "Usage:\n"
    "omegawtk-assetc --application-id [app-id] --output [output-file] inputs...\n";
};

typedef std::vector<unsigned char> UCharVector;

UCharVector fromString(std::string & str){
    UCharVector vec(str.size());
    std::copy(str.begin(),str.end(),vec.begin());
    return vec;
};

std::string fromUCharVector(UCharVector & vec){
    return std::string((const char *)vec.data(),vec.size());
};

void printHashAsHex(UCharVector &bytes){
    std::cout << "Hash:" << std::flush;
    for(unsigned i = 0;i < bytes.size();i++){
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)bytes[i];
        if(i != bytes.size() - 1)
            std::cout << ":";
    };
    std::cout << std::endl;
};

int main(int argc,char * const argv[]){

    std::string val;
    std::string output_file, app_id;
    std::vector<std::string> asset_files;


    for(unsigned i = 1;i < argc;i++){
        val = argv[i];
        if(val == "--help" || val == "-h"){
            logHelp();
            return 0;
            break;
        }
        else if(val == "--application-id"){
            app_id = argv[++i];
        }
        else if(val == "--output"){
            output_file = argv[++i];
        }
        else {
            asset_files.push_back(argv[i]);
        }
    };

    if((output_file.empty() && asset_files.empty()) || app_id.empty()){
        std::cerr << PROG_NAME << ": No input files. Exiting..." << std::endl;
        return 1;
    }
    else {
        /// Hash the AppID with Sha256
        // UCharVector charVector0, charVector;
        // charVector0.resize(EVP_MD_size(EVP_sha256()));
        // charVector.resize(charVector0.size());
        // unsigned outlen;
        // int outlen2,outlen3;

        // auto appIDChars = fromString(app_id);
        // EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();
        // EVP_DigestInit(md_ctx,EVP_sha256());
        // EVP_DigestUpdate(md_ctx,appIDChars.data(),appIDChars.size());
        // EVP_DigestFinal(md_ctx,charVector0.data(),&outlen);
        // EVP_MD_CTX_free(md_ctx);

        // /// Encode the Hash with AES-128-CBC
        // UCharVector key ={10,9,8,1,42,6,19,20,51,59,1,23,0,15,69,28,29,20,10,50,45,102};
        // UCharVector iv = {0,10,9,6,15};

        // EVP_CIPHER_CTX *cph_ctx = EVP_CIPHER_CTX_new();
        // EVP_EncryptInit(cph_ctx,EVP_aes_128_cbc(),key.data(),iv.data());
        // EVP_EncryptUpdate(cph_ctx,charVector.data(),&outlen2,charVector0.data(),charVector0.size());
        // charVector.resize(outlen2);
        // EVP_EncryptFinal(cph_ctx,charVector.data(),&outlen3);
        // EVP_CIPHER_CTX_free(cph_ctx);
        // // std::cout << "SIG:" << fromUCharVector(charVector) << std::endl;
        // std::ofstream out_sig(output_file + ".sig",std::ofstream::binary);
        // out_sig.write((const char *)charVector.data(),charVector.size());
        // out_sig.close();

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
                std::cerr << PROG_NAME <<  ": error: Cannot Open File: " << file << "\nExiting..." << std::endl;
                return 1;
            }

            ++file_it;
        };
        out.close();
        // printHashAsHex(charVector0);
        std::cout << PROG_NAME <<  ":" << " Successfully Compiled Assets to: " << output_file << std::endl;
    };
    
    return 0;
};