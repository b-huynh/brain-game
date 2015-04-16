//
//  OgreView.mm
//  VinezorsiOS
//
//  Created by Calvin Phung on 1/14/14.
//
//

#import <OgreView.h>

#import <QuartzCore/CAEAGLLayer.h>

#include <OgreRenderWindow.h>
#include <OgreCamera.h>

#include "OgreFramework.h"

@implementation OgreView

@synthesize mWindowName;
@synthesize mRenderWindow;
@synthesize resized;

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
    }
    return self;
}

+ (Class) layerClass
{
    return [CAEAGLLayer class];
}

- (void)layoutSubviews
{
    if(mRenderWindow != NULL)
    {
        unsigned int width = (uint)self.bounds.size.width;
        unsigned int height = (uint)self.bounds.size.height;
        
        // Resize the window
        mRenderWindow->resize(width, height);
        OgreFramework::getSingletonPtr()->requestResize();
        resized = true;
    }
}

@end