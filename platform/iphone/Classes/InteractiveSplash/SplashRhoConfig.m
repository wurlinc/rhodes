//
//  SplashRhoConfig.m
//  rhorunner
//
//  Created by David Martinez on 2/19/12.
//  Copyright (c) 2012 wurl. All rights reserved.
//

#import "SplashRhoConfig.h"

@implementation SplashRhoConfig


+(NSDictionary *) rhoConfig
{
    NSDictionary* result = [[NSMutableDictionary alloc] init];
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
        [result setValue:potentialValue forKey:potentialKey];
    }
    return result;
}


@end
