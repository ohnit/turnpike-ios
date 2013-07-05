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
        
        // Append our key value pair to our query string
        queryString = [queryString stringByAppendingString:keyValuePair];
    }
    
    // Return query string
    return queryString;
}

+ (NSDictionary *)dictionaryFromQueryString:(NSString *)queryString {
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
        // Decode the key
        NSString *decodedKey = [self decodeURI:[keyValuePair objectAtIndex:0]];
        // Decode the value
        NSString *decodedValue = [self decodeURI:[keyValuePair objectAtIndex:1]];
        // Save our key value pair in our dictionary
        [queryParameters setValue:decodedValue forKey:decodedKey];
    }
    // Return an immutable copy of our parameters
    return [queryParameters copy];
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
