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

void rho_splash_screen_hide();

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
    UIWebView* _wv = [[[self view] subviews] lastObject];
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
//    if ([urlToLoad isFileURL]) {
//        // Load the file and put it in.
//        NSError *err;
//        NSString *data = [NSString stringWithContentsOfFile:[urlToLoad path] encoding:NSUTF8StringEncoding error:&err];
////        NSString *data = @"<html><body bgcolor='yellow'><h1>DUDE</h1></body></html>";
//        NSLog(@"InteractiveSplash Requesting URL %@ as File URL with data: %@", [urlToLoad path], data);
////        [urlToLoad URLByDeletingLastPathComponent]
//        [_wv loadHTMLString:data baseURL:urlToLoad]; // URL to load or URLByDeletingLastPathComponent?
////        [_wv loadData:[data dataUsingEncoding:NSUTF8StringEncoding] MIMEType:@"text/html" textEncodingName:@"utf-8" baseURL:urlToLoad];
//        NSLog(@"InteractiveSplash Loaded Data as a file");
//
//    } else {
        NSLog(@"Requesting URL %@ as Request URL", encodedUrl);
        NSURLRequest *request = [[NSMutableURLRequest alloc] initWithURL:urlToLoad];
        [_wv loadRequest:request];
        [request release];
//    }
    [_wv loadRequest:[self requestFromURL:self.url]];
    self.loaded = true;
}

// XXX For some reason doing it here doesn't show the splash.
//-(void) setUrl:(NSString *)url {
//    _url = url;
//    if (url) {
//        self.loaded = false;
//        [self loadURL];
//    }
//}

#pragma mark Window Management

- (void) hide {
//    rho_splash_screen_hide(); // This may do the delay we need to do.
//    if (self.showsStatusBarOnDismissal) {
        UIApplication *app = [UIApplication sharedApplication];
        [app setStatusBarHidden:NO withAnimation:UIStatusBarAnimationFade];
//    }
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
    
//    CGRect labelFrame = CGRectMake( 05, 05, 230, 30 );
//    UILabel* label = [[UILabel alloc] initWithFrame: labelFrame];
//    [label setText: @"Proving this works well."];
//    [label setTextColor: [UIColor orangeColor]];
//    [self.view addSubview: label];
    
    CGRect webViewFrame = [[UIScreen mainScreen] applicationFrame];
//    CGRect webViewFrame = CGRectMake(50, 50, 240, 300);
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
    _wv.backgroundColor = [UIColor redColor];
    _wv.hidden = NO;
    _wv.opaque = YES;
    
    self.view.backgroundColor = [UIColor blackColor];
    NSLog(@"InteractiveSplash Loading Webview Done.");
    //    [parentView addSubview:splashView];
    //    [self addToWindow:[[UIApplication sharedApplication] keyWindow]];
    self.view.autoresizesSubviews = YES;
    [self.view addSubview:_wv];
    _wv.delegate = self;
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
- (void)hideSplash {
    rho_splash_screen_hide();
    [self hide];
}


#pragma mark UIWebView Delegate Start
/** UIWebview Delegate */

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
        NSLog(@"InteractiveSplash shouldStartLoadWithRequest scheme Splash. TODO Implement calls!");
        return YES;
    } else { // No Network URLs. Only local and splash
        return NO;
    }
    NSLog(@"InteractiveSplash shouldStartLoadWithRequest req: %@ NavType: %i:? NO", request, navigationType);    
}

- (void)webViewDidStartLoad:(UIWebView *)webView {
    NSLog(@"InteractiveSplash started loading");
}

- (void)webViewDidFinishLoad:(UIWebView *)webView {
    UIWebView* _wv = [[[self view] subviews] lastObject];
    _wv.hidden = NO;
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
