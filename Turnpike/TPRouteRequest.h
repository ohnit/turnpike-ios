//
//  TPRouteRequest.h
//  Turnpike
//
//  Created by James Lawrence Turner on 7/5/13.
//  Copyright (c) 2013 URX. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 ## Overview
 `TPRouteRequest` is an object which holds data about the route being invoked. It is created by the `TPRouter` when invoking a route, and is passed to both filters and the matched (or default) route callback.
 
 The `TPRouteRequest` contains both external invocation specific information, as well as route specific information. If you are writing a route or filter, you should check to see if any of the fields are `nil`, for any of the reasons listed below.
 
 ### External Invocation Specific Information
 Of the `TPRouteRequest`'s 4 properties, 2 of them, the `urlSchema` and `queryParameters`, are used pass information from outside the app. If invoked internally, these values will be `nil`.
 
 The `urlSchema` should be one of the custom URL schemas that your app has registered (see "Implementing Custom URL Schemes" in [http://developer.apple.com/library/ios/#documentation/iPhone/Conceptual/iPhoneOSProgrammingGuide/AdvancedAppTricks/AdvancedAppTricks.html](Apple's Advanced App Tricks documentation)). If you are invoking a route internally, this value will be `nil`.
 
 The `queryParameters` are `NSString` key/value pairs parsed from the query string (if it has one). If you are inovking a route internally, this value will be nil.
 
 ### Route Specific Information
 The route specific information tells you about the route that was matched. If a route was not matched (in which case the default route was invoked), these values will be `nil`.
 
 The `matchedRoute` is the defined route (in route format, ie `user/:user_id') which matched the incoming route. If no match was found, the default route will be invoked and this will be `nil`.
 
 The `routeParameters` is an `NSDictionary` of route parameters found in the matched route. If no route parameters are found this will be an empty `NSDictionary`, and if no matched route was found, this will be `nil`.
 
 ## Example Usage
 
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 if(request.queryParameters && [request.queryParameters valueForKey:@"coupon_id"]) {
 [CouponProcessor validateAndProcessCoupon:[request.queryParameters valueForKey:@"coupon_id"]];
 }
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 if(request.matchedRoute) {
 [MyAwesomeLoggingService logRoute:request.matchedRoute WithParameters:request.routeParameters];
 }
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 
 */
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
