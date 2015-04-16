#ifndef __AppDelegate_H__
#define __AppDelegate_H__

#ifdef __OBJC__

#import <UIKit/UIKit.h>

@class SimpleMenuViewController;
@class MainViewController;

@interface AppDelegate : NSObject <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow* mWindow;
@property (strong, nonatomic) SimpleMenuViewController* mViewControllerMenu;
@property (strong, nonatomic) MainViewController* mViewControllerMain;

- (void)go:(NSString*)str ;

@end

#endif
#endif
