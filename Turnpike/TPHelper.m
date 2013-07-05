//
//  TPHelper.m
//  Turnpike
//
//  Created by James Lawrence Turner on 7/5/13.
//  Copyright (c) 2013 URX. All rights reserved.
//

#import "TPHelper.h"
#import <UIKit/UIKit.h>

@implementation TPHelper

+ (NSString *)queryStringFromDictionary:(NSDictionary *)queryParameters {
    // If there are no keys in the query parameters, don't create a query string
    if ([queryParameters.allKeys count] == 0) {
        return @"";
    }
    
    // Start with our query string identifier
    NSString *queryString = @"?";
    
    for (int i = 0; i < [queryParameters.allKeys count]; i++) {
        // Encode the key
        NSString *encodedKey = [self encodeURI:[queryParameters.allKeys objectAtIndex:i]];
        // Encode the value
        NSString *encodedValue = [self encodeURI:[queryParameters objectForKey:[queryParameters.allKeys objectAtIndex:i]]];
        
        // Create the key value pair string (which has an & at the end if it is not the last pair
        NSString *keyValuePair = [NSString stringWithFormat:@"%@=%@%@", encodedKey, encodedValue, i == [queryParameters.allKeys count]-1 ? @"" : @"&"];
        
        // Return query string
        queryString = [queryString stringByAppendingString:keyValuePair];
    }
    
    return queryString;
}

+ (NSString *)encodeURI:(NSString *)string {
    return (NSString *) CFBridgingRelease(
                                          CFURLCreateStringByAddingPercentEscapes(
                                                                                  NULL,
                                                                                  (CFStringRef)string,
                                                                                  NULL,
                                                                                  (CFStringRef)@";/?:@&=$+{}<>,",
                                                                                  CFStringConvertNSStringEncodingToEncoding(NSUTF8StringEncoding)));
}

+ (NSString *)decodeURI:(NSString *)string {
    return (__bridge NSString *) CFURLCreateStringByReplacingPercentEscapesUsingEncoding(NULL,
                                                                                         (__bridge CFStringRef) string,
                                                                                         CFSTR(""),
                                                                                         kCFStringEncodingUTF8);
}

+ (void)invokeExternalURL:(NSString *)url {
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:url]];
}

+ (void)invokeExternalAppWithSchema:(NSString *)schema Route:(NSString *)route AndQueryParameters:(NSDictionary *)queryParameters {
    [self invokeExternalURL:[NSString stringWithFormat:@"%@%@%@",schema,route,[self queryStringFromDictionary:queryParameters]]];
}

+ (BOOL)canInvokeExternalURL:(NSString *)url {
    return [[UIApplication sharedApplication] canOpenURL:[NSURL URLWithString:url]];
}

@end
