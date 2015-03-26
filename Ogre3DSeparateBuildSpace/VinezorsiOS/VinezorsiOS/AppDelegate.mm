#import "AppDelegate.h"

#import "SimpleMenuViewController.h"
#import "MainViewController.h"

#include "Util.h"

extern Util::ConfigGlobal globals;

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(UIApplication *)application
{
    NSLog(@"Launch");
    self.mWindow = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    
    globals.initGlobalSettingsPath();
    globals.loadGlobalSettings(globals.globalPath);
    
    if (!globals.sessionScreenEnabled)
    {
        self.mViewControllerMain = [[UIStoryboard storyboardWithName:@"MainView" bundle:nil]  instantiateViewControllerWithIdentifier:@"MainViewControllerStoryboard"];

        [self.mViewControllerMain startWithWindow:self.mWindow:[NSString stringWithUTF8String:globals.playerName.c_str()]];
        self.mWindow.rootViewController = self.mViewControllerMain;
        [self.mWindow makeKeyAndVisible];
    }
    else
    {
        // Main Menu
        self.mViewControllerMenu = [[UIStoryboard storyboardWithName:@"SimpleMenu" bundle:nil] instantiateViewControllerWithIdentifier:@"SimpleMenuViewControllerStoryboard"];
        self.mWindow.rootViewController = self.mViewControllerMenu;
        [self.mWindow makeKeyAndVisible];
    }
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    NSLog(@"Terminate");
    if (self.mViewControllerMain) [self.mViewControllerMain stop];
    [[UIApplication sharedApplication] performSelector:@selector(terminate:) withObject:nil afterDelay:0.0];
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    NSLog(@"Resign");
    if (self.mViewControllerMain) [self.mViewControllerMain inactivate];
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    NSLog(@"Active");
    if (self.mViewControllerMain) [self.mViewControllerMain activate];
}

- (NSUInteger)application:(UIApplication *)application supportedInterfaceOrientationsForWindow:(UIWindow *)window {
    NSUInteger mask = 0;
    return mask | UIInterfaceOrientationMaskLandscape;
}

/*
- (BOOL)application:(UIApplication *)application shouldSaveApplicationState:(NSCoder *)coder
{
    return YES;
}

- (BOOL)application:(UIApplication *)application shouldRestoreApplicationState:(NSCoder *)coder
{
    return YES;
}
 */

- (void)go:(NSString*)str
{
    [self.mViewControllerMenu release];
    
    // Run Ogre
    self.mViewControllerMain = [[UIStoryboard storyboardWithName:@"MainView" bundle:nil]  instantiateViewControllerWithIdentifier:@"MainViewControllerStoryboard"];
    //    self.mViewController = [[ViewController alloc] initWithNibName:@"ViewController" bundle:nil];
    [self.mViewControllerMain startWithWindow:self.mWindow:str];
    self.mWindow.rootViewController = self.mViewControllerMain;
    [self.mWindow makeKeyAndVisible];
}

@end