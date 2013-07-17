.. _overview:

###############
Getting Started
###############

.. highlight:: text

.. rubric:: *Overview* of Turnpike

.. _o-main-functions:

Main Functions
==============

Turnpike has four main functions:

- **Mapping Routes**: To map callbacks to routes (either a defined route or a fallback "404" route).
- **Filter Chains**: To hold a list of filters to perform as a filter chain on invoked routes.
- **Invoking URLs & Routes**: To processes incoming routes and URLs with the router's defined routes and filter chains.
- **Launching External Apps**: Helper methods to launch external apps, enabling the web of mobile apps.
 
To resolve a deeplink URI, Turnpike does the following:

- Searches for a matching defined route, and falls back on the default route if no match is found.
- Applies each filter in the filter chain to the created ``TPRouteRequest`` object.
- Invokes the callback associated with the matched (or default) route.
 
.. _o-mapping-routes:

Mapping Routes
==============
 
**Turnpike** can map both defined routes and a default route.
 
Defined routes are written in "route format", where route segments (constants, such as ``login`` or ``user``) and route parameters (variables, such as ``:user_id`` or ``:page_number``) are separated by the route separator, ``/``.
 
Defined routes can either be static, such as ``about/team``, or can dynamic, such as ``user/:user_id/profile/:comment_id``.

.. o-examples-switch-to-a-tab:

Example: Navigate To a Tab
--------------------------

.. code-block:: objc

    self.tabBarController.selectedIndex = 1;
    [Turnpike mapRoute:@"tab/:tab_name" ToCallback:^(TPRouteRequest *request) {
        int tabIndexToSet = [[request.routeParameters valueForKey:@"tab_name"] isEqualToString:@"Home"] ? 0 : 1;
        [[UIApplication sharedApplication].delegate window].rootViewController.tabBarController.selectedIndex = tabIndexToSet;
    }];
 

In the case of dynamic routes, route parameters can be accessed in the filter or callback through the ``TPRouteRequest``, by calling ``request.routeParameters``. The route ``user/:user_id`` invoked as ``user/32`` would have the route parameter ``:user_id`` accessible in the ``TPRouteRequest`` by calling ``[request.routeParameters valueForKey:@"user_id"]`` which gets the value of ``user_id`` from the ``NSDictionary`` ``routeParameters`` (which would return the ``NSString`` of ``32``).

.. o-examples-product-page:

Example: Push a Product Page onto our UINavigation Controller
-------------------------------------------------------------

.. code-block:: objc

    [Turnpike mapRoute:@"product/:product_id" ToCallback:^(TPRouteRequest *request) {
        UINavigationController *navigationController = (UINavigationController*)self.window.rootViewController;
        [navigationController popToRootViewControllerAnimated:NO];
     
        UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"MainStoryboard" bundle:nil];
        ProductViewController *productViewController = [storyboard instantiateViewControllerWithIdentifier:@"ProductViewController"];
     
        [navigationController pushViewController:productViewController animated:NO];
    }];

 
Defined routes are defined with the method ``+ (void)mapRoute:(NSString *)format ToCallback:(TPRoutingCallback)callback``.
 
The Default Route is invoked when no route is matched. By default, this does nothing and just launches your app.
 
The Default Route is defined with the method ``+ (void)mapDefaultToCallback:(TPRoutingCallback)callback``

.. o-filter-chains:

Filter Chains
=============
 
``Turnpike``'s ``TPRouter`` internally keeps a list of filters, which you can append filters to, and which get invoked when a ``TPRouteRequest`` is created.
 
Filters allow you to perform logic with the incoming ``TPRouteRequest`` before the route's mapped callback is invoked. Filters can be used for authentication, redirecting, analytics, and more.
 
Filters are used in a filter chain, and are executed sequentially. The sequence is defined by the order in which filters are added to the router (the first filter added is the first execute, the last is the last executed).

Filters can either be added with the method ``+ (void)addFilter:(id <TPFilterProtocol>)filter``, or they can be defined with a block and added with the method ``+ (void)addAnonymousFilter:(TPFilterBlock)filterBlock``.

.. o-examples-adding-a-filter:

Example: Adding a Filter
------------------------

.. code-block:: objc

    [Turnpike addAnonymousFilter:^(TPRouteRequest *request, TPFilterChain *filterChain) {
        NSLog(@"Matched Route: %@",request.matchedRoute);
     
        [filterChain.doFilterWithRequest:request];
    }];

 
Filters themselves are objects that respond to ``TPFilterProtocol``, and as such implement ``- (void) doFilterWithRequest:(TPRouteRequest *)request AndFilterChain:(TPFilterChain *)filterChain``. This is the method that is used to process a ``TPRouteRequest``. In this method, the Filter should call either ``[filterChain.doFilterWithRequest:request]`` to continue the route, or invoke another route from the router to "redirect". If the filter chain is not continued, it is forgotten and is ended.

.. o-examples-performing-a-redirect:

Performing a Redirect with a Filter
----------------------------------------

.. code-block:: objc

    [Turnpike addAnonymousFilter:^(TPRouteRequest *request, TPFilterChain *filterChain) {
        if([Sweepstakes hasPlayed] == NO && arc4random() % 1000 == 0) {
            [Turnpike invokeInternalRoute:@"sweepstakes/win"];
        }
        else {
            [filterChain.doFilterWithRequest:request];
        }
        [Sweepstakes setHasPlayed:YES];
    }];

.. o-invoking-urls-and-routes:

Invoking URLs & Routes
======================

Routes can either be invoked internally, as a route, or externally, as a URL. Internal invocation is used to navigate or execute route callbacks from within an app, where as external invocation is used to launch your app from another app and then resolve the route (extracted from the URL).

.. o-examples-invoking-routes-internally:

Invoking Routes Internally
--------------------------

.. code-block:: objc

    [Turnpike invokeInternalRoute:@"about/team"];
 
Internally invoked routes can be invoked at any time during your app's execution. These could be mapped to buttons, gestures, events, etc. These routes are invoked by calling ``+ (void)invokeInternalRoute:(NSString *)route`` and passing a route in route format (described above in mapping routes).
 
Externally invoked routes should be invoked during your ``UIApplicationDelegate``'s  ``– application:handleOpenURL:`` or ``– application:openURL:sourceApplication:annotation:`` method. These routes are invoked with the ``+ (void)invokeExternalRouteFromURL:(NSString *)url`` method and should be passed the URL from either of the ``openURL`` methods from ``UIApplicationDelegate``.
 
.. o-launching-external-apps:

Launching External Apps
=======================

``Turnpike`` provides helper methods that wrap functionality from ``UIApplication`` to detect which URLs applications installed on the device can respond to and also to invoke those routes.

.. o-examples-invoking-external routes:
 
Invoking External Routes
------------------------

.. code-block:: objc

    - (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation {
        [Turnpike invokeExternalRouteFromURL:url];
    }
 
To detect if an app is installed, use ``+ (BOOL)canInvokeExternalURL:(NSString *)url``, and pass in the URL which you intend to invoke. If this returns ``YES``, then use either ``+ (void)invokeExternalURL:(NSString *)url`` or ``+ (void)invokeExternalAppWithSchema:(NSString *)schema Route:(NSString *)route AndQueryParameters:(NSDictionary *)queryParameters``. The later will build the URL for you from the destination app's custom URL schema, the route you wish to invoke, and the query parameters to build into a URI encoded query string.

