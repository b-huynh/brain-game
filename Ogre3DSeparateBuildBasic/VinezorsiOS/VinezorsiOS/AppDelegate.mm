#import "AppDelegate.h"

#import "SimpleMenuViewController.h"
#import "MainViewController.h"

//#define AUTO_START 1

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(UIApplication *)application
{
    self.mWindow = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];

#ifdef AUTO_START
    self.mViewControllerMain = [[UIStoryboard storyboardWithName:@"MainView" bundle:nil]  instantiateViewControllerWithIdentifier:@"MainViewControllerStoryboard"];
    //    self.mViewController = [[ViewController alloc] initWithNibName:@"ViewController" bundle:nil];
    [self.mViewControllerMain startWithWindow:self.mWindow:@"subject100"];
    self.mWindow.rootViewController = self.mViewControllerMain;
    [self.mWindow makeKeyAndVisible];
#else
    // Main Menu
    self.mViewControllerMenu = [[UIStoryboard storyboardWithName:@"SimpleMenu" bundle:nil] instantiateViewControllerWithIdentifier:@"SimpleMenuViewControllerStoryboard"];
    self.mWindow.rootViewController = self.mViewControllerMenu;
    [self.mWindow makeKeyAndVisible];
#endif
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    if (self.mViewControllerMain) [self.mViewControllerMain stop];
    
    [[UIApplication sharedApplication] performSelector:@selector(terminate:) withObject:nil afterDelay:0.0];
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    //[[NSNotificationCenter defaultCenter] removeObserver:self];
    if (self.mViewControllerMain) [self.mViewControllerMain inactivate];
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    if (self.mViewControllerMain) [self.mViewControllerMain activate];
}

- (NSUInteger)application:(UIApplication *)application supportedInterfaceOrientationsForWindow:(UIWindow *)window {
    return UIInterfaceOrientationMaskAllButUpsideDown;
}

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