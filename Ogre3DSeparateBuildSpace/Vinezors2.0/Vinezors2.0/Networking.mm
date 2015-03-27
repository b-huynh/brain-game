//
//  Networking.mm
//  VinezorsiOS
//
//  Created by Brandon Huynh on 12/31/13.
//
//

#include "Networking.h"
#include "Util.h"
#include <sstream>

extern Util::ConfigGlobal globals;

bool configStageType(std::string configPath, std::string configKey)
{
    std::cerr << "Reading stage config from file: " << configPath << std::endl;
    
    NSError* error;
    NSString* nsConfigPath = [NSString stringWithUTF8String:configPath.c_str()];
    NSString* jsonString = [[NSString alloc] initWithContentsOfFile:nsConfigPath encoding:NSUTF8StringEncoding error:NULL];
    
    if (!jsonString) {
        std::cerr << "ERROR: Could not read contents of file" << std::endl;
        return false;
    }
    
    NSDictionary* json = [NSJSONSerialization
                          JSONObjectWithData:[jsonString dataUsingEncoding:NSUTF8StringEncoding]
                          options:kNilOptions
                          error:&error];
    
    if (!json) {
        std::cerr << "ERROR: Could not parse file with JSON" << std::endl;
        return false;
    }
    
    NSString* nsConfigKey = [NSString stringWithUTF8String:configKey.c_str()];
    NSDictionary* cgValues = [json objectForKey:nsConfigKey];
    
    if (!cgValues) {
        std::cerr << "ERROR: Config Object for " << configKey << " does not exist" << std::endl;
        return false;
    }
    
    for (NSString* key in [cgValues allKeys]) {
        NSString* value;
        if ([[cgValues objectForKey:key] isKindOfClass:[NSString class]])
            value = [cgValues objectForKey:key];
        else
            value = [[cgValues objectForKey:key] stringValue];
        std::istringstream iss ([value cStringUsingEncoding:NSUTF8StringEncoding]);
        globals.setConfigValue(iss, [key UTF8String]);
    }
    return true;
}

bool configStageType(std::string configPath, std::string backupPath, std::string configKey)
{
    if (!configStageType(configPath, configKey))
        return configStageType(backupPath, configKey);
    return true;
}

/*
 Fetch config file and construct file.
 If file is finished constructing, delete old file and rename new config file.
 */
bool syncConfig (std::string configPath, std::string username)
{
    std::string stdStringURL = "http://138.23.175.177:8080/config/" + username + "/config.json";
    NSString* url = [NSString stringWithUTF8String:stdStringURL.c_str()];
    NSData* data = [NSData dataWithContentsOfURL: [NSURL URLWithString:url] ];
    if (!data) {
        NSLog(@"WARNING: Could not retrieve config from %@", url);
        return false;
    }
    
    NSString* conf_path = [NSString stringWithUTF8String:configPath.c_str()];
    BOOL success = [data writeToFile:conf_path atomically:YES];
    
    return success ? true : false;
}

/*
 * Grabs new config file from lab server based on globals.playerName
 * Places it in globals.configPath on device
 */
bool syncConfig() {
    return syncConfig(globals.configPath, globals.playerName);
}

std::string getVendorID()
{
#if defined(OGRE_IS_IOS)
    NSString *userName;
    userName = [UIDevice currentDevice].identifierForVendor.UUIDString; // For IOS 6.0 & greater
    std::string cppString([userName cStringUsingEncoding:NSUTF8StringEncoding]);
    std::cout << "Networking: " << cppString << std::endl;
    return cppString;
#else
    return "none";
#endif
}

/*
 Attempts to upload log files from filePath to Lab Server under user folder
 */
bool uploadFile(std::string filePath, std::string user)
{
    //File bookkeeping
    NSString* _logPath = [NSString stringWithUTF8String:filePath.c_str()];
    NSString* str = [NSString stringWithContentsOfFile:_logPath encoding:NSUTF8StringEncoding error:nil];
    
    
    if (str == nil) {
        std::cerr << "ERROR: Could not read contents of file " << filePath << std::endl;
        return false;
    }
    
    // Use unique vendor ID to store log files in server
    
    NSString* fileName = [_logPath lastPathComponent];
#if defined(OGRE_IS_IOS)
    NSString *userName;
    userName = [UIDevice currentDevice].identifierForVendor.UUIDString; // For IOS 6.0 & greater
#else
    NSString* userName = [NSString stringWithUTF8String:user.c_str()];
#endif
    NSData* theData = [str dataUsingEncoding:NSUTF8StringEncoding];
    
    //Create URL request
    NSString* urlString = [NSString stringWithFormat: @"http://138.23.175.177:8080/log/%@", userName];
    NSMutableURLRequest *request = [[[NSMutableURLRequest alloc] init] autorelease];
    [request setURL:[NSURL URLWithString:urlString]];
    [request setHTTPMethod:@"POST"];
    
    //Fill body of POST
    NSString* boundary = @"---------------------------14737809831466499882746641449";
    NSString* contentType = [NSString stringWithFormat:@"multipart/form-data; boundary=%@",boundary];
    [request addValue:contentType forHTTPHeaderField: @"Content-Type"];
    
    NSMutableData *body = [NSMutableData data];
    
    [body appendData:[[NSString stringWithFormat:@"\r\n--%@\r\n",boundary]   dataUsingEncoding:NSUTF8StringEncoding]];
    [body appendData:
     [[NSString stringWithString: [NSString stringWithFormat: @"Content-Disposition: form-data; name=\"logfile\"; filename=\"%@\"\r\n", fileName]]dataUsingEncoding:NSUTF8StringEncoding]];
    [body appendData:[@"Content-Type: application/octet-stream\r\n\r\n" dataUsingEncoding:NSUTF8StringEncoding]];
    [body appendData:[NSData dataWithData:theData]];
    [body appendData:[[NSString stringWithFormat:@"\r\n--%@--\r\n",boundary] dataUsingEncoding:NSUTF8StringEncoding]];
    [request setHTTPBody:body];
    
    
    
    [NSURLConnection sendAsynchronousRequest:request
                                       queue:[[[NSOperationQueue alloc] init] autorelease]
                           completionHandler:^(NSURLResponse *response, NSData *data, NSError *connectionError) {
                               
                               if (data == nil || [((NSHTTPURLResponse*)response) statusCode] != 200) {
                                   NSLog(@"CONNECTION ERROR: Could not upload to %@", urlString);
                               }
                               
                               NSString* returnString = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
                               NSLog(@"RECEIVED: %@", returnString);
                           }];
    /*
    NSHTTPURLResponse* response = nil;
     
    NSData* returnData = [NSURLConnection sendSynchronousRequest:request returningResponse:&response error:nil];
    
    if (returnData == nil || [response statusCode] != 200) {
        NSLog(@"WARNING: Could not upload to %@", urlString);
        return false;
    }
    
    NSString* returnString = [[NSString alloc] initWithData:returnData encoding:NSUTF8StringEncoding];
    NSLog(@"RECEIVED: %@", returnString);
     */
    
    return true;
}

