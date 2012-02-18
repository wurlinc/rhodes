/*------------------------------------------------------------------------
 * (The MIT License)
 * 
 *  Copyright (c) 2012 wurl. All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * 
 * http://wurl.com
 *------------------------------------------------------------------------*/
//  InteractiveSplash.m
//  rhorunner
//
//  Created by David Martinez on 2/15/12.

#import "InteractiveSplash.h"
#import "RhodesAppBase.h"

@implementation InteractiveSplash

@synthesize url = _url;
@synthesize delegate = _delegate;
@synthesize loaded = _loaded;

#pragma mark Rhodes Forward Declarations

void rho_app_interactivesplash_done();


#define INTERACTIVE_SPLASH_FILENAME "/apps/app/interactive_splash/splash.html"

#define CONFIG_FILENAME "/apps/rhoconfig.txt"
#define INTERACIVE_SPLASH_CONFIG_ENTRY "iphone_splash_delegate_class"


#pragma mark Factory and Utility
+ (BOOL) hasInteractiveSplash
{
    NSString *resourcePath = [[NSBundle mainBundle] resourcePath];
    NSString *interactiveSplashPath = [resourcePath stringByAppendingString:@INTERACTIVE_SPLASH_FILENAME];
    return [[NSFileManager defaultManager] fileExistsAtPath:interactiveSplashPath];
}

+ (InteractiveSplash*) maybeShowInteractiveSplashInWindow:(UIWindow*)window
{
    InteractiveSplash *splash;
    if ( [self hasInteractiveSplash] ) {
        splash = [[InteractiveSplash alloc] init];
        [splash addToWindow:window];
        splash.url = @INTERACTIVE_SPLASH_FILENAME;
    }
    return splash;
}

+(NSString*) readDelegateClassName
{
    NSString *resourcePath = [[NSBundle mainBundle] resourcePath];
    NSString *configFilePath = [resourcePath stringByAppendingString:@CONFIG_FILENAME];
    NSError *error = nil;
    NSString *configString  = [NSString stringWithContentsOfFile:configFilePath encoding:NSUTF8StringEncoding error:&error];
    if (!configString) {
        NSLog(@"Error: %@", error);
        return nil;
    }
    NSArray *lines = [configString componentsSeparatedByCharactersInSet:[NSCharacterSet newlineCharacterSet]];
    for (NSString* line in lines) {
        NSArray  *nvpair = [line componentsSeparatedByString:@"="];
        NSString *potentialKey   = [nvpair objectAtIndex:0];
        NSString *potentialValue = [nvpair lastObject];
        if ( [potentialKey isEqualToString:@INTERACIVE_SPLASH_CONFIG_ENTRY] ) {
            return [potentialValue stringByTrimmingCharactersInSet:[NSCharacterSet newlineCharacterSet]];
        }
    }
    return NULL;
}

- (UIWebView*) webView
{
    UIWebView* _wv = [[[self view] subviews] lastObject];
    return _wv;
}

#pragma mark Networking Stuff

- (NSURLRequest *) requestFromURL:(NSString *)url {
    return [NSURLRequest requestWithURL:[NSURL URLWithString:url]];
}


- (NSString *)canonicalizeUrlForSplash {
    NSString *resourcePath = [[NSBundle mainBundle] resourcePath];
    NSString *fileURL = [NSString stringWithFormat:@"file://%@", [resourcePath stringByAppendingString:self.url]];
    return fileURL;
}


- (void) loadURL {
    // Assumes webview is always last.
    UIWebView* _wv = [self webView];
    if (!self.url || !_wv) {
        NSLog(@"InteractiveSplash no Webview or URL. Not loading URL yet.");
        return;
    }
//    NSLog(@"InteractiveSplash Loading URL %@ into webview", self.url);
    _wv.delegate = self;

    NSString *canonURL = [self canonicalizeUrlForSplash];
    NSString *encodedUrl = [self encodeUrl:canonURL];
    NSURL* urlToLoad = [NSURL URLWithString:encodedUrl];
    
    NSLog(@"InteractiveSplash Requesting URL %@ in InteractiveSplash view into %@", encodedUrl, _wv);
        NSLog(@"Requesting URL %@ as Request URL", encodedUrl);
        NSURLRequest *request = [[NSMutableURLRequest alloc] initWithURL:urlToLoad];
        [_wv loadRequest:request];
        [request release];
    [_wv loadRequest:[self requestFromURL:self.url]];
    self.loaded = true;
}


#pragma mark Window Management and Interaction

- (NSString *) stringByEvaluatingJavascriptString:(NSString *)string
{
    // Just forward the call.
    return [self.webView stringByEvaluatingJavaScriptFromString:string];
}

- (void) hide {
        UIApplication *app = [UIApplication sharedApplication];
        [app setStatusBarHidden:NO withAnimation:UIStatusBarAnimationFade];
    [self dismissModalViewControllerAnimated:YES];
}

- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
    if (self.url && ! self.loaded)
        [self loadURL];
//    SEL didAppearSelector = @selector(splashScreenDidAppear:); if ([self.delegate respondsToSelector:didAppearSelector]) {
//        [self.delegate splashScreenDidAppear:self];
//    }
//    [self performSelector:@selector(hide) withObject:nil afterDelay:0];
}

