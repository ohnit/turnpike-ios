//
//  TPFilterProtocol.h
//  Turnpike
//
//  Created by James Lawrence Turner on 7/5/13.
//  Copyright (c) 2013 URX. All rights reserved.
//

@class TPRouteRequest;
@class TPFilterChain;

@protocol TPFilterProtocol <NSObject>
///-------------------------------
/// @name Filter Logic
///-------------------------------
/**
 The filter's logic. Can perform actions based on the information provided by the TPRouteRequest.
 @param request The route request, which has data pertaining to the route invocation.
 @param filterChain The filter chain which this filter is a part of. To continue the filter chain, you should call `[filterChain.doFilterWithRequest:request]`.
 */
- (void) doFilterWithRequest:(TPRouteRequest *)request AndFilterChain:(TPFilterChain *)filterChain;

@end