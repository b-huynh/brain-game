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

//#define CONTROL_SPIN_DEBUG_OUTPUT

// private category
@interface MainViewController ()
{
    NSTimer* mTimer;
    OgreApp mApplication;
    
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
    
    unsigned int screenWidth;
    unsigned int screenHeight;
    float angleCurrent;
    float anglePrevious;
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
    panRecognizer.minimumNumberOfTouches = 1;
    panRecognizer.maximumNumberOfTouches = 1;
    [self.view addGestureRecognizer:panRecognizer];
    
    UIPinchGestureRecognizer *pinchRecognizer = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(handlePinch:)];
    [self.view addGestureRecognizer:pinchRecognizer];
    
    /*
    UILongPressGestureRecognizer *shortPressRecognizer = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(handleShortPress:)];
    shortPressRecognizer.minimumPressDuration = 0.01;
    [self.view addGestureRecognizer:shortPressRecognizer];
    
    UILongPressGestureRecognizer *longPressRecognizer = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(handleLongPress:)];
    longPressRecognizer.minimumPressDuration = 0.20;
    [self.view addGestureRecognizer:longPressRecognizer];
     */
    
    // Required if double tap should override single tap
    //[singleTapRecognizer requireGestureRecognizerToFail:doubleTapRecognizer];
    
    singleTapRecognizer.delegate = self;
    doubleTapRecognizer.delegate = self;
    panRecognizer.delegate = self;
    pinchRecognizer.delegate = self;
    /*
    longPressRecognizer.delegate = self;
    shortPressRecognizer.delegate = self;
     */
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)startWithWindow:(UIWindow*)window :(NSString*)str
{
    unsigned int width  = self.view.frame.size.width / 2;
    unsigned int height = self.view.frame.size.height / 2;
 
    NSLog(@"Frame: %d %d\n", width, height);
    
    screenWidth = width;
    screenHeight = height;
    
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
    
    mOgreView = [[OgreView alloc] initWithFrame:CGRectMake(0,0,width,height)];
    
    try
    {
        //mApplication = OgreApp();
        mApplication.startDemo(window, mOgreView, width, height, [str UTF8String]);
        
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
    
    // After days and days of research, this is the resolution to being able to start
    // in landscape view in Ogre...
    // http://stackoverflow.com/questions/1350233/rotate-uiview-in-cocoa-touch
    CGAffineTransform transform = CGAffineTransformMakeRotation(2 * M_PI);
    mOgreView.transform = transform;
    
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
    mApplication.saveState();
    // Throws an exception in iOS 8.0, we don't need the config file anyway
    //Ogre::Root::getSingleton().saveConfig();
}

- (void)activate
{
    OgreFramework::getSingletonPtr()->m_pRenderWnd->setActive(true);
}

- (void)update:(id)sender
{
    //NSLog(@"Update");
    if(!OgreFramework::getSingletonPtr()->m_pRenderWnd->isActive())
        return;
    //NSLog(@"Pass");
    
    if(!OgreFramework::getSingletonPtr()->isOgreToBeShutDown() &&
       Ogre::Root::getSingletonPtr() && Ogre::Root::getSingleton().isInitialised())
    {
        //mStartTime = OgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU();
            
        mApplication.update(mDeltaTime);
        OgreFramework::getSingletonPtr()->updateOgre(mDeltaTime);
            
        OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();
            
        //mLastFrameTime = OgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU() - mStartTime;
    }
    if (mOgreView.resized)
    {
        mApplication.requestResize();
        mOgreView.resized = false;
    }
}

/*
- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
    UITouch *touch = [[event allTouches] anyObject];
    start = [touch locationInView:self.view];
    
    mApplication.activateTouchPress(start.x, start.y);
}

- (void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event
{
    UITouch *touch = [[event allTouches] anyObject];
    end = [touch locationInView:self.view];
    
    mApplication.activateTouchRelease(end.x, end.y);
}

- (void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event
{
    UITouch *touch = [[event allTouches] anyObject];
    CGPoint loc = [touch locationInView:self.view];
    
    mApplication.activateTouchMove(loc.x - end.x, loc.y - end.y);
    
    end = loc;
}

- (IBAction)handleSwipeLeft:(UISwipeGestureRecognizer*)sender
{
    //mApplication.activatePerformLeftMove();
}

- (IBAction)handleSwipeRight:(UISwipeGestureRecognizer*)sender
{
    //mApplication.activatePerformRightMove();
}

- (IBAction)handleSwipeDown:(UISwipeGestureRecognizer*)sender
{
    CGFloat dx = end.x - start.x;
    mApplication.activateTouchRelease(dx, end.y - start.y);
    if (dx < -1.0)
        mApplication.activatePerformLeftMove();
    else if (dx > 1.0)
        mApplication.activatePerformRightMove();
}

- (IBAction)handleSwipeUp:(UISwipeGestureRecognizer*)sender
{
    CGFloat dx = end.x - start.x;
    mApplication.activateTouchRelease(dx, end.y - start.y);
    if (dx < -1.0)
        mApplication.activatePerformLeftMove();
    else if (dx > 1.0)
        mApplication.activatePerformRightMove();
}
*/

- (IBAction)handleDoubleTap:(UITapGestureRecognizer*)sender
{
    CGPoint loc = [sender locationInView:self.view];
    if (sender.state == UIGestureRecognizerStateEnded)
        mApplication.activatePerformDoubleTap(loc.x, loc.y);
}

- (IBAction)handleSingleTap:(UITapGestureRecognizer*)sender
{
    CGPoint loc = [sender locationInView:self.view];
    if (sender.state == UIGestureRecognizerStateEnded)
        mApplication.activatePerformSingleTap(loc.x, loc.y);
}

- (IBAction)handlePinch:(UIPinchGestureRecognizer*)sender
{
    if (sender.state == UIGestureRecognizerStateEnded)
    {
        NSLog(@"%f\n", sender.scale);
        if (sender.scale <= 0.75 || sender.scale >= 1.5)
            mApplication.activatePerformPinch();
    }
}

- (IBAction)handlePan:(UIPanGestureRecognizer*)sender
{
    CGPoint p = [sender locationInView:sender.view];
    CGPoint dp = [sender translationInView:sender.view];
    CGPoint v = [sender velocityInView:sender.view];
    
    CGFloat centerX = screenHeight / 2.0;
    CGFloat centerY = screenWidth / 2.0;
    
    CGFloat vX = 0;
    CGFloat vY = -centerY;
    
    CGFloat uX = centerX - p.x;
    CGFloat uY = centerY - p.y;
    
    CGFloat UDotV = (uX * vX) + (uY * vY);
    CGFloat UDotU = (uX * uX) + (uY * uY);
    CGFloat VDotV = (vX * vX) + (vY * vY);
    CGFloat val = (UDotV) / (sqrtf(UDotU) * sqrtf(VDotV));
    CGFloat angle = acosf(val);
    
    if (sender.state == UIGestureRecognizerStateBegan)
    {
        mApplication.activateVelocity(0.0);
        mApplication.activatePressed(p.x, p.y);
        
        angleCurrent = angle;
        anglePrevious = angleCurrent;
        
#ifdef CONTROL_SPIN_DEBUG_OUTPUT
        NSLog(@"T: (%f, %f)", dp.x, dp.y);
        NSLog(@"Beginning Pan");
#endif
    }
    else if (sender.state == UIGestureRecognizerStateChanged)
    {
        angleCurrent = angle;
        CGFloat dT = angleCurrent - anglePrevious;
        dT = fabs(dT);
        anglePrevious = angleCurrent;
        
#ifdef CONTROL_SPIN_DEBUG_OUTPUT
        NSLog(@"DT: %f", dT);
#endif
        
        /* Calculate Cross Product from center to Velocity vector */
        CGFloat centerX = screenWidth / 2.0;
        CGFloat centerY = screenHeight / 2.0;
        
        CGFloat uX = centerX - p.x;
        CGFloat uY = centerY - p.y;
        
        CGFloat crossZ = (uX * v.y) - (uY * v.x);
        
        BOOL clockwise = crossZ < 0.0;
        
        /* Accrue speed */
        CGFloat dot = (v.x * v.x) + (v.y * v.y);
        CGFloat magnitude = sqrtf(dot);
        
#ifdef CONTROL_SPIN_DEBUG_OUTPUT
        NSLog(@"Speed: %f", magnitude);
        NSLog(@"V: (%f, %f)", v.x, v.y);
#endif
        
        if (clockwise) {
            mApplication.activateAngleTurn(-dT, magnitude);
        } else {
            mApplication.activateAngleTurn(dT, magnitude);
        }
        mApplication.activateMoved(p.x, p.y, dp.x, dp.y);
    }
    else
    {
        /* Calculate Cross Product from center to Velocity vector */
        CGFloat centerX = screenWidth / 2.0;
        CGFloat centerY = screenHeight / 2.0;
        
        CGFloat uX = centerX - p.x;
        CGFloat uY = centerY - p.y;
        
        CGFloat crossZ = (uX * v.y) - (uY * v.x);
        
        BOOL clockwise = crossZ < 0.0;
        
        /* Accrue speed */
        CGFloat dot = (v.x * v.x) + (v.y * v.y);
        CGFloat magnitude = sqrtf(dot);
        
#ifdef CONTROL_SPIN_DEBUG_OUTPUT
        NSLog(@"Speed: %f", magnitude);
        NSLog(@"V: (%f, %f)", v.x, v.y);
#endif
        
        if (clockwise) {
            mApplication.activateVelocity(magnitude);
        } else {
            mApplication.activateVelocity(-magnitude);
        }
        mApplication.activateReleased(p.x, p.y, dp.x, dp.y);
        
#ifdef CONTROL_SPIN_DEBUG_OUTPUT
        NSLog(@"End Pan");
        NSLog(@"V: (%f, %f)", v.x, v.y);
#endif
    }
}

- (IBAction)handleLongPress:(UILongPressGestureRecognizer*)sender
{
    if (sender.state == UIGestureRecognizerStateBegan)
        mApplication.activatePerformBeginLongPress();
    else if (sender.state == UIGestureRecognizerStateEnded)
        mApplication.activatePerformEndLongPress();
}

- (IBAction)handleShortPress:(UILongPressGestureRecognizer*)sender
{
    if (sender.state == UIGestureRecognizerStateBegan)
        mApplication.activatePerformBeginShortPress();
    else if (sender.state == UIGestureRecognizerStateEnded)
        mApplication.activatePerformEndShortPress();
}

- (BOOL)gestureRecognizer:(UIGestureRecognizer*)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer*)otherGestureRecognizer
{
    return YES;
}

- (NSUInteger)supportedInterfaceOrientations {
    NSUInteger mask = 0;
    return mask | UIInterfaceOrientationMaskLandscape;
}

- (BOOL)shouldAutoRotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return interfaceOrientation == UIInterfaceOrientationLandscapeLeft || interfaceOrientation == UIInterfaceOrientationLandscapeRight;
}

- (BOOL)shouldAutorotate {
    UIInterfaceOrientation orientation = [[UIApplication sharedApplication] statusBarOrientation];
    
    return (orientation == UIInterfaceOrientationLandscapeLeft) || (orientation == UIInterfaceOrientationLandscapeRight);
}

/*
- (void)encodeRestorableStateWithCoder:(NSCoder *)coder
{
    [super encodeRestorableStateWithCoder:coder];
}

- (void)decodeRestorableStateWithCoder:(NSCoder *)coder
{
    [super decodeRestorableStateWithCoder:coder];
}
*/

- (void)cleanup
{
    mApplication.endGame();
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