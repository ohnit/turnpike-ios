//
//  TPURIHelper.h
//  Turnpike
//
//  Created by James Lawrence Turner on 7/16/13.
//  Copyright (c) 2013 URX. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface TPURIHelper : NSObject

+ (NSURL *)sanitizeURL:(NSURL *)url;
+ (NSURL *)sanitizeString:(NSString *)inputString;
+ (NSDictionary *)queryStringToMap:(NSString *)queryString;
+ (NSString *)safeSchemeFromURL:(NSURL *)url;

@end
