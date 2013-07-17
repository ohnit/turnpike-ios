#######
Filters
#######

``Turnpike``'s ``TPRouter`` internally keeps a list of filters, which you can append filters to, and which get invoked when a ``TPRouteRequest`` is created.
 
Filters allow you to perform logic with the incoming ``TPRouteRequest`` before the route's mapped callback is invoked. Filters can be used for authentication, redirecting, analytics, and more.

Filter Usage
============
 
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


Filters allow you to perform logic with the incoming ``TPRouteRequest`` before the route's mapped callback is invoked. Filters can be used for authentication, redirecting, analytics, and more.

Filters are used in a filter chain, and are executed sequentially. The sequence is defined by the order in which filters are added to the router (the first filter added is the first execute, the last is the last executed).

Filters themselves are objects that respond to ``TPFilterProtocol``, and as such implement ``- (void) doFilterWithRequest:(TPRouteRequest *)request AndFilterChain:(TPFilterChain *)filterChain``. This is the method that is used to process a ``TPRouteRequest``. In this method, the Filter should call either ``[filterChain.doFilterWithRequest:request]`` to continue the route, or invoke another route from the router to "redirect". If the filter chain is not continued, it is forgotten and is ended.

Filters can either be added to a ``TPRouter`` with the method ``- (void)addFilter:(id <TPFilterProtocol>)filter``, or they can be defined with a block and added with the method ``- (void)addAnonymousFilter:(TPFilterBlock)filterBlock``.

Filter Implementation
---------------------

To implement a filter, create a new object that responds to ``TPFilterProtocol``. You must implement the method ``- (void) doFilterWithRequest:(TPRouteRequest *)request AndFilterChain:(TPFilterChain *)filterChain``, and if you want to continue the filter chain, at the end of your implementation you should call ``[filterChain.doFilterWithRequest:request]``. You can also perform a "redirect" by invoking a new route and not calling ``[filterChain.doFilterWithRequest:request]``.

Filters Example
---------------

.. code-block:: objc

	@implementation MyCouponFilter
	- (void) doFilterWithRequest:(TPRouteRequest *)request AndFilterChain:(TPFilterChain *)filterChain {
	    if(request.queryParameters && [request.queryParameters valueForKey:@"coupon_id"]) {
	        [CouponProcessor validateAndProcessCoupon:[request.queryParameters valueForKey:@"coupon_id"]];
	    }
	    [filterChain.doFilterWithRequest:request];
	}
	@end


Filter Chain
============

The ``TPFilterChain`` is a queue of filters which get processed as its created. In processing each filter, the filter can decide to continue or abandon the filter chain. Should each filter continue the filter chain until there are no more filters left, the ``TPRoutingCallback`` associated with the route from the ``TPRouteRequest`` is invoked and the chain is complete.

How to inteface with the Filter Chain
-------------------------------------

Filter chains are created by a ``TPRouter`` when a route or URL is invoked. The filters in the filter chain are supplied by the router in the order in which they were added to the router.

The typical way to interface with ``TPFilterChain`` is in your filter's logic when creating a filter. In your filter logic, if you want to continue the filter chain with the current route, you need to call ``[filterChain doFilterWithRequest:request]``. You should not need to call the ``TPFilterChain``'s constructor, unless you are subclassing ``TPRouter`` in some special way.

Example: Coupon Filter
----------------------

.. code-block:: objc

    @implementation MyCouponFilter
    - (void) doFilterWithRequest:(TPRouteRequest *)request AndFilterChain:(TPFilterChain *)filterChain {
        if(request.queryParameters && [request.queryParameters valueForKey:@"coupon_id"]) {
            [CouponProcessor validateAndProcessCoupon:[request.queryParameters valueForKey:@"coupon_id"]];
        }
        [filterChain.doFilterWithRequest:request];
    }
    @end

Anonymous Filters
=================

The ``TPAnonymousFilter`` is an object which responds to the ``TPFilterProtocol``, and which lets the user of this object define the behavior of the filter through a block.

``TPAnonymousFilter`` s are useful when making a filter to handle internal logic, where as creating your own class is useful when creating redistributable classes. The main advantage of ``TPAnonymousFilter`` s is being able to avoid the boilerplate required for creating a new class.

How To Use Anonymous Filter
---------------------------

To use the ``TPAnonymousFilter`` you can either create a filter object with the factory method ``+ (id<TPFilterProtocol>) filterWithBlock:(TPFilterBlock)filterBlock``, or more conveniently, you can create them from your ``TPRouter`` by calling ``- (void)addAnonymousFilter:(TPFilterBlock)filterBlock`` or from ``Routable`` by calling ``+ (void)addAnonymousFilter:(TPFilterBlock)filterBlock``.

Example Uses of Anonymous Filters
---------------------------------

.. code-block:: objc
    
    [Routable addAnonymousFilter:^(TPRouteRequest *request, TPFilterChain *filterChain) {
        if(request.matchedRoute) {
            [MyAwesomeLoggingService logRoute:request.matchedRoute WithParameters:request.routeParameters];
        }
        [filterChain doFilterWithRequest:request];
    }];
