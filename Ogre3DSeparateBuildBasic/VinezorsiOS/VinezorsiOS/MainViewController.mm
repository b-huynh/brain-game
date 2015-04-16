//
//  MainViewController.mm
//  VinezorsiOS
//
//  Created by Calvin Phung on 1/14/14.
//
//

#import "MainViewController.h"

#include "OgreApp.h"
#import "OgreView.h"

#import <QuartzCore/QuartzCore.h>
#import <CoreMotion/CoreMotion.h>

// To use CADisplayLink for smoother animation on iPhone comment out
// the following line or define it to 1.  Use with caution, it can
// sometimes cause input lag.
#define USE_CADISPLAYLINK 0

// private category
@interface MainViewController ()
{
    NSTimer* mTimer;
    OgreApp* mApplication;
    
    // Use of the CADisplayLink class is the preferred method for controlling your animation timing.
    // CADisplayLink will link to the main display and fire every vsync when added to a given run-loop.
    // The NSTimer class is used only as fallback when running on a pre 3.1 device where CADisplayLink
    // isn't available.
    id mDisplayLink;
    NSDate* mDate;
    float mLastFrameTime;
    float mStartTime;
    float mDeltaTime;
    BOOL mDisplayLinkSupported;
    
    CGFloat initialThreshold;
    CGFloat swipeThreshold;
    CGPoint startP;
    CGFloat totalX;
    CGFloat totalY;
    CGFloat vmaxX;
    CGFloat vmaxY;
    int swipeState;
}

@property (retain) NSTimer *mTimer;
@property (retain) OgreView* mOgreView;

@end

@implementation MainViewController

@synthesize mTimer;
@synthesize mOgreView;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    
    // Custom Tap Gesture Recognizers are implemented here since one requires to be overshadowed by
    // the other
    UITapGestureRecognizer* singleTapRecognizer = [[[UITapGestureRecognizer alloc] initWithTarget: self action:@selector(handleSingleTap:)] autorelease];
    singleTapRecognizer.numberOfTapsRequired = 1;
    [self.view addGestureRecognizer:singleTapRecognizer];
    
    UITapGestureRecognizer *doubleTapRecognizer = [[[UITapGestureRecognizer alloc] initWithTarget: self action:@selector(handleDoubleTap:)] autorelease];
    doubleTapRecognizer.numberOfTapsRequired = 2;
    [self.view addGestureRecognizer:doubleTapRecognizer];
    
    UIPanGestureRecognizer *panRecognizer = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(handlePan:)];
    [self.view addGestureRecognizer:panRecognizer];
    
    UIPinchGestureRecognizer *pinchRecognizer = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(handlePinch:)];
    [self.view addGestureRecognizer:pinchRecognizer];
    
    UILongPressGestureRecognizer *longPressRecognizer = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(handleLongPress:)];
    longPressRecognizer.minimumPressDuration = 0.20;
    [self.view addGestureRecognizer:longPressRecognizer];
    
    //[singleTapRecognizer requireGestureRecognizerToFail:doubleTapRecognizer];
    
    singleTapRecognizer.delegate = self;
    doubleTapRecognizer.delegate = self;
    panRecognizer.delegate = self;
    pinchRecognizer.delegate = self;
    longPressRecognizer.delegate = self;
    
    //initialThreshold = 12.50;
    //swipeThreshold = 30.00;
    initialThreshold = 15.00;
    swipeThreshold = 30.00;
    swipeState = 0;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)startWithWindow:(UIWindow*)window:(NSString*)str
{
    unsigned int width  = self.view.frame.size.width;
    unsigned int height = self.view.frame.size.height;
    
    mOgreView = [[OgreView alloc] initWithFrame:CGRectMake(0,0,width,height)];
    
    // A system version of 3.1 or greater is required to use CADisplayLink. The NSTimer
    // class is used as fallback when it isn't available.
#if USE_CADISPLAYLINK
    NSString *reqSysVer = @"3.1";
    NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
    if ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending)
        mDisplayLinkSupported = TRUE;
