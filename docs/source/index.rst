.. title:: Mobile Deeplinks with Turnpike - Getting Started


.. raw:: html

   <div style="display:none;">

.. toctree::
   :maxdepth: 1

   self
   mapping-routes
   router
   route-request
   filter-chain

.. raw:: html

   </div>


##############################
Mobile Deeplinks with Turnpike
##############################

.. highlight:: text

.. rubric:: Developed by URX__ to enable mobile deeplinking on iOS and Android platforms.

.. __: http://www.urx.com/

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

.. _filter-chains:

Filter Chains
=============
 
``Turnpike``'s ``TPRouter`` internally keeps a list of filters, which you can append filters to, and which get invoked when a ``TPRouteRequest`` is created.
 
Filters allow you to perform logic with the incoming ``TPRouteRequest`` before the route's mapped callback is invoked. Filters can be used for authentication, redirecting, analytics, and more.
 
Filters are used in a filter chain, and are executed sequentially. The sequence is defined by the order in which filters are added to the router (the first filter added is the first execute, the last is the last executed).

Filters can either be added with the method ``+ (void)addFilter:(id <TPFilterProtocol>)filter``, or they can be defined with a block and added with the method ``+ (void)addAnonymousFilter:(TPFilterBlock)filterBlock``.

.. _examples-adding-a-filter:

Example: Adding a Filter
------------------------

.. code-block:: objc

    [Turnpike addAnonymousFilter:^(TPRouteRequest *request, TPFilterChain *filterChain) {
        NSLog(@"Matched Route: %@",request.matchedRoute);
     
        [filterChain.doFilterWithRequest:request];
    }];

 
Filters themselves are objects that respond to ``TPFilterProtocol``, and as such implement ``- (void) doFilterWithRequest:(TPRouteRequest *)request AndFilterChain:(TPFilterChain *)filterChain``. This is the method that is used to process a ``TPRouteRequest``. In this method, the Filter should call either ``[filterChain.doFilterWithRequest:request]`` to continue the route, or invoke another route from the router to "redirect". If the filter chain is not continued, it is forgotten and is ended.

.. _examples-performing-a-redirect:

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

.. _invoking-urls-and-routes:

Invoking URLs & Routes
======================

Routes can either be invoked internally, as a route, or externally, as a URL. Internal invocation is used to navigate or execute route callbacks from within an app, where as external invocation is used to launch your app from another app and then resolve the route (extracted from the URL).

.. _examples-invoking-routes-internally:

Invoking Routes Internally
--------------------------

.. code-block:: objc

    [Turnpike invokeInternalRoute:@"about/team"];
 
Internally invoked routes can be invoked at any time during your app's execution. These could be mapped to buttons, gestures, events, etc. These routes are invoked by calling ``+ (void)invokeInternalRoute:(NSString *)route`` and passing a route in route format (described above in mapping routes).
 
Externally invoked routes should be invoked during your ``UIApplicationDelegate``'s  ``– application:handleOpenURL:`` or ``– application:openURL:sourceApplication:annotation:`` method. These routes are invoked with the ``+ (void)invokeExternalRouteFromURL:(NSString *)url`` method and should be passed the URL from either of the ``openURL`` methods from ``UIApplicationDelegate``.
 
.. _launching-external-apps:

Launching External Apps
=======================

``Turnpike`` provides helper methods that wrap functionality from ``UIApplication`` to detect which URLs applications installed on the device can respond to and also to invoke those routes.

.. _examples-invoking-external routes:
 
Invoking External Routes
------------------------

.. code-block:: objc

    - (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation {
        [Turnpike invokeExternalRouteFromURL:url];
    }
 
To detect if an app is installed, use ``+ (BOOL)canInvokeExternalURL:(NSString *)url``, and pass in the URL which you intend to invoke. If this returns ``YES``, then use either ``+ (void)invokeExternalURL:(NSString *)url`` or ``+ (void)invokeExternalAppWithSchema:(NSString *)schema Route:(NSString *)route AndQueryParameters:(NSDictionary *)queryParameters``. The later will build the URL for you from the destination app's custom URL schema, the route you wish to invoke, and the query parameters to build into a URI encoded query string.

