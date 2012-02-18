//
//  InteractiveSplashProtocol.h
//  rhorunner
//
//  Created by David Martinez on 2/15/12.
//  Copyright (c) 2012 wurl. All rights reserved.
//

#import <Foundation/Foundation.h>

@class InteractiveSplash;

@protocol InteractiveSplashDelegate <NSObject>

@optional
- (void)splashScreenDidAppear:(InteractiveSplash *)splashScreen;
- (void)splashScreenWillDisappear:(InteractiveSplash *)splashScreen; - (void)splashScreenDidDisappear:(InteractiveSplash *)splashScreen;
@end
