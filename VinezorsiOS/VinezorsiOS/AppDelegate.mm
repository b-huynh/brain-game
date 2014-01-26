#import "AppDelegate.h"

#import "ViewController.h"

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(UIApplication *)application
{
    self.mWindow = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    
    // Override point for customization after application launch.
    self.mViewController = [[ViewController alloc] initWithNibName:@"ViewController" bundle:nil];
    
    self.mWindow.rootViewController = self.mViewController;
    
    [self.mViewController startWithWindow:self.mWindow];
    
    [self.mWindow makeKeyAndVisible];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    [self.mViewController stop];
    
    [[UIApplication sharedApplication] performSelector:@selector(terminate:) withObject:nil afterDelay:0.0];
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
}

@end