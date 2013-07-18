//
//  TPRouteRequest.h
//  Turnpike
//
//  Created by James Lawrence Turner on 7/5/13.
//  Copyright (c) 2013 URX. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface TPRouteRequest : NSObject

///-------------------------------
/// @name Factory Method
///-------------------------------

/**
 Creates a new instance of `TPRouteRequest` with the proper configuration.
 @param urlSchema The URL schema of the incoming URL. If invoked internally, this should be `nil`.
 @param queryParameters The query parameters parsed from the query string into `NSString`s in an `NSDictionary`, and if invoked internally, this should be `nil`.
 @param matchedRoute Which defined route (in route format, ie `user/:user_id') matched the incoming route. If no match was found, the default route will be invoked and this should be `nil`.
 @param routeParameters An `NSDictionary` of route parameters found in the matched route. If no route parameters are found this will be an empty `NSDictionary`, and if no matched route was found, this should be `nil`.
 @return A new instance of `TPRouteRequest`.
 */
+(TPRouteRequest *)routeRequestWithURLSchema:(NSString *)urlSchema QueryParameters:(NSDictionary *)queryParameters MatchedRoute:(NSString *)matchedRoute AndRouteParameters:(NSDictionary *)routeParameters;

///-------------------------------
/// @name Properties
///-------------------------------
/**
 The URL schema of the incoming URL. If invoked internally, this will be `nil`.
 */
@property (strong, nonatomic, readonly) NSString *urlSchema;
/**
 The query parameters parsed from the query string into `NSString`s in an `NSDictionary`. If no query parameters are found this will be an empty `NSDictionary`, and if invoked internally, this will be `nil`.
 */
@property (strong, nonatomic, readonly) NSDictionary *queryParameters;
/**
 Which defined route (in route format, ie `user/:user_id') was matched. If no match was found, the default route will be invoked and this will be `nil`.
 */
@property (strong, nonatomic, readonly) NSString *matchedRoute;
/**
 An `NSDictionary` of route parameters found in the matched route. If no route parameters are found this will be an empty `NSDictionary`, and if no matched route was found, this will be `nil`.
 */
@property (strong, nonatomic, readonly) NSDictionary *routeParameters;

@end
