//
//  macPath.h
//  VinezorsiOS
//
//  Created by Calvin Phung on 10/21/14.
//
//

#ifndef VinezorsiOS_macPath_h
#define VinezorsiOS_macPath_h

#include <string>

// Returns the path of the associated Documents folder of the installed app
//
// This is made in response to iOS 8.0 since the application bundle is moved to
// a different location separate from its associated Documents folder.
std::string applicationDocumentsPath();

#endif
