//
//  SplashRhoConfig.h
//  rhorunner
//
//  Created by David Martinez on 2/19/12.
//  Copyright (c) 2012 wurl. All rights reserved.
//

#import <Foundation/Foundation.h>

#define CONFIG_FILENAME "/apps/rhoconfig.txt"


@interface SplashRhoConfig : NSObject

+(NSDictionary *) rhoConfig;

@end
