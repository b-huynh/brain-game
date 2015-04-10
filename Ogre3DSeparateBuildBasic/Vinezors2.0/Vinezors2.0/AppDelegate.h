#ifndef __AppDelegate_H__
#define __AppDelegate_H__

#include "OgrePlatform.h"

#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE
#error This header is for use with Mac OS X only
#endif

#ifdef __OBJC__

#import <Cocoa/Cocoa.h>

// All this does is suppress some messages in the run log.  NSApplication does not
// implement buttonPressed and apps without a NIB have no target for the action.
@implementation NSApplication (_suppressUnimplementedActionWarning)
- (void) buttonPressed:(id)sender
{
    /* Do nothing */
}
@end


#if defined(MAC_OS_X_VERSION_10_6) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6
@interface AppDelegate : NSObject <NSApplicationDelegate>
#else
@interface AppDelegate : NSObject
#endif
{
    NSTimer *mTimer;
    OgreApp demo;
    
    NSDate *mDate;
    double mFrameInterval;
    double mDeltaTime;
}

- (void)go;
- (void)renderOneFrame:(id)sender;

@property (retain) NSTimer *mTimer;
@property (nonatomic) double mFrameInterval;
@property (nonatomic) double mDeltaTime;

@end

#if __LP64__
static id mAppDelegate;
#endif

@implementation AppDelegate

@synthesize mTimer;
@dynamic mFrameInterval;
@dynamic mDeltaTime;

- (double)mFrameInterval
{
    return mFrameInterval;
}

- (void)setTimeDelta:(double)frameInterval
{
    // Frame interval defines how many display frames must pass between each time the
    // display link fires. The display link will only fire 30 times a second when the
    // frame internal is two on a display that refreshes 60 times a second. The default
    // frame interval setting of one will fire 60 times a second when the display refreshes
    // at 60 times a second. A frame interval setting of less than one results in undefined
    // behavior.
    if (frameInterval >= 1)
    {
        mFrameInterval = frameInterval;
    }
}

- (void)go {
    
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    mFrameInterval = 1;
    mDeltaTime = 1.0f / 30.0F;
    mTimer = nil;
    
    try {
        demo.startDemo("subject102", MUSIC_ENABLED);
        
        Ogre::Root::getSingleton().getRenderSystem()->_initRenderTargets();
        
        // Clear event times
		Ogre::Root::getSingleton().clearEventTimes();
    } catch( Ogre::Exception& e ) {
        std::cerr << "An exception has occurred: " <<
        e.getFullDescription().c_str() << std::endl;
    }
    
    mTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)(mDeltaTime) * mFrameInterval
                                              target:self
                                            selector:@selector(renderOneFrame:)
                                            userInfo:nil
                                             repeats:YES];
    [pool release];
}

- (void)applicationDidFinishLaunching:(NSNotification *)application {
    mFrameInterval = 1;
    mDeltaTime = 1.0f / 30.0f;
    mTimer = nil;
    
    [self go];
}

- (void)renderOneFrame:(id)sender
{
    if(!OgreFramework::getSingletonPtr()->isOgreToBeShutDown() &&
       Ogre::Root::getSingletonPtr() && Ogre::Root::getSingleton().isInitialised())
    {
		if(OgreFramework::getSingletonPtr()->m_pRenderWnd->isActive())
		{
			OgreFramework::getSingletonPtr()->m_pKeyboard->capture();
			OgreFramework::getSingletonPtr()->m_pMouse->capture();
            
            demo.update(mDeltaTime);
            OgreFramework::getSingletonPtr()->updateOgre(mDeltaTime);
                
            OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();
		}
    }
    else
    {
        [mTimer invalidate];
        mTimer = nil;
        [NSApp performSelector:@selector(terminate:) withObject:nil afterDelay:0.0];
    }
}

- (void)dealloc {
    if(mTimer)
    {
        [mTimer invalidate];
        mTimer = nil;
    }
    
    [super dealloc];
}

@end

#endif

#endif
