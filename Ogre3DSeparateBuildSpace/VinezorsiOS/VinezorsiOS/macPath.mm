//
//  macPath.m
//  VinezorsiOS
//
//  Created by Calvin Phung on 10/21/14.
//
//

#import <macPath.h>

// Returns the path of the associated Documents folder of the installed app
std::string applicationDocumentsPath()
{
    NSURL* url = [[[NSFileManager defaultManager]
                   URLsForDirectory:NSDocumentDirectory
                   inDomains:NSUserDomainMask] lastObject];
    //NSString* str = [url absoluteString];
    NSString* str = [NSString stringWithFormat:
                     @"%@", url.path];
    
    return [str UTF8String];
}