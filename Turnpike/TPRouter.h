//
//  TPRouter.h
//  Turnpike
//
//  Created by James Lawrence Turner on 7/5/13.
//  Copyright (c) 2013 URX. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
TPFilalias for `(^TPRoutingCallback)(TPRouteRequest *request)`. Used as the callback block to map to routes.
 */
typedef void (^TPRoutingCallback)(TPRouteRequest *request);

/**
 ## Overview
 `TPRouter` is what maps routes to anonymous functions and allows you to invoke those routes either with a route or a URL.
 
 It has three main functions:
 - Mapping Routes: To map callbacks to routes (either a defined route or a fallback "404" route).
 - Filter Chains: To hold a list of filters to perform as a filter chain on invoked routes.
 - Invoking URLs & Routes: To processes incoming routes and URLs with the router's defined routes and filter chains.
 
 When a route is invoked it:
 - First it searches for a matching defined route, and falls back on the default route if no match is found.
 - Then it applies each filter in the filter chain to the created `TPRouteRequest` object.
 - Finally it invokes the callback associated with the matched (or default) route.
 
 ### Mapping Routes
 
 The `TPRouter` can map both defined routes and a default route.
 
 Defined routes are written in "route format", where route segments (constants, such as `login` or `user`) and route parameters (variables, such as `:user_id` or `:page_number`) are separated by the route separator, `/`.
 
 Defined routes can either be static, such as `about/team`, or can dynamic, such as `user/:user_id/profile/:comment_id`.
 
 In the case of dynamic routes, route parameters can be accessed in the filter or callback through the `TPRouteRequest`, by calling `request.routeParameters`. The route `user/:user_id` invoked as `user/32` would have the route parameter `:user_id` accessible in the `TPRouteRequest` by calling `[request.routeParameters valueForKey:@"user_id"]` which gets the value of `user_id` from the `NSDictionary` `routeParameters` (which would return the `NSString` of `32`).
 
 Defined routes are defined with the method `- (void)mapRoute:(NSString *)format ToCallback:(TPRoutingCallback)callback`.
 
 The Default Route is invoked when no route is matched. By default, this does nothing and just launches your app.
 
 The Default Route is defined with the method `- (void)mapDefaultToCallback:(TPRoutingCallback)callback`
 
 ### Filter Chains
 
 The `TPRouter` internally keeps a list of filters, which you can append filters to, and which get invoked when a `TPRouteRequest` is created.
 
 Filters allow you to perform logic with the incoming `TPRouteRequest` before the route's mapped callback is invoked. Filters can be used for authentication, redirecting, analytics, and more.
 
 Filters are used in a filter chain, and are executed sequentially. The sequence is defined by the order in which filters are added to the router (the first filter added is the first execute, the last is the last executed).
 
 Filters themselves are objects that respond to `TPFilterProtocol`, and as such implement `- (void) doFilterWithRequest:(TPRouteRequest *)request AndFilterChain:(TPFilterChain *)filterChain`. This is the method that is used to process a `TPRouteRequest`. In this method, the Filter should call either `[filterChain.doFilterWithRequest:request]` to continue the route, or invoke another route from the router to "redirect". If the filter chain is not continued, it is forgotten and is ended.
 
 Filters can either be added with the method `- (void)addFilter:(id <TPFilterProtocol>)filter`, or they can be defined with a block and added with the method `- (void)addAnonymousFilter:(TPFilterBlock)filterBlock`.
 
 ### Invoking URLs & Routes
 
 Routes can either be invoked internally, as a route, or externally, as a URL. Internal invocation is used to navigate or execute route callbacks from within an app, where as external invocation is used to launch your app from another app and then resolve the route (extracted from the URL).
 
 Internally invoked routes can be invoked at any time during your app's execution. These could be mapped to buttons, gestures, events, etc. These routes are invoked by calling `- (void)invokeInternalRoute:(NSString *)route` and passing a route in route format (described above in mapping routes).
 
 Externally invoked routes should be invoked during your `UIApplicationDelegate`'s  `– application:handleOpenURL:` or `– application:openURL:sourceApplication:annotation:` method. These routes are invoked with the `- (void)invokeExternalRouteFromURL:(NSString *)url` method and should be passed the URL from either of the `openURL` methods from `UIApplicationDelegate`.
 
 ## Example Usage
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 TPRouter *router = [TPRouter router];
 [router mapRoute:@"product/:product_id" ToCallback:^(TPRouteRequest *request) {
 [Products displayProductWithId:[request.routeParameters valueForKey:@"product_id"]];
 }];
 
 [router addAnonymousFilter:^(TPRouteRequest *request, TPFilterChain *filterChain) {
 NSLog(@"%@",request.matchedRoute);
 [filterChain.doFilterWithRequest:request];
 }];
 
 [router invokeInternalRoute:@"product/1988"]
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

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
- (void)mapRoute:(NSString *)format ToCallback:(TPRoutingCallback)callback;

/**
 Set an anonymous callback as the default route to fallback to when no route is matched.
 @param callback The callback to run when the URL is triggered in `open:`
 */
- (void)mapDefaultToCallback:(TPRoutingCallback)callback;

///-------------------------------
/// @name Filter Chains
///-------------------------------

/**
 Add a filter to the filter chain.
 @param filter An object that responds to `<TPFilterProtocol>`. If you would rather use a block than create a new class, use `addNewAnonymousFilter:` instead.
 */
- (void)addFilter:(id <TPFilterProtocol>)filter;
/**
 Add an anonymous filter to the filter chain.
 @param filterBlock a `TPFilterBlock`. If you would rather create a new class than use a block, use `addFilter:` instead.
 */
- (void)addAnonymousFilter:(TPFilterBlock)filterBlock;

///-------------------------------
/// @name Invoking URLs & Routes
///-------------------------------

/**
 Invokes a route's mapped callback from within the app. After passing the request through any filters this router may have, the route's mapped callback gets invoked. Expects a route in route format, rather than a URL.
 @param route The route to invoke, in route format (i.e. `logout`, `users/16`, `about/team/contact`)
 */
- (void)invokeInternalRoute:(NSString *)route;

/**
 Invokes a route's mapped callback from outside the app. After passing the request through any filters this router may have, the route's mapped callback gets invoked. Expects a route in URL form, as passed by `– application:handleOpenURL:` or `– application:openURL:sourceApplication:annotation:`.
 @param url The URL to invoke, as a [URL encoded string in a proper URL format](http://www.ietf.org/rfc/rfc1738.txt "RFC 1738") (i.e. `com.mycompany.MyApp:logout`, `com.mycompany.MyApp:users/16?highlight=portfolio`, `com.mycompany.MyApp:about/team/contact?city=san%20francisco`).
 */
- (void)invokeExternalRouteFromURL:(NSString *)url;

@end
