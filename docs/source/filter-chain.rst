########
Overview
########

The ``TPFilterChain`` is a a queue of filters which get processed as its created. In processing each filter, the filter can decide to continue or abandon the filter chain. Should each filter continue the filter chain until there are no more filters left, the ``TPRoutingCallback`` associated with the route from the ``TPRouteRequest`` is invoked and the chain is complete.

#####################################
How to inteface with the Filter Chain
#####################################

Filter chains are created by a ``TPRouter`` when a route or URL is invoked. The filters in the filter chain are supplied by the router in the order in which they were added to the router.

The typical way to interface with ``TPFilterChain`` is in your filter's logic when creating a filter. In your filter logic, if you want to continue the filter chain with the current route, you need to call ``[filterChain doFilterWithRequest:request]``. You should not need to call the ``TPFilterChain``'s constructor, unless you are subclassing ``TPRouter`` in some special way.

#############
Example Usage
#############

.. codeblock: objc

    @implementation MyCouponFilter
    - (void) doFilterWithRequest:(TPRouteRequest *)request AndFilterChain:(TPFilterChain *)filterChain {
        if(request.queryParameters && [request.queryParameters valueForKey:@"coupon_id"]) {
            [CouponProcessor validateAndProcessCoupon:[request.queryParameters valueForKey:@"coupon_id"]];
        }
        [filterChain.doFilterWithRequest:request];
    }
    @end