/*
 * Syncs all log files (those with extensions listed at logExtensions)
 * To users log folder in server based on globals.playerName
 */
bool syncLogs()
{
#if defined(OGRE_IS_IOS)
    NSString *userName;
    userName = [UIDevice currentDevice].identifierForVendor.UUIDString; // For IOS 6.0 & greater
#else
    NSString* userName = [NSString stringWithUTF8String:globals.playerName.c_str()];
#endif
    NSString* logpath = [NSString stringWithUTF8String:globals.logPath.c_str()];
    logpath = [logpath stringByDeletingLastPathComponent];
    NSArray* logExtensions = @[@"log", @"act", @"session"];
    
    //Get list of files that are already on the server
    NSString* liststring = [NSString stringWithFormat:@"http://138.23.175.177:8080/listlogs/%@",userName];
    NSURL* listurl = [NSURL URLWithString:liststring];
    NSURLRequest* request = [NSURLRequest requestWithURL:listurl];
    
    [NSURLConnection sendAsynchronousRequest:request
                                    queue:[[[NSOperationQueue alloc] init] autorelease]
                        completionHandler:^(NSURLResponse *response, NSData *data, NSError *connectionError) {
                               
                            if ([data length] <= 0 || connectionError != nil) {
                                NSLog(@"CONNECTION ERROR: Could not get loglist from server");
                                return;
                            }
                            
                            NSError* error;
                            NSDictionary* json = [NSJSONSerialization
                                                  JSONObjectWithData:data
                                                  options:kNilOptions
                                                  error:&connectionError];
                            NSArray* filesOnServer = [json objectForKey:@"files"];
                            
                            //Enumerate the directory
                            NSDirectoryEnumerator* enumerator = [[NSFileManager defaultManager] enumeratorAtPath:logpath];
                            if (enumerator == nil) {
                                NSLog(@"CONNECTION ERROR: Subject folder probably doesn't exist");
                                return;
                            }
                            
                            while(NSString* file = [enumerator nextObject]) {
                                NSString* filePath = [NSString stringWithFormat:@"%@/%@",logpath,file];
                                NSString* fileExtension = [filePath pathExtension];
                                
                                //Check if it's a file or directory.
                                BOOL isDir = NO;
                                [[NSFileManager defaultManager] fileExistsAtPath:filePath isDirectory:&isDir];
                                
                                if (!isDir && //Not a directory
                                    ![filesOnServer containsObject:file] && //Not on server already
                                    [logExtensions containsObject:fileExtension]) //Is correct file extension
                                {
                                    if (!uploadFile(std::string([filePath UTF8String]), globals.playerName)) {
                                        NSLog(@"CONNECTION ERROR: Could not upload file %@", filePath);
                                    }
                                }
                            }
                        }];
    
    /*
    NSData* response = [NSURLConnection sendSynchronousRequest:request returningResponse:nil error:nil];
    
    if (!response) {
        NSLog(@"WARNING: Could not get loglist from server");
        return false;
    }
    
    NSError* error;
    NSDictionary* json = [NSJSONSerialization
                          JSONObjectWithData:response
                          options:kNilOptions
                          error:&error];
    NSArray* filesOnServer = [json objectForKey:@"files"];
    
    //Enumerate the directory
    NSDirectoryEnumerator* enumerator = [[NSFileManager defaultManager] enumeratorAtPath:logpath];
    if (enumerator == nil) {
        NSLog(@"ERROR: Subject folder probably doesn't exist");
        return false;
    }
    
    while(NSString* file = [enumerator nextObject]) {
        NSString* filePath = [NSString stringWithFormat:@"%@/%@",logpath,file];
        NSString* fileExtension = [filePath pathExtension];
        
        //Check if it's a file or directory.
        BOOL isDir = NO;
        [[NSFileManager defaultManager] fileExistsAtPath:filePath isDirectory:&isDir];
        
        if (!isDir && //Not a directory
            ![filesOnServer containsObject:file] && //Not on server already
            [logExtensions containsObject:fileExtension]) //Is correct file extension
        {
            if (!uploadFile(std::string([filePath UTF8String]), globals.playerName)) {
                NSLog(@"WARNING: Could not upload file %@", filePath);
                return false;
            }
        }
    }
     */
    
    return true;
}
