//
//  TPHelper.h
//  Turnpike
//
//  Created by James Lawrence Turner on 7/5/13.
//  Copyright (c) 2013 URX. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 Helper methods to encode/decode data in proper URI format and to launch other apps, enabling the web of mobile apps.
 */
@interface TPHelper : NSObject

///-------------------------------
/// @name Creating & Parsing Query Strings
///-------------------------------
/**
 Creates a URI encoded query string from an NSDictionary, which can be used for passing data to apps.
 @param queryParameters The data to encode as a URI encoded query string.
 @return A URI encoded query string.
 */
+ (NSString *)queryStringFromDictionary:(NSDictionary *)queryParameters;

/**
 Takes a query string with URI encode parameters and returns an NSDictionary with string values.
 @param queryString A query string to parse.
 @return An NSDictionary of parameters from the query string.
 */
+ (NSDictionary *)dictionaryFromQueryString:(NSString *)queryString;

///-------------------------------
/// @name URI Encoding & Decoding
///-------------------------------
/**
 Encode a string into a proper, percent escaped string.
 @param url The URL the OS will open (i.e. "http://google.com" for a web link or "myapp:someitem/something" for an app deep link)
 */
+ (NSString *)encodeURI:(NSString *)string;

/**
 A convenience method for opening a URL using `UIApplication` `canOpenURL:`.
 @param url The URL to try and open.
 @return If no apps respond to the URL, this will return `NO`, otherwise it will return `YES`.
 */
+ (NSString *)decodeURI:(NSString *)string;

///-------------------------------
/// @name Launching External Apps
///-------------------------------
/**
 A convenience method for opening a URL using `UIApplication` `openURL:`.
 @param url The URL the OS will open (i.e. "http://google.com" for a web link or "myapp:someitem/something" for an app deep link)
 */
+ (void)invokeExternalURL:(NSString *)url;

/**
 A convenience method for opening a URL using `UIApplication` `openURL:` with deep link specific parameters.
 @param schema The custom schema of the app you're trying to open.
 @param route The route of the app you're trying to open.
 @param queryParameters The query parameters you wish to send to the app as a query string.
 */
+ (void)invokeExternalAppWithSchema:(NSString *)schema Route:(NSString *)route AndQueryParameters:(NSDictionary *)queryParameters;

/**
 A convenience method for opening a URL using `UIApplication` `canOpenURL:`.
 @param url The URL to try and open.
 @return If no apps respond to the URL, this will return `NO`, otherwise it will return `YES`.
 */
+ (BOOL)canInvokeExternalURL:(NSString *)url;

@end
