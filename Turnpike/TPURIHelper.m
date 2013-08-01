//
//  TPURIHelper.m
//  Turnpike
//
//  Created by James Lawrence Turner on 7/16/13.
//  Copyright (c) 2013 URX. All rights reserved.
//

#import "TPURIHelper.h"

#define LEADING_SLASHES [NSRegularExpression regularExpressionWithPattern:@"^//+"  options:0 error:nil]
#define SINGLE_SLASH @"/"
#define COLON @":"

// "every non-slash character up until the first ':' character
#define URI_SCHEME [NSRegularExpression regularExpressionWithPattern:@"^[^/]+:"  options:0 error:nil]
#define EMPTY_STRING @""
#define AMPERSAND @"&"
#define EQUALS @"="

@implementation TPURIHelper

+ (NSURL *)sanitizeURL:(NSURL *)url {
    return [self sanitizeString:url.absoluteString];
}

+ (NSURL *)sanitizeString:(NSString *)inputString {
    // Parse schema and query parameters and invoke route
    NSString *accumulatedQueryString = @"";
    NSString *queryString = nil;
    NSString *accumulatedSchema = @"";
    NSString *schema = nil;
    NSString *resourcePath = nil;
    
    // Strip out Query String
    for (int i = inputString.length-1; i >= 0; i--) {
        if ([inputString characterAtIndex:i] == '?') {
            resourcePath = [inputString substringToIndex:i];
            queryString = [accumulatedQueryString copy];
            break;
        }
        else {
            accumulatedQueryString = [[NSString stringWithFormat:@"%c",[inputString characterAtIndex:i]] stringByAppendingString:accumulatedQueryString];
        }
    }
    // If there was no query string, then the query string is blank and the resource path is the url (before trimming)
    if (queryString == nil) {
        queryString = @"";
        resourcePath = inputString;
    }
    
    // Strip out the url scheme from the resource path
    for (int i = 0; i < inputString.length; i++) {
        // If we hit a slash before we hit a colon, there is no schema and this is invoked internally
        if ([inputString characterAtIndex:i] == '/') {
            schema = nil;
            break;
        }
        
        // Lets continue looking for the schema
        accumulatedSchema = [accumulatedSchema stringByAppendingString:[NSString stringWithFormat:@"%c", [inputString characterAtIndex:i]]];
        
        // We've got our schema, lets break out of this loop
        if ([inputString characterAtIndex:i] == ':') {
            schema = accumulatedSchema;
            break;
        }
    }
    // If we found a schema, lets strip it from the resource path
    if(schema != nil) {
        if (resourcePath.length == schema.length) {
            resourcePath = @"";
        }
        else {
            resourcePath = [resourcePath substringFromIndex:[schema length]];
        }
    }
    
    // Strip out any leading slashes, if any, from the resource path
    while (resourcePath.length > 0 && [resourcePath characterAtIndex:0] == '/') {
        // If the path is just "/" resolve to ""
        if (resourcePath.length == 1) {
            resourcePath = @"";
        }
        // Remove front most slash
        else {
            resourcePath = [resourcePath substringFromIndex:1];
        }
    }
    
    NSMutableString *sanitizedResourcePath = [resourcePath mutableCopy];
    [LEADING_SLASHES replaceMatchesInString:sanitizedResourcePath options:0 range:NSMakeRange(0, sanitizedResourcePath.length) withTemplate:SINGLE_SLASH];
    
    
    NSString *sanitizedURLString = [NSString stringWithFormat:@"%@//%@%@",
                                    (schema != nil ? schema:@":"),
                                    [sanitizedResourcePath copy],
                                    (queryString != nil && queryString.length > 0 ? [@"?" stringByAppendingString:queryString] : @"")];
    return [NSURL URLWithString:sanitizedURLString];
}

+ (NSDictionary *)queryStringToMap:(NSString *)queryString {
    // Create a dictionary to hold the parameters that we're parsing
    NSMutableDictionary *queryParameters = [NSMutableDictionary dictionary];
    // Break up all the parameters in the query string (separated by '&' or ';') into 'key=value'
    NSArray *parameters = [queryString componentsSeparatedByCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@"&;"]];
    for (NSString *parameter in parameters) {
        // Lets put the key and value as strings in an array
        NSArray *keyValuePair = [parameter componentsSeparatedByString:@"="];
        // If we don't have a key/value pair (two strings in the array) then this is malformed, so let's skip it
        if (keyValuePair.count != 2) {
            continue;
        }
        // Save our key value pair in our dictionary
        [queryParameters setValue:[keyValuePair objectAtIndex:1] forKey:[keyValuePair objectAtIndex:0]];
    }
    // Return an immutable copy of our parameters
    return [queryParameters copy];
}

+ (NSString *)safeSchemeFromURL:(NSURL *)url {
    return url.scheme && url.scheme.length > 0 ? url.scheme : nil;
}

@end
