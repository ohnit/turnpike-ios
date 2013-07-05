//
//  TPAnonymousFilter.h
//  Turnpike
//
//  Created by James Lawrence Turner on 7/5/13.
//  Copyright (c) 2013 URX. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "TPFilterProtocol.h"

/**
 An alias for `(^TPFilterBlock)(TPRouteRequest *request, TPFilterChain *filterChain)`. Used as the block for processing filters.
 */
typedef void (^TPFilterBlock)(TPRouteRequest *request, TPFilterChain *filterChain);

/**
 ## Overview
 The `TPAnonymousFilter` is an object which responds to the `TPFilterProtocol`, and which lets the user of this object define the behavior of the filter through a block.
 
 `TPAnonymousFilter`s are useful when making a filter to handle internal logic, where as creating your own class is useful when creating redistributable classes. The main advantage of `TPAnonymousFilter`s is being able to avoid the boilerplate required for creating a new class.
 
 ### How To Use
 To use the `TPAnonymousFilter` you can either create a filter object with the factory method `+ (id<TPFilterProtocol>) filterWithBlock:(TPFilterBlock)filterBlock`, or more conveniently, you can create them from your `TPRouter` by calling `- (void)addAnonymousFilter:(TPFilterBlock)filterBlock` or from `Routable` by calling `+ (void)addAnonymousFilter:(TPFilterBlock)filterBlock`.
 
 ## Example Usage
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 [Routable addAnonymousFilter:^(TPRouteRequest *request, TPFilterChain *filterChain) {
 if(request.matchedRoute) {
 [MyAwesomeLoggingService logRoute:request.matchedRoute WithParameters:request.routeParameters];
 }
 [filterChain doFilterWithRequest:request];
 }];
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
@interface TPAnonymousFilter : NSObject <TPFilterProtocol>
///-------------------------------
/// @name Factory Method
///-------------------------------
/**
 Create a `TPAnonymousFilter` with a `TPFilterBlock`.
 @param filterBlock The `TPFilterBlock` to perform as the filter logic.
 @return The new `TPAnonymousFilter` object as an `id<TPFilterProtocol>`.
 */
+ (id<TPFilterProtocol>) filterWithBlock:(TPFilterBlock)filterBlock;

@end