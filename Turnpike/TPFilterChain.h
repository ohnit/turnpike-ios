//
//  TPFilterChain.h
//  Turnpike
//
//  Created by James Lawrence Turner on 7/5/13.
//  Copyright (c) 2013 URX. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "TPRouter.h"

/**
 ## Overview
 The `TPFilterChain` is a a queue of filters which get processed as its created. In processing each filter, the filter can decide to continue or abandon the filter chain. Should each filter continue the filter chain until there are no more filters left, the `TPRoutingCallback` associated with the route from the `TPRouteRequest` is invoked and the chain is complete.
 
 ## How to inteface with the Filter Chain
 Filter chains are created by a `TPRouter` when a route or URL is invoked. The filters in the filter chain are supplied by the router in the order in which they were added to the router.
 
 The typical way to interface with `TPFilterChain` is in your filter's logic when creating a filter. In your filter logic, if you want to continue the filter chain with the current route, you need to call `[filterChain doFilterWithRequest:request]`. You should not need to call the `TPFilterChain`'s constructor, unless you are subclassing `TPRouter` in some special way.
 
 ## Example Usage
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 @implementation MyCouponFilter
 - (void) doFilterWithRequest:(TPRouteRequest *)request AndFilterChain:(TPFilterChain *)filterChain {
 if(request.queryParameters && [request.queryParameters valueForKey:@"coupon_id"]) {
 [CouponProcessor validateAndProcessCoupon:[request.queryParameters valueForKey:@"coupon_id"]];
 }
 [filterChain.doFilterWithRequest:request];
 }
 @end
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
@interface TPFilterChain : NSObject
///-------------------------------
/// @name Factory Method
///-------------------------------
/**
 Create a `TPFilterChain` with an `NSArray` of `id<TPFilterProtocol>`s, a `TPRouteRequest` object, and a `TPRoutingCallback` block.
 @param filters The ordered array of filters to perform.
 @param request The route request to pass to the filters and routing callback.
 @param callback The callback mapped to the invoked route.
 */
+ (void) dofilterChainWithFilters:(NSArray *)filters Request:(TPRouteRequest *)request AndCallback:(TPRouteDestination)callback;
///-------------------------------
/// @name Filter Interaction
///-------------------------------
/**
 Continue the filter chain. If no more filters are left, it will invoke the callback mapped to the invoked route.
 @param request The route request to pass to the filters and routing callback.
 */
- (void) doFilterWithRequest:(TPRouteRequest *)request;

@end