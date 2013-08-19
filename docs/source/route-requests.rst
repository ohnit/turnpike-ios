

##############
Route Requests
##############

Route Requests hold data about the in-app session created as a user enters an app from a deeplink URI. 

Requests have a lifecycle consisting of 3 stages:

- **Creation**: Requests are created by the ``Router`` when resolving a route. See :ref:`below <rr-route-request-creation>` for details.
- **Filtration**: Requests are passed through :doc:`filters <filter-chains>`
- **Resolution**: After filtration, the ``Destination`` callback is invoked to navigate the user to a predefined page or action within the app. 

The ``TPRouteRequest`` contains both external invocation specific information, as well as route specific information. If you are writing a route or filter, it is important to check for ``nil`` fields in the Route Request.

.. _rr-deeplink-metadata:

Deeplink Metadata
=================

The ``urlSchema`` should be one of the custom URL schemas that :doc:`your app has already registered <enabling-mobile-deeplinking>` earlier in the Turnpike documentation. 

.. note::
   
   For more information about iOS deeplink support, please see "`Implementing Custom URL Schemes`_" in Apple's Advanced App Tricks documentation). 

.. _Implementing Custom URL Schemes: http://developer.apple.com/library/ios/documentation/iPhone/Conceptual/iPhoneOSProgrammingGuide/AdvancedAppTricks/AdvancedAppTricks.html#//apple_ref/doc/uid/TP40007072-CH7-SW18

Two of ``TPRouteRequest``'s properties are used pass information specific to deeplink traffic from outside the app: 

- ``urlSchema`` 
- ``queryParameters``

.. code-block:: objc

	if(request.queryParameters && [request.queryParameters valueForKey:@"coupon_id"]) {
	    [CouponProcessor validateAndProcessCoupon:[request.queryParameters valueForKey:@"coupon_id"]];
	}

The ``queryParameters`` are ``NSString`` key/value pairs parsed from the query string (if it has one). If you are inovking a route internally, this value will be nil.

.. _rr-route-request-creation:

Route Request Creation
======================

During Request **Creation**, a ``Router`` attempts to find a matching route for an incoming deeplink URI, ``matchedRoute`` is set to the corresponding defined route (in route format, ie ``user/:user_id``). If no match was found, the default route will be invoked and this will be ``nil``.

After route matching, the ``Router`` parses out route parameters (from variables defined in dynamic routes) and query parameters. Query params and route params are passed into the new Request, along with the matched route and its destination. If the incoming route did not match any defined routes, a ``nil`` route will be passed into the request along with a default Destination.

.. code-block:: objc

	if(request.matchedRoute) {
	    [MyAwesomeLoggingService logRoute:request.matchedRoute WithParameters:request.routeParameters];
	}

The ``routeParameters`` is an ``NSDictionary`` of route parameters found in the matched route. If no route parameters are found this will be an empty ``NSDictionary``, and if no matched route was found, this will be ``nil``.