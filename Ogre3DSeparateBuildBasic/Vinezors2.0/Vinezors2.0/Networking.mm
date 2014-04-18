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
        if ([key isEqualToString:@"scheduleMain"] || [key isEqualToString:@"scheduleRepeat"] || [key isEqualToString:@"scheduleRepeatRandomPool"] || [key isEqualToString:@"playerName"])
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
    std::string stdStringURL = "http://braingame.brandonhuynh.name:3000/configs/" + username + "/conf.json";
    std::cout << stdStringURL << std::endl;
    NSString* url = [NSString stringWithUTF8String:stdStringURL.c_str()];
    
    NSData* data = [NSData dataWithContentsOfURL: [NSURL URLWithString:url] ];
    //NSLog(@"%@", data);
    if (!data) return false;
    
    NSString* conf_path = [NSString stringWithUTF8String:configPath.c_str()];
    std::cerr << "Conf Path: " << configPath << std::endl;
    
    BOOL success = [data writeToFile:conf_path atomically:YES];
    
    return success ? true : false;
}


