##############
Route Requests
##############

``TPRouteRequest`` is an object which holds data about the route being invoked. It is created by the ``TPRouter`` when invoking a route, and is passed to both filters and the matched (or default) route callback.

The ``TPRouteRequest`` contains both external invocation specific information, as well as route specific information. If you are writing a route or filter, you should check to see if any of the fields are ``nil``, for any of the reasons listed below.

External Invocation Specific Information
========================================

Of the ``TPRouteRequest``'s 4 properties, 2 of them, the ``urlSchema`` and ``queryParameters``, are used pass information from outside the app. If invoked internally, these values will be ``nil``.

.. code-block:: objc

	if(request.queryParameters && [request.queryParameters valueForKey:@"coupon_id"]) {
	    [CouponProcessor validateAndProcessCoupon:[request.queryParameters valueForKey:@"coupon_id"]];
	}

The ``urlSchema`` should be one of the custom URL schemas that your app has registered (see "Implementing Custom URL Schemes" in Apple__'s Advanced App Tricks documentation). If you are invoking a route internally, this value will be ``nil``.

.. __:http://developer.apple.com/library/ios/#documentation/iPhone/Conceptual/iPhoneOSProgrammingGuide/AdvancedAppTricks/AdvancedAppTricks.html

The ``queryParameters`` are ``NSString`` key/value pairs parsed from the query string (if it has one). If you are inovking a route internally, this value will be nil.

Route Specific Information
--------------------------

The route specific information tells you about the route that was matched. If a route was not matched (in which case the default route was invoked), these values will be ``nil``.

The ``matchedRoute`` is the defined route (in route format, ie ``user/:user_id') which matched the incoming route. If no match was found, the default route will be invoked and this will be ``nil``.

The ``routeParameters`` is an ``NSDictionary`` of route parameters found in the matched route. If no route parameters are found this will be an empty ``NSDictionary``, and if no matched route was found, this will be ``nil``.

.. code-block:: objc

	if(request.matchedRoute) {
	    [MyAwesomeLoggingService logRoute:request.matchedRoute WithParameters:request.routeParameters];
	}




