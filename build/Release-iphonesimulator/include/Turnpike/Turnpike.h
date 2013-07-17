//
//  Turnpike.h
//  Turnpike
//
//  Created by James Lawrence Turner on 7/5/13.
//  Copyright (c) 2013 URX. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "TPFilterChain.h"

/**
  `Turnpike` is a wrapper for a shared `TPRouter` object. Most apps will only need one `TPRouter` object, and for convenience's sake, the `Turnpike` class exposes the instance methods of the a shared `TPRouter` as class methods. `Turnpike` also provides some helper methods which wrap native functionality for launching other apps.
 */
@interface Turnpike : NSObject

///-------------------------------
/// @name Mapping Routes
///-------------------------------

/**
 Map a route to an anonymous callback.
 @param format A route in route format (i.e. "users/:id" or "logout")
 @param callback The callback to run when the route is invoked
 */
+ (void)mapRoute:(NSString *)format ToDestination:(TPRouteDestination)destination;

/**
 Set an anonymous callback as the default route to fallback to when no route is matched.
 @param callback The callback to run when the URL is triggered in `open:`
 */
+ (void)mapDefaultToDestination:(TPRouteDestination)destination;

///-------------------------------
/// @name Filter Chains
///-------------------------------

/**
 Add a filter to the filter chain.
 @param filter An object that responds to `<TPFilterProtocol>`. If you would rather use a block than create a new class, use `addNewAnonymousFilter:` instead.
 */
+ (void)addFilter:(id <TPFilterProtocol>)filter;
/**
 Add an anonymous filter to the filter chain.
 @param filterBlock a `TPFilterBlock`. If you would rather create a new class than use a block, use `addFilter:` instead.
 */
+ (void)addAnonymousFilter:(TPFilterBlock)filterBlock;

///-------------------------------
/// @name Invoking URLs & Routes
///-------------------------------

/**
 Invokes a route's mapped callback from outside the app. After passing the request through any filters this router may have, the route's mapped callback gets invoked. Expects a route in URL form, as passed by `– application:handleOpenURL:` or `– application:openURL:sourceApplication:annotation:`.
 @param url The URL to invoke, as a [URL encoded string in a proper URL format](http://www.ietf.org/rfc/rfc1738.txt "RFC 1738") (i.e. `com.mycompany.MyApp:logout`, `com.mycompany.MyApp:users/16?highlight=portfolio`, `com.mycompany.MyApp:about/team/contact?city=san%20francisco`).
 */
+ (void)resolveURL:(NSURL *)url;

///-------------------------------
/// @name Launching External Apps
///-------------------------------
/**
 A convenience method for opening a URL using `UIApplication` `openURL:`.
 @param url The URL the OS will open (i.e. "http://google.com" for a web link or "myapp:someitem/something" for an app deep link)
 */
+ (void)invokeURL:(NSString *)url;

/**
 A convenience method for opening a URL using `UIApplication` `openURL:` with deep link specific parameters.
 @param schema The custom schema of the app you're trying to open.
 @param route The route of the app you're trying to open.
 @param queryParameters The query parameters you wish to send to the app as a query string.
 */
+ (void)invokeAppWithSchema:(NSString *)schema Route:(NSString *)route AndQueryParameters:(NSDictionary *)queryParameters;

/**
 A convenience method for opening a URL using `UIApplication` `canOpenURL:`.
 @param url The URL to try and open.
 @return If no apps respond to the URL, this will return `NO`, otherwise it will return `YES`.
 */
+ (BOOL)canInvokeURL:(NSString *)url;

+ (BOOL)canInvokeAppWithSchema:(NSString *)schema;

@end
