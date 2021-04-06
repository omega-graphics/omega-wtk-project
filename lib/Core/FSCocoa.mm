#import "FSCocoa.h"

#import <Foundation/Foundation.h>

bool file_exists(const char *str){
    NSString *path = [NSString stringWithCString:str encoding:NSUTF8StringEncoding];
    BOOL exists = [[NSFileManager defaultManager] fileExistsAtPath:path];
    return exists == YES;
};

const char * get_asset_dir(){
    
    NSString * str = [NSBundle mainBundle].resourcePath;
    auto buffer = str.UTF8String;
    return buffer;
};
