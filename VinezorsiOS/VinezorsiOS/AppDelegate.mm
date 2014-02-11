#import "AppDelegate.h"

#import "SimpleMenuViewController.h"
#import "MainViewController.h"

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(UIApplication *)application
{
    self.mWindow = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];

    // Main Menu
    self.mViewControllerMenu = [[UIStoryboard storyboardWithName:@"SimpleMenu" bundle:nil] instantiateViewControllerWithIdentifier:@"SimpleMenuViewControllerStoryboard"];
    self.mWindow.rootViewController = self.mViewControllerMenu;
    [self.mWindow makeKeyAndVisible];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    [self.mViewControllerMain stop];
    
    [[UIApplication sharedApplication] performSelector:@selector(terminate:) withObject:nil afterDelay:0.0];
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
}

- (void)go:(NSString*)str :(BOOL)isOn
{
    [self.mViewControllerMenu release];
    
    // Run Ogre
    self.mViewControllerMain = [[UIStoryboard storyboardWithName:@"MainView" bundle:nil]  instantiateViewControllerWithIdentifier:@"MainViewControllerStoryboard"];
//    self.mViewController = [[ViewController alloc] initWithNibName:@"ViewController" bundle:nil];
    self.mWindow.rootViewController = self.mViewControllerMain;
    [self.mViewControllerMain startWithWindow:self.mWindow:str:isOn];
    [self.mWindow makeKeyAndVisible];
}

@end