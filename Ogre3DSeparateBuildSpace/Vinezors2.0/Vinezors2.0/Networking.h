#ifndef __VinezorsiOS__Networking__
#define __VinezorsiOS__Networking__

#include <iostream>

/* 
 Configures values based on the dictionary object 
 keyed by configKey in JSON file pointed to by configPath
 */
bool configStageType(std::string configPath, std::string configKey);
bool configStageType(std::string configPath, std::string backupPath, std::string configKey);

/*
 Fetch Config files from server.
 Returns true on success.
 */
bool syncConfig (std::string configPath, std::string username);

/*
 Attempts to sync existing log files with server.
 Returns true if files are synced.
 */
bool syncLogs (std::string logPath, std::string username);


#endif /* defined(__VinezorsiOS__Networking__) */
