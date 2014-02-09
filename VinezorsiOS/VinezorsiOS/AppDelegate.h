#ifndef __AppDelegate_H__
#define __AppDelegate_H__

#ifdef __OBJC__

#import <UIKit/UIKit.h>

@class ViewController;

@interface AppDelegate : NSObject <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow* mWindow;
@property (strong, nonatomic) ViewController* mViewController;

- (void)go:(NSString*)str :(BOOL)isOn;

@end

#endif
#endif
