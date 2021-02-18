#import "FSCocoa.h"

#import <Foundation/Foundation.h>

const char * get_asset_dir(){
    
    NSString * str = [NSBundle mainBundle].resourcePath;
    char *buffer = const_cast<char *>(str.UTF8String);
//    buffer[str.length] = '\0';
    return buffer;
    
};
