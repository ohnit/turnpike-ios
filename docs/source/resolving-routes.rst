################
Resolving Routes
################

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