#endif
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    mLastFrameTime = 1;
    mDeltaTime = 1.0f / 30.0f;
    mStartTime = 0;
    mTimer = nil;
    
    try
    {
        mApplication = new OgreApp();
        mApplication->startDemo(window, mOgreView, width, height, [str UTF8String]);
        
        Ogre::Root::getSingleton().getRenderSystem()->_initRenderTargets();
        // Clear event times
		Ogre::Root::getSingleton().clearEventTimes();
    }
    catch( Ogre::Exception& e )
    {
        std::cerr << "An exception has occurred: " <<
        e.getFullDescription().c_str() << std::endl;
    }
    
    // Linking the ogre view to the render window
    mOgreView.mRenderWindow = OgreFramework::getSingletonPtr()->m_pRenderWnd;
    
    // Ogre has created an EAGL2ViewController for the provided mOgreView
    // and assigned it as the root view controller of the window
    //
    // Let's first retrieve it
    UIViewController* ogreViewController = window.rootViewController;
    NSAssert(ogreViewController.view == mOgreView, @"Ogre's created view controller owns the given view.");
    NSAssert(ogreViewController.interfaceOrientation == self.interfaceOrientation, @"Ogre's view controller has the same device orientation");
    
    // I want to be the actual root view controller
    window.rootViewController = self;
    
    // but i want to add a child link with the ogre one
    [self addChildViewController:ogreViewController];
    
    // add the ogre view as a sub view and add constraints for them to match
    [self.view addSubview:mOgreView];
    [self.view sendSubviewToBack:mOgreView];
    mOgreView.translatesAutoresizingMaskIntoConstraints = NO;
    NSDictionary *views = NSDictionaryOfVariableBindings(mOgreView);
    [self.view addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|[mOgreView]|"
                                                                      options:0
                                                                      metrics:nil
                                                                        views:views]];
    [self.view addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|[mOgreView]|"
                                                                      options:0
                                                                      metrics:nil
                                                                        views:views]];
    [self.view layoutIfNeeded];
    
    if (mDisplayLinkSupported)
    {
        // CADisplayLink is API new to iPhone SDK 3.1. Compiling against earlier versions will result in a warning, but can be dismissed
        // if the system version runtime check for CADisplayLink exists in -initWithCoder:. The runtime check ensures this code will
        // not be called in system versions earlier than 3.1.
        mDate = [[NSDate alloc] init];
        mLastFrameTime = -[mDate timeIntervalSinceNow];
        
        mDisplayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(update:)];
        [mDisplayLink setFrameInterval:(mDeltaTime) * mLastFrameTime];
        [mDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    }
    else
    {
        mTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)(mDeltaTime) * mLastFrameTime
                                                  target:self
                                                selector:@selector(update:)
                                                userInfo:nil
                                                 repeats:YES];
    }
    
    [pool release];
}

- (void)stop
{
    [self cleanup];
}

- (void)inactivate
{
    OgreFramework::getSingletonPtr()->m_pRenderWnd->setActive(false);
    //Ogre::Root::getSingleton().saveConfig();
}

- (void)activate
{
    OgreFramework::getSingletonPtr()->m_pRenderWnd->setActive(true);
}

- (void)update:(id)sender
{
    if(!OgreFramework::getSingletonPtr()->m_pRenderWnd->isActive())
        return;
    
    if(!OgreFramework::getSingletonPtr()->isOgreToBeShutDown() &&
       Ogre::Root::getSingletonPtr() && Ogre::Root::getSingleton().isInitialised())
    {
		if(OgreFramework::getSingletonPtr()->m_pRenderWnd->isActive())
		{
			mStartTime = OgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU();
            
            mApplication->update(mDeltaTime);
            OgreFramework::getSingletonPtr()->updateOgre(mDeltaTime);
            
			OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();
            
			mLastFrameTime = OgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU() - mStartTime;
		}
        if (mOgreView.resized)
        {
            mApplication->requestResize();
        }
    }
    else
    {
        [self cleanup];
        [[UIApplication sharedApplication] performSelector:@selector(terminate:) withObject:nil afterDelay:0.0];
    }
}

/*
- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
    UITouch *touch = [[event allTouches] anyObject];
    start = [touch locationInView:self.view];
    
    mApplication->activateTouchPress(start.x, start.y);
}

- (void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event
{
    UITouch *touch = [[event allTouches] anyObject];
    end = [touch locationInView:self.view];
    
    mApplication->activateTouchRelease(end.x, end.y);
}

- (void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event
{
    UITouch *touch = [[event allTouches] anyObject];
    CGPoint loc = [touch locationInView:self.view];
    
    mApplication->activateTouchMove(loc.x - end.x, loc.y - end.y);
    
    end = loc;
}

- (IBAction)handleSwipeLeft:(UISwipeGestureRecognizer*)sender
{
    //mApplication->activatePerformLeftMove();
}

- (IBAction)handleSwipeRight:(UISwipeGestureRecognizer*)sender
{
    //mApplication->activatePerformRightMove();
}

- (IBAction)handleSwipeDown:(UISwipeGestureRecognizer*)sender
{
    CGFloat dx = end.x - start.x;
    mApplication->activateTouchRelease(dx, end.y - start.y);
    if (dx < -1.0)
        mApplication->activatePerformLeftMove();
    else if (dx > 1.0)
        mApplication->activatePerformRightMove();
}

- (IBAction)handleSwipeUp:(UISwipeGestureRecognizer*)sender
{
    CGFloat dx = end.x - start.x;
    mApplication->activateTouchRelease(dx, end.y - start.y);
    if (dx < -1.0)
        mApplication->activatePerformLeftMove();
    else if (dx > 1.0)
        mApplication->activatePerformRightMove();
}
*/

