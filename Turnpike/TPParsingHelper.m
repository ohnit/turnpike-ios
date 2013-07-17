//
//  TPParsingHelper.m
//  Turnpike
//
//  Created by James Lawrence Turner on 7/16/13.
//  Copyright (c) 2013 URX. All rights reserved.
//

#import "TPParsingHelper.h"

#define MULTIPLE_COLONS [NSRegularExpression regularExpressionWithPattern:@"::+" options:0 error:nil]
#define MULTIPLE_SLASHES [NSRegularExpression regularExpressionWithPattern:@"//+" options:0 error:nil]
#define QUERY_STRING [NSRegularExpression regularExpressionWithPattern:@"\\?" options:0 error:nil]
#define LEADING_TRAILING_SLASHES [NSRegularExpression regularExpressionWithPattern:@"(^/)|(/$)" options:0 error:nil]

#define EMPTY_STRING @""
#define SINGLE_SLASH @"/"
#define COLON @":"

@implementation TPParsingHelper

+ (NSString *)sanitizeMappedPath:(NSString *)path {
    NSMutableString *sanitizedPath = [path mutableCopy];
    [MULTIPLE_SLASHES replaceMatchesInString:sanitizedPath options:0 range:NSMakeRange(0, sanitizedPath.length) withTemplate:SINGLE_SLASH];
    return [sanitizedPath copy];
}

+ (void)validateDispatchedPath:(NSString *)dispatchedPath error:(NSError **)error {
    
}


@end
