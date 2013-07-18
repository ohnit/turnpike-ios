//
//  TPFilterChain.h
//  Turnpike
//
//  Created by James Lawrence Turner on 7/5/13.
//  Copyright (c) 2013 URX. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "TPRouter.h"

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