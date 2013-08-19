

#######################
Filters & Filter Chains
#######################

Filters allow you to perform logic with the incoming Route Request before the route's mapped destination is resolved. Filters can be used for authentication, redirecting, analytics, and more. Turnpike's implementation of filters uses a simplified version of the `intercepting filter`_ pattern.

.. _intercepting filter: http://www.oracle.com/technetwork/java/interceptingfilter-142169.html

.. note::
   The goal of routing filters is to reduce code duplication for cross-cutting concerns that should be evaluated on each incoming deeplink (for instance, tracking UTM campaign query parameters on inbound deeplinks).

Filters are processed sequentially in a :ref:`Filter Chain <fc-filter-chain>`. Each ``Router`` has a single ``Filter Chain``; each individual filter runs once on each request processed by the ``Router``. 

Filters can either be added to the filter chain in one of two ways:

- :ref:`Filters <fc-filters>` implementing ``TPFilterProtocol`` can be added using ``appendFilter``. 
- :ref:`Anonymous Filters <fc-anonymous-filters>` can be defined with a block and added using ``appendAnonymousFilter``.

.. _fc-filters:

Filters
=======

.. _fc-implementing-a-filter:

Implementing a Filter
---------------------

Filters themselves are objects that respond to ``TPFilterProtocol``, and as such implement the following method:

.. code-block:: objc

   - (void) doFilterWithRequest:(TPRouteRequest *)request AndFilterChain:(TPFilterChain *)filterChain 

This is the method that is used to process a ``TPRouteRequest``. To pass processing on to the next filter, the active filter should call 

.. code-block:: objc

   [filterChain.doFilterWithRequest:request]

.. _fc-examples-processing-query-params:

Processing Query Parameters with a Filter
-----------------------------------------

.. code-block:: objc

    @implementation MyCouponFilter
    - (void) doFilterWithRequest:(TPRouteRequest *)request AndFilterChain:(TPFilterChain *)filterChain {
        if(request.queryParameters && [request.queryParameters valueForKey:@"coupon_id"]) {
            [CouponProcessor validateAndProcessCoupon:[request.queryParameters valueForKey:@"coupon_id"]];
        }
        [filterChain.doFilterWithRequest:request];
    }
    @end


.. _fc-anonymous-filters:

Anonymous Filters
=================

The ``TPAnonymousFilter`` is an object which responds to the ``TPFilterProtocol``, and which lets the user of this object define the behavior of the filter through a block.

``TPAnonymousFilter``'s are useful when making a filter to handle internal logic, where as creating your own class is useful when creating redistributable classes. The main advantage of ``TPAnonymousFilter``'s is being able to avoid the boilerplate required for creating a new class.

.. _fc-implementing-anonymous:

Implementing an Anonymous Filter
--------------------------------

To use the ``TPAnonymousFilter`` you can create a filter object with the factory method ``+ (id<TPFilterProtocol>) filterWithBlock:(TPFilterBlock)filterBlock``.

.. code-block:: objc
    
    [Turnpike addAnonymousFilter:^(TPRouteRequest *request, TPFilterChain *filterChain) {
        if(request.matchedRoute) {
            [MyAwesomeLoggingService logRoute:request.matchedRoute WithParameters:request.routeParameters];
        }
        [filterChain doFilterWithRequest:request];
    }];


.. _fc-filter-chain:

Filter Chain
============

The ``TPFilterChain`` is a queue of filters which get processed as its created. In processing each filter, the filter can decide to continue or abandon the filter chain. Should each filter continue the filter chain until there are no more filters left, the ``TPRoutingCallback`` associated with the route from the ``TPRouteRequest`` is invoked and the chain is complete.

How to inteface with the Filter Chain
-------------------------------------

Filter chains are created by a ``TPRouter`` when a route or URL is invoked. The filters in the filter chain are supplied by the router in the order in which they were added to the router.

.. note::
   The order of execution is defined by the order in which filters are added to the router (the first filter added is the first execute, the last is the last executed). Each filter passes the current request (after processing and/or modifying the request) to the next filter in the filter chain.

The typical way to interface with ``TPFilterChain`` is in your filter's logic when creating a filter. In your filter logic, if you want to continue the filter chain with the current route, you need to call ``[filterChain doFilterWithRequest:request]``. You should not need to call the ``TPFilterChain``'s constructor, unless you are subclassing ``TPRouter`` in some special way.
