//
//  TPParsingHelper.m
//  Turnpike
//
//  Created by James Lawrence Turner on 7/16/13.
//  Copyright (c) 2013 URX. All rights reserved.
//

#import "TPParsingHelper.h"

#define ERROR_DOMAIN @"com.urx.turnpike.parsing"
#define MULTIPLE_SLASHES_ERROR_CODE 1
#define LEADING_TRAILING_SLASHES_ERROR_CODE 2
#define MULTIPLE_COLONS_ERROR_CODE 2

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
    [LEADING_TRAILING_SLASHES replaceMatchesInString:sanitizedPath options:0 range:NSMakeRange(0, sanitizedPath.length) withTemplate:EMPTY_STRING];
    [MULTIPLE_COLONS replaceMatchesInString:sanitizedPath options:0 range:NSMakeRange(0, sanitizedPath.length) withTemplate:COLON];
    
    return [sanitizedPath copy];
}

+ (void)validateDispatchedPath:(NSString *)dispatchedPath error:(NSError **)error {
    if ([MULTIPLE_SLASHES firstMatchInString:dispatchedPath options:0 range:NSMakeRange(0, dispatchedPath.length)]) {
        *error = [NSError errorWithDomain:ERROR_DOMAIN code:MULTIPLE_SLASHES_ERROR_CODE userInfo:@{NSLocalizedDescriptionKey: @"Path must not contain multiple consecutive slashes."}];
        return;
    }
    if ([LEADING_TRAILING_SLASHES firstMatchInString:dispatchedPath options:0 range:NSMakeRange(0, dispatchedPath.length)]) {
        *error = [NSError errorWithDomain:ERROR_DOMAIN code:LEADING_TRAILING_SLASHES_ERROR_CODE userInfo:@{NSLocalizedDescriptionKey: @"Path must not contain leading or trailing slashes."}];
        return;
    }
    if ([MULTIPLE_COLONS firstMatchInString:dispatchedPath options:0 range:NSMakeRange(0, dispatchedPath.length)]) {
        *error = [NSError errorWithDomain:ERROR_DOMAIN code:MULTIPLE_COLONS_ERROR_CODE userInfo:@{NSLocalizedDescriptionKey: @"Path must not contain multiple consecutive colons."}];
        return;
    }
}


@end
