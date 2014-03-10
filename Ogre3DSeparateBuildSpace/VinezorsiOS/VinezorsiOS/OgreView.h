//
//  OgreView.h
//  VinezorsiOS
//
//  Created by Calvin Phung on 1/14/14.
//
//
#import <OgreString.h>

#import <UIKit/UIKit.h>

@interface OgreView : UIView
{
    Ogre::String mWindowName;
    Ogre::RenderWindow* mRenderWindow;
    BOOL resized;
}

@property Ogre::String mWindowName;
@property Ogre::RenderWindow* mRenderWindow;
@property BOOL resized;

@end