//
//  InteractiveSplash.h
//  rhorunner
//
//  Created by David Martinez on 2/15/12.
//  Copyright (c) 2012 wurl. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "InteractiveSplashDelegate.h"

@interface InteractiveSplash : UIViewController <UIWebViewDelegate> {}

/** The URL that we will load. Set so we can load it. */
@property (nonatomic, copy, readwrite) NSString *url;
@property (readonly) UIWebView *webView;

/** 
 * A delegate to subscribe to events.
 */
@property (nonatomic, assign) IBOutlet id<InteractiveSplashDelegate> delegate;

@property (nonatomic, assign) BOOL loaded;

- (void)hide;
- (void) addToWindow:(UIWindow *)window;
- (NSString*)encodeUrl:(NSString*)url;
- (void)closeSplash;
- (NSString *) stringByEvaluatingJavascriptString:(NSString *)string;

+ (InteractiveSplash*) maybeShowInteractiveSplashInWindow:(UIWindow*)window;
+ (BOOL) hasInteractiveSplash;

/* Delegate not running. Trying to declare these */

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType;
- (void)webViewDidStartLoad:(UIWebView *)webView;
- (void)webViewDidFinishLoad:(UIWebView *)webView;
- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error;

@end