- (IBAction)handleDoubleTap:(UITapGestureRecognizer*)sender
{
    CGPoint loc = [sender locationInView:self.view];
    if (sender.state == UIGestureRecognizerStateEnded)
        mApplication->activatePerformDoubleTap(loc.x, loc.y);
}

- (IBAction)handleSingleTap:(UITapGestureRecognizer*)sender
{
    CGPoint loc = [sender locationInView:self.view];
    if (sender.state == UIGestureRecognizerStateEnded)
        mApplication->activatePerformSingleTap(loc.x, loc.y);
}

- (IBAction)handlePinch:(UIPinchGestureRecognizer*)sender
{
    if (sender.state == UIGestureRecognizerStateEnded)
        mApplication->activatePerformPinch();
}

- (IBAction)handlePan:(UIPanGestureRecognizer*)sender
{
    CGPoint p = [sender locationInView:sender.view];
    CGPoint dp = [sender translationInView:sender.view];
    CGPoint v = [sender velocityInView:sender.view];
    
    if (sender.state == UIGestureRecognizerStateBegan)
    {
        startP = p;
        totalX = 0.0;
        totalY = 0.0;
        vmaxX = 0.0;
        vmaxY = 0.0;
        swipeState = 0;
    }
    else if (sender.state == UIGestureRecognizerStateChanged)
    {
        CGFloat dx = dp.x - totalX;
        CGFloat dy = dp.y - totalY;
        
        if (abs(vmaxX) < abs(v.x))
            vmaxX = v.x;
        if (abs(vmaxY) < abs(v.y))
            vmaxY = v.y;
        
        NSLog(@"%f %f", dx, v.x);
        
        if ((totalX == 0.0 && dx < -initialThreshold && vmaxX < -initialThreshold) || (dx < vmaxX / 5 && vmaxX < -swipeThreshold))
            //if ((totalX == 0.0 && dx < -initialThreshold && v.x < -initialThreshold) || (dx < v.x && v.x < -swipeThreshold))
        {
            mApplication->activatePerformLeftMove();
            totalX = dp.x;
        }
        if ((totalX == 0.0 && dx > initialThreshold && vmaxX > initialThreshold) || (dx > vmaxX / 5 && vmaxX > swipeThreshold))
            //if ((totalX == 0.0 && dx > initialThreshold && v.x > initialThreshold) || (dx > v.x && v.x > swipeThreshold))
        {
            mApplication->activatePerformRightMove();
            totalX = dp.x;
        }
    }
    else
    {
        totalX = 0.0;
        totalY = 0.0;
        vmaxX = 0.0;
        vmaxY = 0.0;
        swipeState = 0;
    }
}

- (IBAction)handleLongPress:(UILongPressGestureRecognizer*)sender
{
    if (sender.state == UIGestureRecognizerStateBegan)
        mApplication->activatePerformBeginLongPress();
    else if (sender.state == UIGestureRecognizerStateEnded)
        mApplication->activatePerformEndLongPress();
}

- (BOOL)gestureRecognizer:(UIGestureRecognizer*)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer*)otherGestureRecognizer
{
    return YES;
}

- (NSUInteger)supportedInterfaceOrientations {
    return UIInterfaceOrientationMaskAll;
}

- (UIInterfaceOrientation)preferredInterfaceOrientationForPresentation {
    return UIInterfaceOrientationPortrait;
}

- (void)cleanup
{
    mApplication->endGame();
    try
    {
        Ogre::Root::getSingleton().queueEndRendering();
        OgreFramework::getSingletonPtr()->m_pRenderWnd->setActive(false);
    }
    catch( Ogre::Exception& e )
    {
        std::cerr << "An exception has occurred: " <<
        e.getFullDescription().c_str() << std::endl;
    }
    
    if (mDisplayLinkSupported)
    {
        [mDate release];
        mDate = nil;
        
        [mDisplayLink invalidate];
        mDisplayLink = nil;
    }
    else
    {
        [mTimer invalidate];
        mTimer = nil;
    }
    
    [mOgreView release];
}

- (void)dealloc
{
    [self cleanup];
    [super dealloc];
}
@end