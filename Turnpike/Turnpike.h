//
//  Turnpike.h
//  Turnpike
//
//  Created by James Lawrence Turner on 7/5/13.
//  Copyright (c) 2013 URX. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "TPRouter.h"

/**
 ## Overview
 `Turnpike` is a wrapper for a shared `TPRouter` object. Most apps will only need one `TPRouter` object, and for convenience sake, the `Turnpike` class exposes the instance methods of the a shared `TPRouter` as class methods. `Turnpike` also provides some helper methods which wrap native functionality for launching other apps.
 
 Turnpike has four main functions:
 - Mapping Routes: To map callbacks to routes (either a defined route or a fallback "404" route).
 - Filter Chains: To hold a list of filters to perform as a filter chain on invoked routes.
 - Invoking URLs & Routes: To processes incoming routes and URLs with the router's defined routes and filter chains.
 - Launching External Apps: Helper methods to launch external apps, enabling the web of mobile apps.
 
 When a route is invoked it:
 - First it searches for a matching defined route, and falls back on the default route if no match is found.
 - Then it applies each filter in the filter chain to the created `TPRouteRequest` object.
 - Finally it invokes the callback associated with the matched (or default) route.
 
 ### Mapping Routes
 
 `Turnpike` can map both defined routes and a default route.
 
 Defined routes are written in "route format", where rqoute segments (constants, such as `login` or `user`) and route parameters (variables, such as `:user_id` or `:page_number`) are separated by the route separator, `/`.
 
 Defined routes can either be static, such as `about/team`, or can dynamic, such as `user/:user_id/profile/:comment_id`.
 
 In the case of dynamic routes, route parameters can be accessed in the filter or callback through the `TPRouteRequest`, by calling `request.routeParameters`. The route `user/:user_id` invoked as `user/32` would have the route parameter `:user_id` accessible in the `TPRouteRequest` by calling `[request.routeParameters valueForKey:@"user_id"]` which gets the value of `user_id` from the `NSDictionary` `routeParameters` (which would return the `NSString` of `32`).
 
 Defined routes are defined with the method `+ (void)mapRoute:(NSString *)format ToCallback:(TPRoutingCallback)callback`.
 
 The Default Route is invoked when no route is matched. By default, this does nothing and just launches your app.
 
 The Default Route is defined with the method `+ (void)mapDefaultToCallback:(TPRoutingCallback)callback`
 
 ### Filter Chains
 
 `Turnpike`'s `TPRouter` internally keeps a list of filters, which you can append filters to, and which get invoked when a `TPRouteRequest` is created.
 
 Filters allow you to perform logic with the incoming `TPRouteRequest` before the route's mapped callback is invoked. Filters can be used for authentication, redirecting, analytics, and more.
 
 Filters are used in a filter chain, and are executed sequentially. The sequence is defined by the order in which filters are added to the router (the first filter added is the first execute, the last is the last executed).
 
 Filters themselves are objects that respond to `TPFilterProtocol`, and as such implement `- (void) doFilterWithRequest:(TPRouteRequest *)request AndFilterChain:(TPFilterChain *)filterChain`. This is the method that is used to process a `TPRouteRequest`. In this method, the Filter should call either `[filterChain.doFilterWithRequest:request]` to continue the route, or invoke another route from the router to "redirect". If the filter chain is not continued, it is forgotten and is ended.
 
 Filters can either be added with the method `+ (void)addFilter:(id <TPFilterProtocol>)filter`, or they can be defined with a block and added with the method `+ (void)addAnonymousFilter:(TPFilterBlock)filterBlock`.
 
 ### Invoking URLs & Routes
 
 Routes can either be invoked internally, as a route, or externally, as a URL. Internal invocation is used to navigate or execute route callbacks from within an app, where as external invocation is used to launch your app from another app and then resolve the route (extracted from the URL).
 
 Internally invoked routes can be invoked at any time during your app's execution. These could be mapped to buttons, gestures, events, etc. These routes are invoked by calling `+ (void)invokeInternalRoute:(NSString *)route` and passing a route in route format (described above in mapping routes).
 
 Externally invoked routes should be invoked during your `UIApplicationDelegate`'s  `– application:handleOpenURL:` or `– application:openURL:sourceApplication:annotation:` method. These routes are invoked with the `+ (void)invokeExternalRouteFromURL:(NSString *)url` method and should be passed the URL from either of the `openURL` methods from `UIApplicationDelegate`.
 
 ### Launching External Apps
 
 `Turnpike` provides helper methods that wrap functionality from `UIApplication` to detect which URLs applications installed on the device can respond to and also to invoke those routes.
 
 To detect if an app is installed, use `+ (BOOL)canInvokeExternalURL:(NSString *)url`, and pass in the URL which you intend to invoke. If this returns `YES`, then use either `+ (void)invokeExternalURL:(NSString *)url` or `+ (void)invokeExternalAppWithSchema:(NSString *)schema Route:(NSString *)route AndQueryParameters:(NSDictionary *)queryParameters`. The later will build the URL for you from the destination app's custom URL schema, the route you wish to invoke, and the query parameters to build into a URI encoded query string.
 
 ## Example Usage
 
 ### Routing
 
 #### Switch To a Tab
 self.tabBarController.selectedIndex = 1;
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 [Turnpike mapRoute:@"tab/:tab_name" ToCallback:^(TPRouteRequest *request) {
 int tabIndexToSet = [[request.routeParameters valueForKey:@"tab_name"] isEqualToString:@"Home"] ? 0 : 1;
 [[UIApplication sharedApplication].delegate window].rootViewController.tabBarController.selectedIndex = tabIndexToSet;
 }];
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 
 #### Push a Product Page onto our UINavigation Controller
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 [Turnpike mapRoute:@"product/:product_id" ToCallback:^(TPRouteRequest *request) {
 UINavigationController *navigationController = (UINavigationController*)self.window.rootViewController;
 [navigationController popToRootViewControllerAnimated:NO];
 
 UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"MainStoryboard" bundle:nil];
 ProductViewController *productViewController = [storyboard instantiateViewControllerWithIdentifier:@"ProductViewController"];
 
 [navigationController pushViewController:productViewController animated:NO];
 }];
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 
 ### Filter Chains
 
 #### Adding a Filter
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 [Turnpike addAnonymousFilter:^(TPRouteRequest *request, TPFilterChain *filterChain) {
 NSLog(@"Matched Route: %@",request.matchedRoute);
 
 [filterChain.doFilterWithRequest:request];
 }];
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 
 #### Performing a Redirect with a Filter
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 [Turnpike addAnonymousFilter:^(TPRouteRequest *request, TPFilterChain *filterChain) {
 if([Sweepstakes hasPlayed] == NO && arc4random() % 1000 == 0) {
 [Turnpike invokeInternalRoute:@"sweepstakes/win"];
 }
 else {
 [filterChain.doFilterWithRequest:request];
 }
 [Sweepstakes setHasPlayed:YES];
 }];
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 
 ### Invoking URLs & Routes
 
 #### Invoking Routes Internally
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 [Turnpike invokeInternalRoute:@"about/team"];
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 
 #### Invoking External Routes
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 - (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation {
 [Turnpike invokeExternalRouteFromURL:url];
 }
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
+ (void)mapRoute:(NSString *)format ToCallback:(TPRoutingCallback)callback;

/**
 Set an anonymous callback as the default route to fallback to when no route is matched.
 @param callback The callback to run when the URL is triggered in `open:`
 */
+ (void)mapDefaultToCallback:(TPRoutingCallback)callback;

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
 Invokes a route's mapped callback from within the app. After passing the request through any filters this router may have, the route's mapped callback gets invoked. Expects a route in route format, rather than a URL.
 @param route The route to invoke, in route format (i.e. `logout`, `users/16`, `about/team/contact`)
 */
+ (void)invokeInternalRoute:(NSString *)route;

/**
 Invokes a route's mapped callback from outside the app. After passing the request through any filters this router may have, the route's mapped callback gets invoked. Expects a route in URL form, as passed by `– application:handleOpenURL:` or `– application:openURL:sourceApplication:annotation:`.
 @param url The URL to invoke, as a [URL encoded string in a proper URL format](http://www.ietf.org/rfc/rfc1738.txt "RFC 1738") (i.e. `com.mycompany.MyApp:logout`, `com.mycompany.MyApp:users/16?highlight=portfolio`, `com.mycompany.MyApp:about/team/contact?city=san%20francisco`).
 */
+ (void)invokeExternalRouteFromURL:(NSString *)url;

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