- (void)splashScreenDidDisappear:(InteractiveSplash *)splashScreen { 
//    webView = nil;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:
(UIInterfaceOrientation)toInterfaceOrientation {
    return NO;
}

- (void) addToWindow:(UIWindow *)window {
    NSLog(@"InteractiveSplash Adding to Window.");
	self.view.autoresizingMask = UIViewAutoresizingFlexibleLeftMargin | UIViewAutoresizingFlexibleTopMargin | UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
	self.view.autoresizesSubviews = YES;
    
    UIApplication *app = [UIApplication sharedApplication];
    [app setStatusBarHidden:YES withAnimation:UIStatusBarAnimationFade];

    [window addSubview:self.view];
    NSLog(@"InteractiveSplash Added to Window.");
}

- (void)viewDidLoad {
    self.url = nil;
    
    CGRect webViewFrame = [[UIScreen mainScreen] applicationFrame];
    self.view.frame = webViewFrame;
    NSLog(@"InteractiveSplash Loading Webview");
    UIWebView* _wv = [[UIWebView alloc] initWithFrame:webViewFrame];

    _wv.autoresizesSubviews = YES;
    _wv.scalesPageToFit = YES;
    _wv.userInteractionEnabled = YES;
    _wv.multipleTouchEnabled = YES;
    _wv.clipsToBounds = NO;
    _wv.dataDetectorTypes = UIDataDetectorTypeNone;
    _wv.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
    //	webView.autoresizingMask = UIViewAutoresizingFlexibleLeftMargin | UIViewAutoresizingFlexibleTopMargin | UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
    _wv.hidden = YES;
    _wv.opaque = NO;
    _wv.backgroundColor = [UIColor clearColor];
    self.view.backgroundColor = [UIColor blackColor];
    
    NSLog(@"InteractiveSplash Loading Webview Done.");
    //    [parentView addSubview:splashView];
    //    [self addToWindow:[[UIApplication sharedApplication] keyWindow]];
    self.view.autoresizesSubviews = YES;
    [self.view addSubview:_wv];
    _wv.delegate = self;
    
    NSString *delegateClassName = [InteractiveSplash readDelegateClassName];
    if ( delegateClassName ) {
        NSLog(@"Splash Delegate: %@", delegateClassName);
        id obj = [[NSClassFromString(delegateClassName) alloc] init];
        self.delegate = obj;
    }
}

#pragma mark Lifted from elsewhere in Rhodes
- (NSString*)encodeUrl:(NSString*)url {
    // This decode/encode trick allow to work properly with urls which are already encoded
    // in the same manner as with those which are not. In case if 'url' is already encoded,
    // encodedUrl will be exactly the same as original one whereas if original url was not
    // encoded, encodedUrl will contain correct encoded version
    NSString *decodedUrl = [url stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    NSString *encodedUrl = [decodedUrl stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    return encodedUrl;
}

/* hideSplash gets called from a separate thread. We force it to wait using CSplashScreen and
   only then we call hide. */
- (void)closeSplash {
    [self stringByEvaluatingJavascriptString:@"if ( typeof(splash_will_close) == 'function' ) { splash_will_close(); }"];
    if (self.delegate)
        [self.delegate splashScreenWillDisappear:self];
    rho_app_interactivesplash_done();
    self.view.hidden = YES;
    [self.view removeFromSuperview];
}

#pragma mark UIWebView Delegate Start
/** UIWebview Delegate */

- (void) handleSplashCommand:(NSURLRequest *)splashCommand {
    
    NSString *baseCommand = [[splashCommand URL] resourceSpecifier];
    // Handle Internal Commands
    if ([baseCommand isEqualToString:@"//close"]) {
        [self closeSplash];
    } else if (self.delegate) {
        [self.delegate handleSplashCommand:splashCommand WithInteractiveSplash:self];
    }
}

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType {
    if ( navigationType == UIWebViewNavigationTypeLinkClicked )
        return YES;
    
    if ( [[request URL] isFileURL] ) {
        if ( ! [[NSFileManager defaultManager] fileExistsAtPath:[[request URL] path]] ) {
           NSLog(@"File %@ not found locally. Ignoring.", [[request URL] path]);
           return NO;
        } else {
            return YES;
        }
    }
    if ( [[[request URL] scheme] isEqualToString:@"splash"]  ) {
        [self handleSplashCommand:request];
        return NO;
    } else { // No Network URLs. Only local and splash
        return NO;
    }
    NSLog(@"InteractiveSplash shouldStartLoadWithRequest req: %@ NavType: %i:? NO", request, navigationType);    
}

- (void)webViewDidStartLoad:(UIWebView *)webView {
    NSLog(@"InteractiveSplash started loading");
}

- (void)webViewDidFinishLoad:(UIWebView *)webView {
    UIWebView* _wv = [self webView];
    _wv.hidden = NO;
    _wv.opaque = YES;
    NSLog(@"InteractiveSplash finished loading");

}
- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error {
    NSString *titleString = @"Error Loading Page";
    NSString *messageString = [error localizedDescription];
    NSString *moreString = [error localizedFailureReason] ?
    [error localizedFailureReason] :
    NSLocalizedString(@"Try typing the URL again.", nil);
    messageString = [NSString stringWithFormat:@"%@. %@", messageString, moreString];
    
    UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:titleString
                                                        message:messageString delegate:self
                                              cancelButtonTitle:@"Cancel" otherButtonTitles:nil];
    [alertView show];
    [alertView release];

    NSLog(@"InteractiveSplash Failed Load: %@", error);
}
#pragma mark 




@end
