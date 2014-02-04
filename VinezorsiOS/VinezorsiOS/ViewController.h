//
//  MainViewController.h
//  VinezorsiOS
//
//  Created by Calvin Phung on 1/14/14.
//
//

#import <UIKit/UIKit.h>

@class OgreView;

@interface ViewController : UIViewController  <UIGestureRecognizerDelegate>

- (void)startWithWindow:(UIWindow*) window;
- (void)update:(id)sender;
- (void)stop;

/*
- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event;
- (void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event;
- (void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event;
- (IBAction)handleSwipeLeft:(UISwipeGestureRecognizer*)sender;
- (IBAction)handleSwipeRight:(UISwipeGestureRecognizer*)sender;
- (IBAction)handleSwipeUp:(UISwipeGestureRecognizer*)sender;
- (IBAction)handleSwipeDown:(UISwipeGestureRecognizer*)sender;
 */

- (IBAction)handleDoubleTap:(UITapGestureRecognizer*)sender;
- (IBAction)handleSingleTap:(UITapGestureRecognizer*)sender;
- (IBAction)handlePinch:(UIPinchGestureRecognizer*)sender;
- (IBAction)handlePan:(UIPanGestureRecognizer*)sender;
- (void)cleanup;

@end
