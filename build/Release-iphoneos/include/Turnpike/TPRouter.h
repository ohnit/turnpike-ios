//
//  TPRouter.h
//  Turnpike
//
//  Created by James Lawrence Turner on 7/5/13.
//  Copyright (c) 2013 URX. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "TPRouteRequest.h"
#import "TPAnonymousFilter.h"

/**
Alias for `(^TPRoutingCallback)(TPRouteRequest *request)`. Used as the callback block to map to routes.
 */
typedef void (^TPRouteDestination)(TPRouteRequest *request);

@interface TPRouter : NSObject

///-------------------------------
/// @name Factory Method
///-------------------------------

/**
 Creates a new instance of `TPRouter`.
 @return A new instance of `TPRouter`.
 */
+ (TPRouter *)router;


///-------------------------------
/// @name Mapping Routes
///-------------------------------

/**
 Map a route to an anonymous callback.
 @param format A route in route format (i.e. "users/:id" or "logout")
 @param callback The callback to run when the route is invoked
 */
- (void)mapRoute:(NSString *)route ToDestination:(TPRouteDestination)destination;

/**
 Set an anonymous callback as the default route to fallback to when no route is matched.
 @param callback The callback to run when the URL is triggered in `open:`
 */
- (void)mapDefaultToDestination:(TPRouteDestination)destination;

///-------------------------------
/// @name Filter Chains
///-------------------------------

/**
 Add a filter to the filter chain.
 @param filter An object that responds to `<TPFilterProtocol>`. If you would rather use a block than create a new class, use `addNewAnonymousFilter:` instead.
 */
- (void)appendFilter:(id <TPFilterProtocol>)filter;
/**
 Add an anonymous filter to the filter chain.
 @param filterBlock a `TPFilterBlock`. If you would rather create a new class than use a block, use `addFilter:` instead.
 */
- (void)appendAnonymousFilter:(TPFilterBlock)filterBlock;

///-------------------------------
/// @name Invoking URLs & Routes
///-------------------------------

/**
 Invokes a route's mapped callback from outside the app. After passing the request through any filters this router may have, the route's mapped callback gets invoked. Expects a route in URL form, as passed by `– application:handleOpenURL:` or `– application:openURL:sourceApplication:annotation:`.
 @param url The URL to invoke, as a [URL encoded string in a proper URL format](http://www.ietf.org/rfc/rfc1738.txt "RFC 1738") (i.e. `com.mycompany.MyApp:logout`, `com.mycompany.MyApp:users/16?highlight=portfolio`, `com.mycompany.MyApp:about/team/contact?city=san%20francisco`).
 */
- (void)resolveURL:(NSURL *)url;

@end
